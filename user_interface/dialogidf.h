#ifndef DIALOGIDF_H
#define DIALOGIDF_H

#include <QDialog>

namespace Ui {
class Dialogidf;
}

class Dialogidf : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogidf(QWidget *parent = 0);
    ~Dialogidf();

private:
    Ui::Dialogidf *ui;
public slots:
    void accept();
signals:
    void confirmed(double);
};

#endif // DIALOGIDF_H
