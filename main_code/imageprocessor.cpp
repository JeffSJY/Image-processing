#include "imageprocessor.h"
#include "matrix.h"
#include<QPixmap>
#include<QImage>
#include<cmath>
#include <cstdio>
#include <algorithm>
#include<QtGlobal>
#include<QtCore>
#include<QtCore/QVariantComparisonHelper>
#include<QtCore/QContiguousCacheTypedData>
#include<QString>
#include"twofft.h"
ImageProcessor::ImageProcessor()
{

}
QImage ImageProcessor::reb2gray(const QImage &img){
    QImage ret(img);
    int w=ret.width(); //图片长度
    int h=ret.height();//图片宽度
    Matrix<double> gray = fromQImage(img,'y');
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){

            int y=int(gray(j,i));
            ret.setPixel(i,j,qRgb(y,y,y));//存入新的图像的对应点
        }
    }
    return ret;//返回处理后图像
}
QImage ImageProcessor::reverse(const QImage &img){
    QImage ret(img);
    int w=ret.width();
    int h=ret.height();
    Matrix<double> red = fromQImage(img,'r');
    Matrix<double> green = fromQImage(img,'g');
    Matrix<double> blue = fromQImage(img,'b');
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){
            int r=255-int(red(j,i));
            int g=255-int(green(j,i));
            int b=255-int(blue(j,i));    //翻转后颜色信息
            ret.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return ret;
}
QImage ImageProcessor::logtrans(const QImage &img, double c){
    QImage ret(img);
    int w=ret.width();
    int h=ret.height();
    Matrix<double> gray = fromQImage(img,'y');
    for(int i=0;i<w;i++){
        for(int j=0;j<h;j++){

            double r=gray(j,i)/255.0;    //得到r
            int res=c*std::log(1+r)*255; //转换后的结果
            if(res>255) res=255; //如果结果大于255用255替代
            ret.setPixel(i,j,qRgb(res,res,res));
        }
    }
    return ret;

}
QImage ImageProcessor::histEq(const QImage &img){
    QImage ret(img);
    int w=ret.width();
    int h=ret.height();
    Matrix<double> gray = fromQImage(img,'y');
    long N=w*h;
    // count histogram
    int hist[256]={0},map[256];
    for(int i=0;i<w;++i){
        for(int j=0;j<h;++j){
            hist[int(gray(j,i))]++;
        }
    }

    //calculate map function
    cumulateHist(hist,map,N);
    //map the pixel
    for(int i=0;i<w; ++i){
        for(int j=0;j<h;++j){
            int g=map[int(gray(j,i))];
            ret.setPixel(i,j,qRgb(g,g,g));
        }
    }
    return ret;
}
QImage ImageProcessor::colorhist1(const QImage &img){
    QImage ret(img);
    int w=ret.width();
    int h=ret.height();
    long N=w*h;
    Matrix<double> red = fromQImage(img,'r');
    Matrix<double> green = fromQImage(img,'g');
    Matrix<double> blue = fromQImage(img,'b');
    // count histogram
    int Rhist[256]={0},Ghist[256]={0},Bhist[256]={0},Rmap[256],Gmap[256],Bmap[256];
    for(int i=0;i<w;++i){
        for(int j=0;j<h;++j){
            Rhist[int(red(j,i))]++;
            Ghist[int(green(j,i))]++;
            Bhist[int(blue(j,i))]++;
        }
    }
    //calculate map function
    cumulateHist(Rhist,Rmap,N);
    cumulateHist(Ghist,Gmap,N);
    cumulateHist(Bhist,Bmap,N);
    //map the pixel
    for(int i=0;i<w; ++i){
        for(int j=0;j<h;++j){
            int r=Rmap[int(red(j,i))];
            int g=Gmap[int(green(j,i))];
            int b=Bmap[int(blue(j,i))];
            ret.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return ret;
}
QImage ImageProcessor::colorhist2(const QImage &img){
    QImage ret(img);
    int w=ret.width();
    int h=ret.height();
    long N=w*h;
    int Ihist[256]={0},IMap[256];
    float H[w][h],S[w][h],L[w][h];
    Matrix<double> red = fromQImage(ret,'r');
    Matrix<double> green = fromQImage(ret,'g');
    Matrix<double> blue = fromQImage(ret,'b');
    for(int i=0;i<w;++i){
        for(int j=0;j<h;++j){
           float var_R=(int(red(j,i)))/255.0;
           float var_G=(int(green(j,i)))/255.0;
           float var_B=(int(blue(j,i)))/255.0;
           float var_Min=qMin(qMin(var_B,var_G),var_R);
           float var_Max=qMax(qMax(var_R,var_B),var_G);
           float del=var_Max-var_Min;
           L[i][j]=(var_Max+var_Min)/2.0;
           int i1=int(L[i][j]*255);
           Ihist[i1]++;
           if(del==0){
               H[i][j]=0;S[i][j]=0;
           }
           else{
               if(L[i][j]<0.5) S[i][j]=del/(var_Max+var_Min);
               else S[i][j]=del/(2.0-var_Max-var_Min);
               float del_R=(((var_Max-var_R)/6.0)+(del/2.0))/del;
               float del_G=(((var_Max-var_G)/6.0)+(del/2.0))/del;
               float del_B=(((var_Max-var_B)/6.0)+(del/2.0))/del;
               if(var_R==var_Max) H[i][j]=del_B-del_G;
               else if(var_G==var_Max) H[i][j]=(0.333)+del_R-del_B;
               else if(var_B==var_Max) H[i][j]=(0.667)+del_G-del_R;
               if(H[i][j]<0) H[i][j]+=1.0;
               if(H[i][j]>1.0) H[i][j]-=1.0;
           }
        }
    }

    cumulateHist(Ihist,IMap,N);
    for(int i=0;i<w;++i){
        for(int j=0;j<h;++j){
            int r,g,b,m;
            float l,var_2,var_1;
            int i1=int(L[i][j]*255);
            m=IMap[i1];
            l=m/255.0;
            if(S[i][j]==0){
                r=m;g=m;b=m;
            }
            else{
                if(l<0.5) var_2=l*(1.0+S[i][j]);
                else var_2=(l+S[i][j]-(S[i][j]*l));
                var_1=2.0*l-var_2;

                r=255*Hue2RGB(var_1,var_2,H[i][j]+(0.333));
                g=255*Hue2RGB(var_1,var_2,H[i][j]);
                b=255*Hue2RGB(var_1,var_2,H[i][j]-(0.333));
            }
            ret.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return ret;
}
double ImageProcessor::Hue2RGB(float v1, float v2, float vh){
       if(vh<0) vh+=1.0;
       if(vh>1.0) vh-=1.0;
       if((6.0*vh)<1.0) return(v1+(v2-v1)*6.0*vh);
       if((2.0*vh)<1.0) return(v2);
       if((3.0*vh)<2.0) return(v1+(v2-v1)*((0.667)-vh)*6.0);
       else return(v1);
}
void ImageProcessor::cumulateHist(int *hist,int* map,int N){
    double sum=0;
    for(int i=0;i<256;++i){
        sum+=hist[i];
        map[i]=round(sum/N*255);
    }
}
QImage ImageProcessor::liner(const QImage &img, std::vector<double> kernel, int nCol){
    int nRow=kernel.size()/nCol;
    if(0==nRow%2||0==nCol%2) return img;
    //padding
    Matrix<double> red = fromQImage(img,'r');
    Matrix<double> green = fromQImage(img,'g');
    Matrix<double> blue = fromQImage(img,'b');
    Matrix<double> padded_red=zeropadding(red,nCol,nRow);
    Matrix<double> padded_green=zeropadding(green,nCol,nRow);
    Matrix<double> padded_blue=zeropadding(blue,nCol,nRow);
    Matrix<int> filtered_red=Filter(padded_red,kernel,nCol,nRow);
    Matrix<int> filtered_green=Filter(padded_green,kernel,nCol,nRow);
    Matrix<int> filtered_blue=Filter(padded_blue,kernel,nCol,nRow);
    Matrix<int> croped_red=Crop(filtered_red,nCol,nRow);
    Matrix<int> croped_green=Crop(filtered_green,nCol,nRow);
    Matrix<int> croped_blue=Crop(filtered_blue,nCol,nRow);
    QImage ret(croped_red._nCol,croped_red._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(croped_red(j,i),croped_green(j,i),croped_blue(j,i)));
        }
    }
//    QImage padded=zeroPadding(img,nCol,nRow);
//    QImage filtered= filter(padded,kernel,nCol,nRow);
//   QImage croped=crop(filtered,nCol,nRow);
    return ret;
}
Matrix<double> ImageProcessor::zeropadding(Matrix<double> &mat, int nCol, int nRow){
    Matrix<double> ret(mat._nRow+nRow-1,mat._nCol+nCol-1,0);
    int width= ret._nCol;
    int height = ret._nRow;
    for(int i =0;i<height;++i){
        for(int j = 0;j<width;++j){
            if(j<nCol/2||j>=width-nCol/2||i<nRow/2||i>=height-nRow/2)
                ret(i,j)=0;
            else
                ret(i,j)=mat(i-nRow/2,j-nCol/2);
        }
    }
    return ret;
}
Matrix<int> ImageProcessor::Filter(Matrix<double> &mat, std::vector<double> kernel, int nCol, int nRow){
    Matrix<int> ret(mat._nRow,mat._nCol,0);
    int width=mat._nCol;
    int height=mat._nRow;
    for(int i = nCol/2;i<width-nCol/2;i++){
        for(int j = nRow/2;j<height-nRow/2;++j){
            std::vector<int> patchR = getpatch(mat,i,j,nCol,nRow);
            double sum=0;
            for(int k=0;k<kernel.size();++k){
                sum+=kernel[k]*patchR[k];
                }
            int a=(int)sum;
            if(a>255) a=255;
            if(a<0) a=0;
            ret(j,i)=a;
        }
    }
    return ret;
}
std::vector<int> ImageProcessor::getpatch(Matrix<double> &mat, int w, int h, int nCol, int nRow){
    std::vector<int> ret;
    for(int i = w-nCol/2;i<=w+nCol/2;++i){
        for(int j = h-nRow/2;j<=h+nRow/2;++j){
            ret.push_back(mat(j,i));
        }
    }
    return ret;
}
Matrix<int> ImageProcessor::Crop(Matrix<int> &mat, int nCol, int nRow){
    Matrix<int> ret(mat._nRow-nRow+1,mat._nCol-nCol+1,0);
    int width=ret._nCol;
    int height=ret._nRow;
    for(int i=0;i<width;i++){
        for(int j = 0 ;j<height;j++){
            ret(j,i)=mat(j+nRow/2,i+nCol/2);
        }
    }
    return ret;
}
QImage ImageProcessor::gaussblur(const QImage &img, int size, double sigma){
    std::vector<double> kernel = creatgaussKernel(size,sigma);
    kernelNormalization(kernel);
    Matrix<double> red = fromQImage(img,'r');
    Matrix<double> green = fromQImage(img,'g');
    Matrix<double> blue = fromQImage(img,'b');
    Matrix<int> filtered_red=Filter(red,kernel,size,size);
    Matrix<int> filtered_green=Filter(green,kernel,size,size);
    Matrix<int> filtered_blue=Filter(blue,kernel,size,size);
    QImage ret(filtered_red._nCol,filtered_red._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(filtered_red(j,i),filtered_green(j,i),filtered_blue(j,i)));
        }
    }
    return ret;
}
std::vector<double> ImageProcessor::creatgaussKernel(int size, double sigma){
    std::vector<double> ret;
    for(int i=0;i<size;++i){
        for(int j = 0;j<size;++j){
            int deltaX=abs(i-size/2);
            int deltaY=abs(j-size/2);
            ret.push_back(exp((-deltaX*deltaX-deltaY*deltaY)/(2*sigma*sigma)));
        }
    }
    return ret;
}
QImage ImageProcessor::bilfliter(const QImage &img, int size, double sigmaColor, double sigmaSpace){
    Matrix<double> gray = fromQImage(img,'y');
    Matrix<double> padded_gray=zeropadding(gray,size,size);
    int width=padded_gray._nCol;
    int height=padded_gray._nRow;
    Matrix<int> filtered_gray(height,width,0);
    std::vector<double> kernelSpace=creatgaussKernel(size,sigmaSpace);
    std::vector<double> kernelColor=creatbilfliterKernel(sigmaColor);
    for(int i = size/2;i<width-size/2;i++){
        for(int j=size/2;j<height-size/2;j++){
            std::vector<int> patch=getpatch(padded_gray,i,j,size,size);
            std::vector<double> kernel;
            for(int k =0;k<kernelSpace.size();k++){
                int delta = abs(patch[k]-int(padded_gray(j,i)));
                kernel.push_back(kernelColor[delta]*kernelSpace[k]);
            }
            kernelNormalization(kernel);
            double sum=0;
            for(int k =0;k<kernelSpace.size();k++){
                sum +=kernel[k]*patch[k];
            }
            int y=(int)sum;
            if(y>255) y=255;
            if(y<0)  y=0;
            filtered_gray(j,i)=y;
        }
    }
    Matrix<int> croped_gray=Crop(filtered_gray,size,size);
    QImage ret(croped_gray._nCol,croped_gray._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(croped_gray(j,i),croped_gray(j,i),croped_gray(j,i)));
        }
    }
    return ret;
}
std::vector<double> ImageProcessor::creatbilfliterKernel(double sigma){
    std::vector<double> ret;
    for(int i=0;i<256;i++)
    ret.push_back(exp((-i*i)/(2*sigma*sigma)));
    return ret;
}
void ImageProcessor::kernelNormalization(std::vector<double> &kernel){
    double sum = 0;
    for(int i=0;i<kernel.size();i++){
        sum+=kernel[i];
    }
    if(0!=sum){
        for(int i =0;i<kernel.size();i++)
            kernel[i]/=sum;
    }
}
QImage ImageProcessor::medianfilter(const QImage &img, int size){
    Matrix<double> red = fromQImage(img,'r');
    Matrix<double> green = fromQImage(img,'g');
    Matrix<double> blue = fromQImage(img,'b');
    Matrix<double> padded_red=zeropadding(red,size,size);
    Matrix<double> padded_green=zeropadding(green,size,size);
    Matrix<double> padded_blue=zeropadding(blue,size,size);
    int width=padded_red._nCol;
    int height=padded_red._nRow;
    Matrix<int> filtered_red(height,width,0);
    Matrix<int> filtered_green(height,width,0);
    Matrix<int> filtered_blue(height,width,0);
    for(int i = size/2;i<width-size/2;i++){
        for(int j = size/2;j<height-size/2;++j){
            std::vector<int> patchR = getpatch(padded_red,i,j,size,size);
            std::vector<int> patchG = getpatch(padded_green,i,j,size,size);
            std::vector<int> patchB = getpatch(padded_blue,i,j,size,size);
            std::sort(patchR.begin(),patchR.end());
            std::sort(patchG.begin(),patchG.end());
            std::sort(patchB.begin(),patchB.end());
            int r=patchR.at(patchR.size()/2);
            int g=patchG.at(patchG.size()/2);
            int b=patchB.at(patchB.size()/2);
            filtered_red(j,i)=r;
            filtered_green(j,i)=g;
            filtered_blue(j,i)=b;
        }
    }
    Matrix<int> croped_red=Crop(filtered_red,size,size);
    Matrix<int> croped_green=Crop(filtered_green,size,size);
    Matrix<int> croped_blue=Crop(filtered_blue,size,size);
    QImage ret(croped_red._nCol,croped_red._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(croped_red(j,i),croped_green(j,i),croped_blue(j,i)));
        }
    }
    return ret;
}
QImage ImageProcessor::expansion(const QImage &img, int size){
    Matrix<double> gray = fromQImage(img,'y');
    Matrix<double> padded_gray=zeropadding(gray,size,size);
    int width=padded_gray._nCol;
    int height=padded_gray._nRow;
    Matrix<int> filtered_gray(height,width,0);
    for(int i = size/2;i<width-size/2;i++){
        for(int j = size/2;j<height-size/2;++j){
            std::vector<int> patchGrey = getpatch(padded_gray,i,j,size,size);
            int y=*std::max_element(patchGrey.begin(),patchGrey.end());
            filtered_gray(j,i)=y;
        }
    }
    Matrix<int> croped_gray=Crop(filtered_gray,size,size);
    QImage ret(croped_gray._nCol,croped_gray._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(croped_gray(j,i),croped_gray(j,i),croped_gray(j,i)));
        }
    }
    return ret;
}
QImage ImageProcessor::etching(const QImage &img, int size){
    Matrix<double> gray = fromQImage(img,'y');
    Matrix<double> padded_gray=zeropadding(gray,size,size);
    int width=padded_gray._nCol;
    int height=padded_gray._nRow;
    Matrix<int> filtered_gray(height,width,0);
    for(int i = size/2;i<width-size/2;i++){
        for(int j = size/2;j<height-size/2;++j){
            std::vector<int> patchGrey = getpatch(padded_gray,i,j,size,size);
            int y=*std::min_element(patchGrey.begin(),patchGrey.end());
            filtered_gray(j,i)=y;
        }
    }
    Matrix<int> croped_gray=Crop(filtered_gray,size,size);
    QImage ret(croped_gray._nCol,croped_gray._nRow,img.format());
    for(int i=0;i<ret.width();i++){
        for(int j=0;j<ret.height();j++){
            ret.setPixel(i,j,qRgb(croped_gray(j,i),croped_gray(j,i),croped_gray(j,i)));
        }
    }
    return ret;
}
Matrix<double> ImageProcessor::fromQImage(const QImage& img, char c){
    int width=img.width();
    int height=img.height();
    Matrix<double> mat(height,width,0);
    switch (c) {
    case 'r':
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                mat(j,i)=qRed(img.pixel(i,j));
            }
        }
        return mat;
        break;
    case 'g':
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                mat(j,i)=qGreen(img.pixel(i,j));
            }
        }
        return mat;
        break;
    case 'b':
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                mat(j,i)=qBlue(img.pixel(i,j));
            }
        }
        return mat;
        break;
    case'y':
        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){
                QRgb rgb=img.pixel(i,j); //一个像素的颜色信息
                double r=qGray(rgb);
                mat(j,i)=r;
            }
        }
        return mat;
        break;
    default:
        break;
    }

}
Matrix<double> ImageProcessor::matabs(Matrix<complex<double> > &mat){
    int M=mat._nCol; int N=mat._nRow;
    Matrix<double> mat2(N,M,0);
    double max=0,min=99;
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            mat2(i,j)=qLn(1+abs(mat(i,j))); //取模，对数变换
            if (max < mat2(i,j)) max = mat2(i,j);
            if( min > mat2(i,j)) min = mat2(i,j);
        }
    }
//    mat2=mat2.fftshift(); //fftshift
    double sub=max-min;
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            mat2(i,j)=(mat2(i,j)-min)/sub;
        }
    }

    return mat2;
}

QImage ImageProcessor::ffttoQImage(const QImage& img){

//    Matrix<complex<double> > red=twofft::fft2d(fromQImage(img,'r'),0,0);
//    Matrix<complex<double> > green=twofft::fft2d(fromQImage(img,'g'),0,0);
//    Matrix<complex<double> > blue=twofft::fft2d(fromQImage(img,'b'),0,0);
    Matrix<complex<double> > gray=twofft::fft2d(fromQImage(img,'y'),0,0);
//    Matrix<double> r=matabs(red);
//    Matrix<double> g=matabs(green);
//    Matrix<double> b=matabs(blue);
     Matrix<double> y=matabs(gray);
    int M=y._nCol;
    int N=y._nRow;
    Matrix<double> y_fftshift(2*N,2*M,1);
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++)
            y_fftshift(i,j)=y(i,j);
    }
    for(int i=0;i<N;i++){
        for(int j=M;j<2*M;j++)
            y_fftshift(i,j)=y(i,j-M);
    }
    for(int i=N;i<2*N;i++){
        for(int j=0;j<M;j++)
            y_fftshift(i,j)=y(i-N,j);
    }
    for(int i=N;i<2*N;i++){
        for(int j=M;j<2*M;j++)
            y_fftshift(i,j)=y(i-N,j-M);
    }
    QImage ret(M,N,img.format());
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++){
            int n=int(y_fftshift(j+N/2,i+M/2)*255);
            if(n>255) n=255;
            if(n<0) n=0;
            ret.setPixel(i,j,qRgb(n,n,n));
        }
    }
    return ret;
}
QImage ImageProcessor::IDF(const QImage &img,double d0){
    Matrix<double> gray=fromQImage(img,'y');
    int M=gray._nRow;int N=gray._nCol;
    int P=M*2;int Q=N*2;
    Matrix<double> fc(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<M;j++){
            fc(i,j)=gray(i,j)*pow((-1),(i+j));
        }
    }
    Matrix<complex<double> > H(P,Q,0);
    Matrix<complex<double> > F=twofft::fft2d(fc,P,Q);
    for(int i=(-1*P)/2;i<(P/2)-1;i++){
        for(int j=(-1*Q)/2;j<(Q/2)-1;j++){
            double d=pow((pow(i,2)+pow(j,2)),(0.5));
            if(d<=d0) H(i+(P/2)+1,j+(Q/2)+1)=1;
        }
    }
    for(int i=0;i<P;i++){
        for(int j=0;j<Q;j++){
            F(i,j)=F(i,j)*H(i,j);
        }
    }
    Matrix<complex<double> >iF=twofft::ifft2d(F,0,0);
    Matrix<double> g(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            g(i,j)=real(iF(i,j));
            g(i,j)=g(i,j)*pow((-1),(i+j));
        }
    }
    QImage ret(N,M,img.format());
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++){
            int n=int(g(i,j));
            if(n>255) n=255;
            ret.setPixel(j,i,qRgb(n,n,n));
        }
    }
    return ret;
}
QImage ImageProcessor::HDF(const QImage &img,double d0){
    Matrix<double> gray=fromQImage(img,'y');
    int M=gray._nRow;int N=gray._nCol;
    int P=M*2;int Q=N*2;
    Matrix<double> fc(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<M;j++){
            fc(i,j)=gray(i,j)*pow((-1),(i+j));
        }
    }
    Matrix<complex<double> > H(P,Q,1);
    Matrix<complex<double> > F=twofft::fft2d(fc,P,Q);
    for(int i=(-1*P)/2;i<(P/2)-1;i++){
        for(int j=(-1*Q)/2;j<(Q/2)-1;j++){
            double d=pow((pow(i,2)+pow(j,2)),(0.5));
            if(d<=d0) H(i+(P/2)+1,j+(Q/2)+1)=0;
        }
    }
    for(int i=0;i<P;i++){
        for(int j=0;j<Q;j++){
            F(i,j)=F(i,j)*H(i,j);
        }
    }
    Matrix<complex<double> >iF=twofft::ifft2d(F,0,0);
    double max=0,min=99;
    Matrix<double> g(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            g(i,j)=real(iF(i,j));
            g(i,j)=g(i,j)*pow((-1),(i+j));
            if (max < g(i,j)) max = g(i,j);
            if( min > g(i,j)) min = g(i,j);
        }
    }
    double sub=max-min;
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            g(i,j)=(g(i,j)-min)/sub;
        }
    }
    QImage ret(N,M,img.format());
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++){
            int n=int(g(i,j)*255);
            if(n>255) n=255;
            if(n<0) n=0;
            ret.setPixel(j,i,qRgb(n,n,n));
        }
    }
    return ret;
}
QImage ImageProcessor::transform(const QImage &img, const QMatrix &mat){
    int width=img.width();
    int height=img.height();
    bool a=mat.isInvertible();
    if(a){
        QPoint point1(0,0),point2(0,height-1),
               point3(width-1,0),point4(width-1,height-1);
        QPoint n_point1,n_point2,n_point3,n_point4;
        n_point1=mat.map(point1);
        n_point2=mat.map(point2);
        n_point3=mat.map(point3);
        n_point4=mat.map(point4);
     double min_x,min_y,max_x,max_y;
     min_x=qMin(qMin(qMin(n_point1.x(),n_point2.x()),n_point3.x()),n_point4.x());
     min_y=qMin(qMin(qMin(n_point1.y(),n_point2.y()),n_point3.y()),n_point4.y());
     max_x=qMax(qMax(qMax(n_point1.x(),n_point2.x()),n_point3.x()),n_point4.x());
     max_y=qMax(qMax(qMax(n_point1.y(),n_point2.y()),n_point3.y()),n_point4.y());
     int new_width=(int)max_x-min_x;
     int new_height=(int)max_y-min_y;
     QImage ret(new_width+1,new_height+1,img.format());  //new image

     for(int i=min_x;i<=max_x;i++){
         for(int j=min_y;j<=max_y;j++){
             QPoint ret_point(i,j);
             QPoint img_point;
             img_point=mat.inverted().map(ret_point);
             QRgb rgb;
             if(img_point.x()>=0&&img_point.x()<width&&img_point.y()>=0&&img_point.y()<height){
             if(ceil(img_point.x())==img_point.x()&&ceil(img_point.y())==img_point.y())
             {

                 rgb=img.pixel(img_point.x(),img_point.y());
             }
             else if(ceil(img_point.x())==img_point.x()&&ceil(img_point.y())!=img_point.y())
             {

                 rgb=(ceil(img_point.y())-img_point.y())*img.pixel(img_point.x(),floor(img_point.y()))
                         +(img_point.y()-floor(img_point.y()))*img.pixel(img_point.x(),ceil(img_point.y()));


             }
             else if(ceil(img_point.x())!=img_point.x()&&ceil(img_point.y())==img_point.y())
             {
                 rgb=(ceil(img_point.x())-img_point.x())*img.pixel(floor(img_point.x()),img_point.y())
                         +(img_point.x()-floor(img_point.x()))*img.pixel(ceil(img_point.x()),img_point.y());

             }
             else if(ceil(img_point.x())!=img_point.x()&&ceil(img_point.y())!=img_point.y())
             {
                 QRgb rgb1,rgb2;
                 rgb1=(ceil(img_point.x())-img_point.x())*img.pixel(floor(img_point.x()),floor(img_point.y()))
                         +(img_point.x()-floor(img_point.x()))*img.pixel(ceil(img_point.x()),floor(img_point.y()));
                 rgb2=(ceil(img_point.x())-img_point.x())*img.pixel(floor(img_point.x()),ceil(img_point.y()))
                         +(img_point.x()-floor(img_point.x()))*img.pixel(ceil(img_point.x()),ceil(img_point.y()));
                 rgb=(ceil(img_point.y())-img_point.y())*rgb1
                         +(img_point.y()-floor(img_point.y()))*rgb2;

             }
             }
             else {
                 rgb=(0,0,0);
             }
             ret.setPixel(i-min_x,j-min_y,rgb);
         }
     }
     return ret;
    }
}
