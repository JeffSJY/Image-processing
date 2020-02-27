#include "dialoghdf.h"
#include "ui_dialoghdf.h"

Dialoghdf::Dialoghdf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoghdf)
{
    ui->setupUi(this);
}

Dialoghdf::~Dialoghdf()
{
    delete ui;
}
void Dialoghdf::accept(){

    bool ok;
    double c=ui->lineEdit->text().toInt(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
