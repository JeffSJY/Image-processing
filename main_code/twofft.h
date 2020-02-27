#ifndef TWOFFT_H
#define TWOFFT_H
#include "matrix.h"
#include "complex"
#include "complex.h"
using namespace std;
const double PI = 3.1415926;

class twofft
{
public:
    twofft();
    static size_t fun(size_t v) ;
    static size_t calcN(size_t length);

    // different function with different input

//    std::vector<std::complex<double> > fft(std::vector<int> data, size_t N);
//    std::vector<std::complex<double> > fft(std::vector<double> data, size_t N);
    static std::vector<std::complex<double> > fft(std::vector<std::complex<double> > data, size_t N) ;
    static std::vector<std::complex<double> > ifft(std::vector<std::complex<double> > data, size_t N) ;
    static Matrix<complex<double> > fft2d (const Matrix<int>&, size_t, size_t );
    static Matrix<complex<double> > fft2d (const Matrix<double>&, size_t, size_t );
    static Matrix<complex<double> > fftrow(const Matrix<complex<double> >&);
    static Matrix<complex<double> > ifft2d (const Matrix<complex<double> >&, size_t, size_t );
    static Matrix<complex<double> > ifftrow(const Matrix<complex<double> >&);
};

#endif // TWOFFT_H
