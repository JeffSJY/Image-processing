#ifndef DIALOG4_H
#define DIALOG4_H

#include <QDialog>

namespace Ui {
class Dialog4;
}

class Dialog4 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog4(QWidget *parent = 0);
    ~Dialog4();

private:
    Ui::Dialog4 *ui;
public slots:
    void accept();
signals:
    void confirmed(int,double,double);
};

#endif // DIALOG4_H
