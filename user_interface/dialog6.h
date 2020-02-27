#ifndef DIALOG6_H
#define DIALOG6_H

#include <QDialog>

namespace Ui {
class Dialog6;
}

class Dialog6 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog6(QWidget *parent = 0);
    ~Dialog6();

private:
    Ui::Dialog6 *ui;
public slots:
    void accept();
signals:
    void confirmed(int);
};

#endif // DIALOG6_H
