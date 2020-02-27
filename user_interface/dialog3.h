#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>

namespace Ui {
class Dialog3;
}

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget *parent = 0);
    ~Dialog3();

private:
    Ui::Dialog3 *ui;
public slots:
    void accept();
signals:
    void confirmed(int,double);
};

#endif // DIALOG2_H
