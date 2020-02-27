#include "dialog6.h"
#include "ui_dialog6.h"

Dialog6::Dialog6(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog6)
{
    ui->setupUi(this);
}

Dialog6::~Dialog6()
{
    delete ui;
}
void Dialog6::accept(){

    bool ok;
    double c=ui->lineEdit->text().toInt(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
