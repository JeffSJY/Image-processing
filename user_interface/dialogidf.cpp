#include "dialogidf.h"
#include "ui_dialogidf.h"

Dialogidf::Dialogidf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogidf)
{
    ui->setupUi(this);
}

Dialogidf::~Dialogidf()
{
    delete ui;
}
void Dialogidf::accept(){

    bool ok;
    double c=ui->lineEdit->text().toInt(&ok); //拿到参数c
    if(ok){
        emit confirmed(c); //传递
    }
    close();// close dialog
}
