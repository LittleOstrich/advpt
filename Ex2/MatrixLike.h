#pragma once
#include <cstddef> //für size_t
using namespace std;


// forward declarations
template<typename T, size_t r, size_t c>
class Matrix;

template<typename T, size_t r>
using Vector = Matrix<T, r, 1>;

template<typename T, class Derived, size_t nRows, size_t nColumns>
class MatrixLike {
public:
    /// c'tor/ d'tor
    virtual ~MatrixLike ( ) noexcept {}; // virtual destructor

    /// virtual operators
    // virtual methods cannot be templated, i.e. the full matrix muliplication
    // which would require one additional template parameter cannot be abstract
    virtual Vector<T, nRows> operator* (const Vector<T, nColumns> & o) const = 0;

    // TODO: optimize the () operator
    virtual const T & operator() (int r, int c) const = 0;
    virtual T & operator() (int r, int c) = 0;

    /// other functions
    virtual Derived inverseDiagonal( ) const = 0;
    // feel free to extend as required

protected:
};




