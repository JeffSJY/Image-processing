#include "dialog3.h"
#include "ui_dialog3.h"
#include"QDebug"


Dialog3::Dialog3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog3)
{
    ui->setupUi(this);
}

Dialog3::~Dialog3()
{
    delete ui;
}

void Dialog3::accept(){
    qDebug()<<"dialog accept";
    bool ok;
    int size=ui->lineEdit->text().toInt(&ok);
    double sigma=ui->lineEdit1->text().toDouble(&ok); //拿到参数c

    emit confirmed(size,sigma); //传递

    close();// close dialog
}
