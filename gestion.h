#ifndef GESTION_H
#define GESTION_H

#include <QWidget>

namespace Ui {
class gestion;
}

class gestion : public QWidget
{
    Q_OBJECT

public:
    explicit gestion(QWidget *parent = nullptr);
    ~gestion();

private slots:
    void on_btnAgregar_clicked();

private:
    Ui::gestion *ui;
};

#endif // GESTION_H
