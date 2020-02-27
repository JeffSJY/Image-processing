#ifndef DIALOG5_H
#define DIALOG5_H

#include <QDialog>

namespace Ui {
class Dialog5;
}

class Dialog5 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog5(QWidget *parent = 0);
    ~Dialog5();

private:
    Ui::Dialog5 *ui;
public slots:
    void accept();
signals:
    void confirmed(int);
};

#endif // DIALOG5_H
