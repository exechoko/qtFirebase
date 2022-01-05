#include "gestion.h"
#include "ui_gestion.h"

gestion::gestion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gestion)
{
    ui->setupUi(this);
}

gestion::~gestion()
{
    delete ui;
}

void gestion::on_btnAgregar_clicked()
{
    QString nombre;
    QString tipo;

    nombre = ui->edtNombre->text();
    tipo = ui->edtTipo->text();

}
