#ifndef MATRIX__H
#define MATRIX__H
#include <QImage>
#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Matrix
{
private:
    // point to the data
    T** _data;
    // number of row and column

    // flag to indicate row-first or column-first
    int _t,startRow,startCol;
    bool checkRange(size_t r, size_t c) const;
    void swap(Matrix<T>& mat);

public:
    size_t _nRow, _nCol;
    // construct a r-row and c-column matrix with all init element
    Matrix(size_t r, size_t c, T init);
    // copy constructor
    Matrix(const Matrix<T>& mat);
    Matrix& operator=(Matrix<T> mat);
    ~Matrix();
    // get element at r-row and c-column
    // return reference to make it modifiable
    T& operator()(size_t r, size_t c) const;
    // output the matrix
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Matrix<U>& rhs); //独立
    size_t getNRow() const;
    size_t getNCol() const;
    std::vector<T> getRow(size_t r) const;
    std::vector<T> getCol(size_t c) const;
    bool setRow(size_t r, const std::vector<T>& d);
    bool setCol(size_t c, const std::vector<T>& d);
    // transpose it in-place
    Matrix<T>& transpose();
    Matrix<T>& fftshift();
    void rowCyclicShift(int r);
    void colCyclicShift(int c);
};

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T init):
_nRow(r), _nCol(c), _t(0),startRow(0),startCol(0)
{
   _data = new T*[r];
    for(size_t i =0;i<r;i++){
        _data[i] = new T[c];
        for(size_t j=0;j<c;j++){
            _data[i][j]=init;
        }
    }
    // new _data
    // new _data[i] within loop
}


template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mat):
_nRow(mat._nRow), _nCol(mat._nCol), _t(mat._t),startCol(mat.startCol),startRow(mat.startRow)
{
    if(_t==0){
        _data = new T*[_nRow];
         for(size_t i =0;i<_nRow;i++){
             _data[i] = new T[_nCol];
             for(size_t j=0;j<_nCol;j++){
                 _data[i][j]=mat._data[i][j];
             }
         }
    }
    else{
        _data = new T*[_nCol];
         for(size_t i =0;i<_nCol;i++){
             _data[i] = new T[_nRow];
             for(size_t j=0;j<_nRow;j++){
                 _data[i][j]=mat._data[i][j];
             }
         }
    }

}

template<typename T>
void Matrix<T>::swap(Matrix<T>& mat) {
    std::swap(_data, mat._data);
    std::swap(_nRow, mat._nRow);
    std::swap(_nCol, mat._nCol);
    std::swap(_t, mat._t);
}



template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> mat)
{
    swap(mat);
    return *this;
}

template<typename T>
size_t Matrix<T>::getNRow() const
{
    return _nRow;
    // return the number of row
}

template<typename T>
size_t Matrix<T>::getNCol() const
{
    return _nCol;
    // return the number of column
}

template<typename T>
bool Matrix<T>::checkRange(size_t r, size_t c) const
{
    if(r>_nRow||c>_nCol||r<=0||c<=0)
        return false;
    else {
       return true;
    }
    // check if r-row and c-column is out of range
}




template<typename T>
T& Matrix<T>::operator()(size_t r, size_t c) const
{
    if(_t==0) return _data[(r+startRow)%_nRow][(c+startCol)%_nCol];
    else return _data[(c+startCol)%_nCol][(r+startRow)%_nRow];
    // get element in (r,c)
    // remember check first
    // different of _t has different order
}

//template<typename T>
//Matrix<T> Matrix<T>::subMatrix(size_t rs, size_t re, size_t cs, size_t ce) const
//{

//    // note the validity of value:rs, re, cs, ce
//    // try to use "except" to broadcast the exception
//}


template<typename T>
std::vector<T> Matrix<T>::getRow(size_t r) const
{
    std::vector<T> T1;
    if(r<_nRow){
        for(size_t j=0;j<_nCol;j++){
            if(_t==0) {
                T1.push_back(_data[r][j]);
            }
            else T1.push_back(_data[j][r]);

        }
        return T1;
    }

    // get all element in r-row and push it back in a vector
    // remember check range first
}


template<typename T>
std::vector<T> Matrix<T>::getCol(size_t c) const
{
    std::vector<T> T2;
    if(c<_nCol)
    {for(size_t i=0;i<_nRow;i++){
            if(_t==0) {
                T2.push_back(_data[i][c]);
            }
            else T2.push_back(_data[c][i]);

    }
    return T2;}
    // get all element in c-column and push it back in a vector
    // remember check range first
}
template<typename T>
bool Matrix<T>::setRow(size_t r, const std::vector<T>& d)
{
    for(size_t j=0;j<_nCol;j++){
        if(_t==0) {
            _data[r][j]=d[j];
        }
        else _data[j][r]=d[j];

    }
    return true;
}

template<typename T>
bool Matrix<T>::setCol(size_t c, const std::vector<T>& d)
{
    for(size_t i=0;i<_nRow;i++){
        if(_t==0) {
            _data[i][c]=d[i];
        }
        else _data[c][i]=d[i];
//        _data[i][c]=d[i];
    }
    return true;
    // set the element of the c-column
    // remember check range first
}

template<typename T>
Matrix<T>::~Matrix()
{
    if(_t==0){
        for(size_t i=0;i<_nRow;i++){
               delete[] _data[i];
        }
    }
    else{
        for(size_t i=0;i<_nCol;i++){
               delete[] _data[i];
        }
    }


    delete[] _data;
     // delete _data[i] first
    // then delete _data
    // note the value of _t
}

template<typename T>
Matrix<T>& Matrix<T>::transpose()
{
   _t=1-_t;
   size_t temp;
   temp=_nCol;
   _nCol=_nRow;
   _nRow=temp;
   return *this;
    // change _t
    // swap _nRow and _nCol
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& rhs)
{
    for(size_t i=0; i<rhs._nRow; ++i) {
            for(size_t j=0; j<rhs._nCol; ++j) {
                out << rhs(i,j) << "\t";
            }
            out <<std::endl;
        }
        return out;
}
template<typename T>
Matrix<T>& Matrix<T>::fftshift(){
    Matrix ret(*this);
    ret.rowCyclicShift(ret._nRow/2);
    ret.colCyclicShift(ret._nCol/2);
    return ret;
    }
template<typename T>
void Matrix<T>::rowCyclicShift(int r){
    startRow-=(r%_nRow);
    if(startRow<0) startRow+=_nRow;
}
template<typename T>
void Matrix<T>::colCyclicShift(int c){
    startCol-=(c%_nCol);
    if(startCol<0) startCol+=_nCol;
}

#endif
