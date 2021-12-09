#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class authhandler : public QObject
{
    Q_OBJECT
public:
    explicit authhandler(QObject *parent = nullptr);
    ~authhandler();
    void setAPIKey( const QString & apiKey );
    void signUserUp( const QString & emailAddress, const QString & password );
    void signUserIn( const QString & emailAddress, const QString & password );
    bool okAuth;

public slots:
    void networkReplyReadyRead();

    void performAuthenticatedDatabaseCall();

signals:
    void userSignedIn();
private:
    void performPOST( const QString & url, const QJsonDocument & payload );
    void parseResponse( const QByteArray & reponse );
    QString m_apiKey;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkReply * m_networkReply;
    QString m_idToken;
};

#endif // AUTHHANDLER_H
