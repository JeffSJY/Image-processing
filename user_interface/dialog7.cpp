#include "dialog7.h"
#include "ui_dialog7.h"

Dialog7::Dialog7(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog7)
{
    ui->setupUi(this);
}

Dialog7::~Dialog7()
{
    delete ui;
}
void Dialog7::accept(){

    bool ok;
    double c=ui->lineEdit->text().toInt(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
