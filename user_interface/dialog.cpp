#include "dialog.h"
#include "ui_dialog.h"
#include"qdebug.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::accept(){
    qDebug()<<"dialog accept";
    bool ok;
    double c=ui->lineEdit->text().toDouble(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
