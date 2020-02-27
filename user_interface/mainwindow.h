#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<QFileDialog>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QDialog>
#include<mypixmapitem.h>
#include<dialog.h>
#include<dialog2.h>
#include<vector>
#include<dialog3.h>
#include<dialog4.h>
#include<dialog5.h>
#include<dialog6.h>
#include<dialog7.h>
#include<dialogmat.h>
#include<dialogidf.h>
#include<dialoghdf.h>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFileDialog *filedialog;
    QFileDialog *filedialog2;
    QGraphicsScene *scene;
    mypixmapitem  *pixmapItem;
    QFileInfoList fileList;
    Dialog *dialog;
    Dialog2 *dialog2;
    Dialog3 *dialog3;
    Dialog4 *dialog4;
    Dialog5 *meadiandfilterdialog;
    Dialog6 *expansiondialog;
    Dialog7 *etchingdialog;
    Dialogmat *transformmat;
    Dialogidf *idfdialog;
    Dialoghdf *hdfdialog;
    int currFileIndex;
    void previous();
    void next();
    void open();
    void reset();
    void showImage(QImage);
    bool getDisplayedImage(QImage& ) const; //  use const can not change the factors in the class 不会改这个类里的参数
public slots:
    void on_button_open_clicked();
    void on_button_reset_clicked();
    void on_button_pre_clicked();
    void on_button_next_clicked();
    void on_actionrgb2grey_triggered();
    void on_actionreverse_triggered();
    void on_actionlog_transform_triggered();
    void on_Dialog_confirmed(double);
    void on_actionhistgram_equstlison_triggered();
    void on_actioncolor_hist1_triggered();
    void on_actioncolor_hist2_triggered();
    void on_actionliner_triggered();
    void on_Dialog2_confirmed(QString);
    vector<double> parseMatrix1(QString txt, int& nCol);
    bool readoneRow(QString row, std::vector<double>& kernal, int& nCol);
    void on_actiongaussblur_triggered();
    void on_Dialog3_confirmed(int,double);
    void on_actionbilfilter_triggered();
    void on_Dialog4_confirmed(int,double,double);
    void on_actionmedianfilter_triggered();
    void on_Dialog5_confirmed(int);
    void on_actionexpansion_triggered();
    void on_Dialog6_confirmed(int);
    void on_actionetching_triggered();
    void on_Dialog7_confirmed(int);
    void on_actionfft_triggered();
    void on_actiontransform_triggered();
    void on_Dialogmat_confirmed(QMatrix);
    void on_button_save_clicked();
    void on_actionidf_triggered();
    void on_Dialogidf_confirmed(double);
    void on_actionhdf_triggered();
    void on_Dialoghdf_confirmed(double);
};

#endif // MAINWINDOW_H
