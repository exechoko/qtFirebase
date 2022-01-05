#include "login.h"
#include "ui_login.h"

#include <QMessageBox>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_ingresar_clicked()
{
    //authhandler *authHandler = new authhandler();

    authHandler = new authhandler ();
    //firebaseQT
    authHandler->setAPIKey("YOUR API");

    QString username = ui->username->text();
    QString password = ui->password->text();

    authHandler->signUserIn(username, password);

}
