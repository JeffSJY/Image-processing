#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include "matrix.h"
#include<matrix.h>
#include<QImage>
#include<QPixmap>
#include <cstdio>
#include <algorithm>
#include<QtGlobal>
#include<QtCore/qmath.h>
#include"twofft.h"

class ImageProcessor
{
public:
    ImageProcessor();

    static QImage reb2gray(const QImage&); //const 当前图像作为参数，但是不希望它改变，只是作为参数
    static QImage reverse(const QImage&);
    static QImage logtrans(const QImage&, double c);
    static QImage histEq(const QImage&);
    static QImage colorhist1(const QImage&);
    static QImage colorhist2(const QImage&);
    static void cumulateHist(int *hist,int* map,int N);
    static double Hue2RGB(float v1,float v2,float vh);
    static QImage liner(const QImage&, std::vector<double> kernel, int nCol);

    static Matrix<double> zeropadding(Matrix<double>&,int nCol, int nRow);

    static Matrix<int> Filter(Matrix<double>&,std::vector<double> kernel, int nCol, int nRow);

    static Matrix<int> Crop(Matrix<int>&,int nCol,int nRow);

    static std::vector<int> getpatch(Matrix<double>&,int w, int h, int nCol, int nRow);
    static QImage gaussblur(const QImage&,int size, double sigma);
    static std::vector<double> creatgaussKernel(int size,double sigma);
    static void kernelNormalization(std::vector<double>&);
    static QImage bilfliter(const QImage&,int size,double sigmaColor,double sigmaSpace);
    static std::vector<double> creatbilfliterKernel(double sigma);
    static QImage medianfilter(const QImage&,int size);
    static QImage expansion(const QImage&,int size);
    static QImage etching(const QImage&,int size);
    static Matrix<double> fromQImage(const QImage&, char c);
    static QImage ffttoQImage(const QImage&);
    static Matrix<double>  matabs(Matrix<complex<double> >& );
    static QImage transform(const QImage&,const QMatrix&);
    static QImage IDF(const QImage&,double d0);
    static QImage HDF(const QImage&,double d0);
};

#endif // IMAGEPROCESSOR_H
