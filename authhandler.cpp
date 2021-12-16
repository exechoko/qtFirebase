#include "authhandler.h"
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


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

bool authhandler::getOKAuth()
{
    return okAuth;
}

void authhandler::setOKAuth(bool _okAuth)
{
    okAuth = _okAuth;
}

void authhandler::setResponse(const QByteArray &response)
{
    this->response = response;
}

const QByteArray authhandler::getResponse()
{
    return this->response;
}

void authhandler::conectarSQL()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //Abrimos la base de datos si falla mostramos el error
    //db.setDatabaseName("basededatos.sqlite");
    db.setDatabaseName("basedatos.db");

    if (!db.open()) {
        QMessageBox::warning(0, "Error", "No se conecto la base de datos.");

    }
}

void authhandler::desconectarSQL()
{
    QSqlDatabase::removeDatabase("basedatos.db");
}

void authhandler::insertarDatosJsonToSQL(QJsonDocument jsonDocument)
{
    /*QJsonValue uid, nombre, imagen, domicilio, dni, causa;
    QJsonArray todasLasPersonas = jsonDocument["uid"].toArray();
    uid = jsonDocument["uid"];
    for (int i = 0; i < todasLasPersonas.size(); i++ ) {
        qDebug() << i;
    }*/

    QJsonValue nombre, tipo;
    QJsonObject objeto = jsonDocument.object();

    QJsonObject nodo = objeto.value("nodo1").toObject();
    nombre = nodo.value("nombre");
    tipo = nodo.value("tipo");
    qDebug() << nombre.toString() << " - " << tipo.toString();


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
    //QString endPoint = "https://app-fire-exe.firebaseio.com/Persona.json?auth=" + m_idToken;
    QString endPoint = "https://fir-qt-8bef7-default-rtdb.firebaseio.com/Mascotas.json?auth=" + m_idToken;
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
    QMessageBox mensaje;
    bool ingreso = false;
    QJsonDocument jsonDocument = QJsonDocument::fromJson( response );
    if ( jsonDocument.object().contains("error") ) {
        qDebug() << "Error occured!" << response;
        mensaje.setText("Error al iniciar sesión");
        mensaje.exec();
    } else if ( jsonDocument.object().contains("kind")) {
        QString idToken = jsonDocument.object().value("idToken").toString();
        //qDebug() << "Obtained user ID Token: " << idToken;
        qDebug() << "User signed in successfully!";
        m_idToken = idToken;
        emit userSignedIn();
        mensaje.setText("Ingreso correctamente");
        mensaje.exec();


    } else {
        ingreso = true;
        qDebug() << "The response was: " << response;
    }

    if(ingreso){
        insertarDatosJsonToSQL(jsonDocument);
    }

}
