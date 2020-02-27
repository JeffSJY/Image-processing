#include "dialog5.h"
#include "ui_dialog5.h"

Dialog5::Dialog5(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog5)
{
    ui->setupUi(this);
}

Dialog5::~Dialog5()
{
    delete ui;
}
void Dialog5::accept(){

    bool ok;
    double c=ui->lineEdit->text().toInt(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
