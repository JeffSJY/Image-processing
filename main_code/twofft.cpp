#include "twofft.h"

twofft::twofft()
{

}
size_t twofft::fun(size_t v)  {

    float f = (float)(v - 1);

    return 1 << ((*(unsigned int*)(&f) >> 23) - 126);

}

size_t twofft::calcN(size_t length) {
    size_t num=length;
    length = length&(length-1);
    if(length==0) return num;
    else return fun(num);
}
//vector<complex<double> > twofft::fft(vector<int> data, size_t N)
//{
////    vector<complex<double> > ret(N,0);
////    for(int i =0;i<N;i++){
////        ret[i]=data[i];
////    }
////  // convert data to vector<complex<double> >
////  // call:
////  // vector<complex<double> >
////  return fft(ret,  N);
//}

//vector<complex<double> > twofft::fft(vector<double> data, size_t N)
//{

////    vector<complex<double> > ret(N,0);
////    for(int i =0;i<N;i++){
////        ret[i]=data[i];
////    }

////  return fft(ret,  N);
//}

vector<complex<double> >
twofft::fft(vector<complex<double> > data, size_t N) {
    N=calcN(N);
   vector<complex<double> > a(N,0);
   if(N==1||N==0)  return data;
   else if(N==2)  {
       for(int k=0;k<2;++k){
           complex<double> t(0,0);
           for(int n=0;n<2;++n)
           {
               t+=data[n]*complex<double>(cos(PI*k*n),-sin(PI*k*n));
           }
           a[k]=t;
       }
       return a;
   }
   else if(N>2)
   {
       vector<complex<double> >  b(N,0);
       vector<complex<double> >  c(N,0);
       for(size_t i=0;i<N/2;i++)
       {
           b[i]=data[i*2];
           c[i]=data[i*2+1];      }
       b=fft(b,N/2);
       c=fft(c,N/2);
       for(size_t  i=0;i<N/2;i++)
       {   a[i]=b[i]+c[i]*complex<double>(cos(2*PI*i/N),-sin(2*PI*i/N));
           a[N/2+i]= b[i]-c[i]*complex<double>(cos(2*PI*i/N),-sin(2*PI*i/N));
       }
       return a;
   }


}

vector<complex<double> >
twofft::ifft(vector<complex<double> > data, size_t N) {
    N=calcN(N);
   vector<complex<double> > a(N,0);
  for(int i=0;i<N;i++){
    a[i]= complex<double>(data[i].real(), -data[i].imag());
  }
  vector<complex<double> > b=fft(a,N);
  for(int i =0;i<N;i++){
    b[i] = complex<double>(b[i].real() / N, -b[i].imag() / N);
  }
  return b;
}

Matrix<complex<double> >
twofft::fft2d (const Matrix<int>& data, size_t M, size_t N){
    Matrix<int> mat(data);
    size_t m,n;
    m=mat._nRow; n=mat._nCol;
    M=calcN(mat._nRow);
    N=calcN(mat._nCol);
    Matrix<complex<double> > temp(M,N,0);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){

                temp(i,j) = mat(i,j);

        }
    }
    return fftrow(fftrow(temp).transpose()).transpose();
}

Matrix<complex<double> >
twofft::fft2d (const Matrix<double>& data, size_t m, size_t n){
    size_t M,N;
    if(m==0)
    {M=calcN(data._nRow); N=calcN(data._nCol);}
    else{
        M=calcN(m); N=calcN(n);
    }
    Matrix<complex<double> > temp(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){

                temp(i,j) = data(i,j);

        }
    }
    return fftrow(fftrow(temp).transpose()).transpose();

}


Matrix<complex<double> >
twofft::fftrow(const Matrix<complex<double> >& mat){
    Matrix<complex<double> > ret(mat);
    for(size_t i=0;i<ret._nRow;++i){
        ret.setRow(i,fft(mat.getRow(i),mat._nRow));
    }
    return ret;
}

Matrix<complex<double> >
twofft::ifft2d (const Matrix<complex<double> >&data, size_t m, size_t n){

    size_t M,N;
    if(m==0)
    {M=calcN(data._nRow); N=calcN(data._nCol);}
    else{
        M=calcN(m); N=calcN(n);
    }
    Matrix<complex<double> > temp(M,N,0);
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){

                temp(i,j) = data(i,j);

        }
    }
    return ifftrow(ifftrow(temp).transpose()).transpose();

}

Matrix<complex<double> >
twofft::ifftrow(const Matrix<complex<double> >& mat){
    Matrix<complex<double> > ret(mat);
    for(size_t i=0;i<ret._nRow;++i){
        ret.setRow(i,ifft(mat.getRow(i),mat._nRow));
    }
    return ret;
}
