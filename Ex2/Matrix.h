#pragma once
#include "MatrixLike.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <typename T, size_t r, size_t c>
class Matrix : public MatrixLike<T, Matrix<T, r, c>, r, c> {

public:
  size_t row = 0;
  size_t col = 0;
  T *data;

  Matrix<T, r, c>() : row(r), col(c) {
    data = new T[row * col];
    for (size_t i = 0; i < row * col; i++) {
      data[i] = T();
    }
  }

  Matrix<T, r, c>(T t) : row(r), col(c) {
    data = new T[row * col];
    for (size_t i = 0; i < row * col; i++)
      data[i] = t;
  }

  Matrix<T, r, c>(Matrix<T, r, c> &other) : row(r), col(c) {
    data = new T[row * col];
    for (int i = 0; i < row * col; i++) {
      data[i] = other.data[i];
    }
  }

  Matrix<T, r, c>(const Matrix<T, r, c> &other) : row(r), col(c) {
    data = new T[row * col];
    for (int i = 0; i < row * col; i++) {
      data[i] = other.data[i];
    }
  }

  template <size_t, size_t> Matrix<T, r, c>(T t) : row(r), col(c) {
    for (size_t i = 0; i < row * col; i++)
      data[i] = t;
  }

  template <typename, size_t, size_t> Matrix(T t) : row(r), col(c) {
    for (size_t i = 0; i < row * col; i++)
      data[i] = t;
  }

  template <typename T, size_t, size_t>
  Matrix(size_t r, size_t c) : row(r), col(c) {
    assert(false);
  }

  template <typename T, size_t r, size_t c> Matrix() { assert(false); }
  template <typename T, size_t r, size_t c> Matrix(T t) : row(r), col(c) {
    for (size_t i = 0; i < row * col; i++)
      data[i] = t;
  }

  // template <typename NewTypeT> T &operator=(const T<NewTypeT> &rhs) {}

  //  template <typename> Matrix(T t) {}
  template <typename> Matrix(T t) : row(r), col(c) {
    for (size_t i = 0; i < row * col; i++) {
      data[i] = t;
    }
  }

  ~Matrix() { delete[] data; }

  template <size_t r, size_t c> void replace(const Matrix<T, r, c> &other) {
    delete[] data;
    row = other.row;
    col = other.col;
    data = new T[col * row];

    for (size_t i = 0; i < row * col; i++)
      data[i] = other.data[i];
  }

  T &getReference(const size_t i, const size_t j) {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  T &operator()(size_t i, size_t j) {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  T &operator()(size_t i, size_t j) const {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  template <size_t r, size_t c>
  Matrix &operator=(const Matrix<T, r, c> &other) {
    assert(other.row > 0);
    assert(other.col > 0);
    replace(other);
    return *this;
  }

  // template <typename T, size_t, size_t>
  bool operator==(const Matrix &other) const {
    if (other.row != row || other.col != col)
      return false;
    else {
      for (size_t i = 0; i < row * col; i++) {
        if (other.data[i] != data[i])
          return false;
      }
    }
    return true;
  }

  template <typename T, size_t r2, size_t c2>
  bool operator==(const Matrix<T, r2, c2> &other) const {
    return false;
  }

  template <typename T, size_t r2, size_t c2>
  bool operator!=(const Matrix<T, r2, c2> &other) const {
    return true;
  }

  bool operator!=(const Matrix<T, r, c> &other) const {
    return !(*this == other);
  }

  Matrix<T, r, c> operator+(const Matrix<T, r, c> &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Matrix<T, r, c> result;
    for (size_t i = 0; i < col * row; i++) {
      T d1 = data[i];
      T d2 = other.data[i];
      result.data[i] = d1 + d2;
    }
    return result;
  }

  Matrix<T, r, c> operator-(const Matrix<T, r, c> &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Matrix<T, r, c> result;
    for (size_t i = 0; i < col * row; i++) {
      T d1 = data[i];
      T d2 = other.data[i];
      result.data[i] = d1 - d2;
    }
    return result;
  }

  template <size_t L>
  Matrix<T, r, L> operator*(const Matrix<T, c, L> &other) const {
    Matrix<T, r, L> pm;
    for (size_t i = 0; i < pm.row * pm.col; i++) {
      int lr = i / pm.col;
      int rc = i % pm.col;
      for (size_t j = 0; j < col; j++) {
        int li = lr * col + j;
        int ri = j * other.col + rc;
        T val = data[li] * other.data[ri];
        pm.data[i] += val;
      }
    }
    return pm;
  }

  Matrix<T, r, c> &operator+=(const Matrix<T, r, c> &other) {
    Matrix<T, r, c> tmp = *this + other;
    this->replace(tmp);
    return *this;
  }

  Matrix<T, r, c> &operator-=(const Matrix<T, r, c> &other) {
    Matrix tmp = *this - other;
    this->replace(tmp);
    return *this;
  }

  Matrix<T, r, c> &operator*=(const Matrix<T, r, c> &other) {
    Matrix<T, r, c> tmp = *this * other;
    replace(tmp);
    return *this;
  }

  size_t rows() { return row; }
  size_t cols() { return col; }

  friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix) {
    for (int i = 0; i < matrix.row * matrix.col; i++) {
      output << matrix.data[i] << "\t";
      if ((i + 1) % matrix.col == 0)
        output << std::endl;
    }
    return output;
  }
};
