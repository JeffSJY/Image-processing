#include "dialog4.h"
#include "ui_dialog4.h"
#include"QDebug"

Dialog4::Dialog4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog4)
{
    ui->setupUi(this);
}

Dialog4::~Dialog4()
{
    delete ui;
}
void Dialog4::accept(){
    qDebug()<<"dialog accept";
    bool ok;
    int size=ui->lineEdit->text().toInt(&ok);
    double sigmaColor=ui->lineEdit1->text().toDouble(&ok);
    double sigmaSpace=ui->lineEdit2->text().toDouble(&ok);

    emit confirmed(size,sigmaColor,sigmaSpace); //传递

    close();// close dialog
}
