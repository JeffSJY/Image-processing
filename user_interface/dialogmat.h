#ifndef DIALOGMAT_H
#define DIALOGMAT_H

#include <QDialog>
#include <QMatrix>
namespace Ui {
class Dialogmat;
}

class Dialogmat : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogmat(QWidget *parent = 0);
    ~Dialogmat();

private:
    Ui::Dialogmat *ui;
public slots:
    void accept();
signals:
    void confirmed(QMatrix);
};

#endif // DIALOGMAT_H
