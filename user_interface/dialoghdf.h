#ifndef DIALOGHDF_H
#define DIALOGHDF_H

#include <QDialog>

namespace Ui {
class Dialoghdf;
}

class Dialoghdf : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoghdf(QWidget *parent = 0);
    ~Dialoghdf();

private:
    Ui::Dialoghdf *ui;
public slots:
    void accept();
signals:
    void confirmed(double);
};
#endif // DIALOGHDF_H
