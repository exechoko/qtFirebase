#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <authhandler.h>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_ingresar_clicked();

private:
    Ui::login *ui;
    authhandler * authHandler;
    //MainWindow *mw;
    //SeleccionDeBases *select;
};

#endif // LOGIN_H
