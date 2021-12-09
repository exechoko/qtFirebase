#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>

authhandler::authhandler(QObject *parent)
    : QObject(parent)
    , m_apiKey( QString() )
{
    m_networkAccessManager = new QNetworkAccessManager( this );
    connect( this, &authhandler::userSignedIn, this, &authhandler::performAuthenticatedDatabaseCall );
}

authhandler::~authhandler()
{
    m_networkAccessManager->deleteLater();
}

void authhandler::setAPIKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void authhandler::signUserUp(const QString &emailAddress, const QString &password)
{
    QString signUpEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );
    performPOST( signUpEndpoint, jsonPayload );
}

void authhandler::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;

    QVariantMap variantPayload;
    variantPayload["email"] = emailAddress;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant( variantPayload );

    performPOST( signInEndpoint, jsonPayload );
}

void authhandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    //qDebug() << response;
    m_networkReply->deleteLater();

    parseResponse( response );
}

void authhandler::performAuthenticatedDatabaseCall()
{
    QString endPoint = "https://app-fire-exe.firebaseio.com/Persona.json?auth=" + m_idToken;
    //QString endPoint = "https://qtfirebaseintegrationexample-default-rtdb.firebaseio.com/Pets.json?auth=" + m_idToken;
    m_networkReply = m_networkAccessManager->get( QNetworkRequest(QUrl(endPoint)));
    connect( m_networkReply, &QNetworkReply::readyRead, this, &authhandler::networkReplyReadyRead );
}

void authhandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest( (QUrl( url )) );
    newRequest.setHeader( QNetworkRequest::ContentTypeHeader, QString( "application/json"));
    m_networkReply = m_networkAccessManager->post( newRequest, payload.toJson());
    connect( m_networkReply, &QNetworkReply::readyRead, this, &authhandler::networkReplyReadyRead );
}

void authhandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
    if ( jsonDocument.object().contains("error") )
    {
        qDebug() << "Error occured!" << response;
        okAuth = false;
    }
    else if ( jsonDocument.object().contains("kind"))
    {
        okAuth = true;
        QString idToken = jsonDocument.object().value("idToken").toString();
        //qDebug() << "Obtained user ID Token: " << idToken;
        qDebug() << "User signed in successfully!";
        m_idToken = idToken;
        emit userSignedIn();
    }
    else {

        qDebug() << "The response was: " << response;
    }

}
