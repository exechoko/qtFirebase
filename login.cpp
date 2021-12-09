#include "login.h"
#include "ui_login.h"
#include <authhandler.h>
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
    authhandler authHandler;
    authHandler.setAPIKey("AIzaSyDHOnU8T7SLeVuvumiMlf931vI8whaQ4Mg");

    QString username = ui->username->text();
    QString password = ui->password->text();

    authHandler.signUserIn(username, password);

    if (authHandler.okAuth){
        QMessageBox::information(this, "Ingresor", "Ingreso correctamente");
    } else {
        QMessageBox::warning(this, "Error", "");
    }


}
