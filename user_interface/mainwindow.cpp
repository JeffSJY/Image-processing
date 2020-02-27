#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<imageprocessor.h>
#include<vector>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(NULL),
    pixmapItem(NULL),
     dialog(new Dialog(this)),
     dialog2(new Dialog2(this)),
     dialog3(new Dialog3(this)),
     dialog4(new Dialog4(this)),
     meadiandfilterdialog(new Dialog5(this)),
     expansiondialog(new Dialog6(this)),
     etchingdialog(new Dialog7(this)),
     transformmat(new Dialogmat(this)),
     idfdialog(new Dialogidf(this)),
     hdfdialog(new Dialoghdf(this)),
     filedialog2(new QFileDialog(this)),
    filedialog(new QFileDialog(this))
{
    filedialog->setNameFilter("Image(*.bmp *.jpg *.png)");
    filedialog->setFileMode(QFileDialog::ExistingFile);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete filedialog;
    delete pixmapItem;
    delete scene;
}

void MainWindow::on_button_open_clicked(){
   open();
}

void MainWindow::on_button_reset_clicked()
{
   reset();
}
void MainWindow::on_button_pre_clicked(){
    previous();
}
void MainWindow::on_button_next_clicked(){
    next();
}
void MainWindow::on_actionrgb2grey_triggered(){
    QImage img;
    if(getDisplayedImage(img)){
        //rgb2gray
        QImage res=ImageProcessor::reb2gray(img);
        showImage(res);

    };
}
void MainWindow::on_actionreverse_triggered(){
    QImage img;
    if(getDisplayedImage(img)){
        //rgb2gray
        QImage res=ImageProcessor::reverse(img);
        showImage(res);

    };
}
void MainWindow::on_actionlog_transform_triggered(){
      dialog->exec();
}
void MainWindow::on_Dialog_confirmed(double c){
    QImage img;
    if(getDisplayedImage(img)){
        QImage res=ImageProcessor::logtrans(img,c);
        showImage(res);
    };
}
void MainWindow::on_actionhistgram_equstlison_triggered(){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok)
        showImage(ImageProcessor::histEq(img));
}
void MainWindow::on_actioncolor_hist1_triggered(){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok)
        showImage(ImageProcessor::colorhist1(img));
}
void MainWindow::on_actioncolor_hist2_triggered(){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok)
        showImage(ImageProcessor::colorhist2(img));
}
void MainWindow::on_actionliner_triggered(){
      dialog2->exec();
}
void MainWindow::on_Dialog2_confirmed(QString c){
   qDebug()<<c;
   QImage img;
   int nCol;
   bool ok=getDisplayedImage(img);
   if(ok){
       vector<double> k = parseMatrix1(c,nCol);
       showImage(ImageProcessor::liner(img,k,nCol));
   }
}
vector<double> MainWindow::parseMatrix1(QString txt, int &nCol){

    QStringList rows=txt.replace("\n","").trimmed().split(QRegularExpression("\\s*;"));
    nCol = -1;
    vector<double> ret;
    for(int i=0;i<rows.size();++i){
        bool ok;
        QString row = rows[i];
        if(row.trimmed().isEmpty()) continue;
        if(nCol==-1){
            int temp;
            ok = readoneRow(row, ret,temp);
            if(ok) nCol=temp;
            else return vector<double>();
        }
        else
        {
            int temp;
            ok=readoneRow(row, ret, temp);
            if(ok &&temp==nCol) continue;
            else
            {
                nCol= -1;
                return vector<double>();
            }
        }
    }
    return ret;
}

bool MainWindow::readoneRow(QString row, std::vector<double> &kernal, int& nCol){
    nCol=0;
    QStringList cols = row.trimmed().split(QRegularExpression("\\s*,|\\s+"));
    for(int j =0; j<cols.size();++j)
    {
        bool ok;
        QString col = cols[j];
        if (col.trimmed().isEmpty()) continue;
        double d = col.trimmed().toDouble(&ok);
        if(ok){
            nCol++;
            kernal.push_back(d);
        }
        else{
            return false;
        }

    }
    return true;
}


void MainWindow::on_actiongaussblur_triggered(){
    dialog3->exec();
}
void MainWindow::on_Dialog3_confirmed(int size, double sigma){
    QString size1 = QString::number(size);
    QString sigma1=QString::number(sigma);
    qDebug()<<size1<<"  "<<sigma1;
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::gaussblur(img,size,sigma));
    }
}
void MainWindow::on_actionbilfilter_triggered(){
    dialog4->exec();
}
void MainWindow::on_Dialog4_confirmed(int size, double sigmaColor, double sigmaSpace){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){

        showImage(ImageProcessor::bilfliter(img,size,sigmaColor,sigmaSpace));
    }
}
void MainWindow::on_actionmedianfilter_triggered(){
    meadiandfilterdialog->exec();
}
void MainWindow::on_Dialog5_confirmed(int size){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::medianfilter(img,size));
    }
}
void MainWindow::on_actionexpansion_triggered(){
    expansiondialog->exec();
}
void MainWindow::on_Dialog6_confirmed(int size){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::expansion(img,size));
    }
}
void MainWindow::on_actionetching_triggered(){
    etchingdialog->exec();
}
void MainWindow::on_Dialog7_confirmed(int size){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::etching(img,size));
    }
}
void MainWindow::on_actionfft_triggered(){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        qDebug()<<"triggered";
        showImage(ImageProcessor::ffttoQImage(img));
    }
}
void MainWindow::on_actiontransform_triggered(){
    transformmat->exec();
}
void MainWindow::on_Dialogmat_confirmed(QMatrix mat){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::transform(img,mat));
    }
}
void MainWindow::on_button_save_clicked(){
    QList<QGraphicsItem *> items= ui->graphicsView->scene()->items();
    QGraphicsPixmapItem* item= (QGraphicsPixmapItem*)items.at(0);
    QImage img;
    img=item->pixmap().toImage(); //拿到当前的图像
    QString savePath=filedialog2->getSaveFileName(this,"请选择保存路径...","./",tr("Images (*.png *.bmp *.jpg)"));
    img.save(savePath);
}
void MainWindow::on_actionidf_triggered(){
    idfdialog->exec();
   }
void MainWindow::on_Dialogidf_confirmed(double c){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::IDF(img,c));
    }
}
void MainWindow::on_actionhdf_triggered(){
    hdfdialog->exec();
   }
void MainWindow::on_Dialoghdf_confirmed(double c){
    QImage img;
    bool ok=getDisplayedImage(img);
    if(ok){
        showImage(ImageProcessor::HDF(img,c));
    }

}
void MainWindow::open()
{
    qDebug()<<"test";
    if(filedialog->exec())
    {
        QStringList files=filedialog->selectedFiles();
        QDir dir= filedialog->directory();
        QStringList filters;
        filters<<"*.bmp"<<"*.jpg"<<"*.png";
        fileList = dir.entryInfoList(filters);
        for(int i=0;i<fileList.size();++i){
            if(fileList[i].absoluteFilePath()==files[0])
            {
                currFileIndex=i;
                break;
            }
        }
        qDebug()<<files[0];
        showImage(QImage(fileList[currFileIndex].absoluteFilePath()));

    }
}
void MainWindow::reset(){
    pixmapItem->reset();
    QRectF rect=pixmapItem->boundingRect();  //拿到图片边界
    scene->setSceneRect(0,0,rect.width(),rect.height()); //重置画布
    ui->graphicsView->fitInView(rect,Qt::KeepAspectRatio); //自适应，保持宽高比
}

void MainWindow::showImage(QImage img)
{
    if(pixmapItem)
    {
      delete pixmapItem;
      pixmapItem =NULL;
      }
      if(scene)
      {
        delete scene;
        scene =NULL;
        }


    pixmapItem = new mypixmapitem();
    pixmapItem->setPixmap(QPixmap::fromImage(img));

    scene= new QGraphicsScene(this);
    scene->addItem(pixmapItem);

    ui->graphicsView->setScene(scene);

}

void MainWindow::previous(){
    if(currFileIndex==0)
    {
        currFileIndex=fileList.size();
    }
    showImage(QImage(fileList[--currFileIndex].absoluteFilePath()));
}

void MainWindow::next(){
    if(currFileIndex==fileList.size()-1){
        currFileIndex=-1;
    }
    showImage(QImage(fileList[++currFileIndex].absoluteFilePath()));
}
bool MainWindow::getDisplayedImage(QImage &img) const{
   QList<QGraphicsItem *> items= ui->graphicsView->scene()->items();// return all items
   if(items.size()==0) return false;  // if nothing is opened
    QGraphicsPixmapItem* item= (QGraphicsPixmapItem*)items.at(0);
    img=item->pixmap().toImage();
    return true;

}
