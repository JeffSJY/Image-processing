#include "dialogmat.h"
#include "ui_dialogmat.h"

Dialogmat::Dialogmat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogmat)
{
    ui->setupUi(this);
}

Dialogmat::~Dialogmat()
{
    delete ui;
}
void Dialogmat::accept(){
    bool ok;
    double mat[3][3];
    mat[0][0]=ui->lineEdit->text().toInt(&ok);
    mat[0][1]=ui->lineEdit_2->text().toInt(&ok);
    mat[0][2]=ui->lineEdit_3->text().toInt(&ok);
    mat[1][0]=ui->lineEdit_4->text().toInt(&ok);
    mat[1][1]=ui->lineEdit_5->text().toInt(&ok);
    mat[1][2]=ui->lineEdit_6->text().toInt(&ok);
    mat[2][0]=ui->lineEdit_7->text().toInt(&ok);
    mat[2][1]=ui->lineEdit_8->text().toInt(&ok);
    mat[2][2]=ui->lineEdit_9->text().toInt(&ok);
    QMatrix mat2(mat[0][0],mat[0][1],
                 mat[1][0],mat[1][1],
                 mat[2][0],mat[2][1]);
    if(ok){
        emit confirmed(mat2); //浼犻€?
    }
    close();// close dialog
}
