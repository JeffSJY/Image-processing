#include "dialog2.h"
#include "ui_dialog2.h"
#include"QDebug"

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::accept(){
    qDebug()<<"dialog accept";
    QString c=ui->lineEdit->text(); //拿到参数c

    emit confirmed(c); //传递

    close();// close dialog
}
