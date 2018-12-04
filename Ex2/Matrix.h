#pragma once
#include "MatrixLike.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <typename T, size_t nRows, size_t nCols>
class Matrix : public MatrixLike<T, Matrix<T, nRows, nCols>, nRows, nCols> {

public:
  T *data;

  Matrix<T, nRows, nCols>() {
    static_assert(nRows >= 0, "");
    static_assert(nCols >= 0, "");
    data = new T[nRows * nCols];
    for (size_t i = 0; i < nRows * nCols; i++) {
      data[i] = T();
    }
  }

  Matrix<T, nRows, nCols>(T t) {
    data = new T[nRows * nCols];
    for (size_t i = 0; i < nRows * nCols; i++)
      data[i] = t;
  }

  // for vectors, seems to be needed in tests ( Solver.cpp, lines 104)
  Matrix<T, nRows, nCols>(const size_t dim, T t) {
    static_assert(nCols > 0, "Hello");
    static_assert(nRows > 0, "Hello");
    assert(nRows == dim);
    data = new T[nRows];
    for (size_t i = 0; i < nRows; i++)
      data[i] = t;
  }

  Matrix<T, nRows, nCols>(Matrix<T, nRows, nCols> &other) {
    data = new T[nRows * nCols];
    for (int i = 0; i < nRows * nCols; i++) {
      data[i] = other.data[i];
    }
  }

  Matrix<T, nRows, nCols>(const Matrix<T, nRows, nCols> &other) {
    data = new T[nRows * nCols];
    for (int i = 0; i < nRows * nCols; i++) {
      data[i] = other.data[i];
    }
  }

  // template <typename NewTypeT> T &operator=(const T<NewTypeT> &rhs) {}

  //  template <typename> Matrix(T t) {}

  ~Matrix() { delete[] data; }

  void replace(const Matrix<T, nRows, nCols> &other) {
    delete[] data;
    data = new T[nCols * nRows];

    for (size_t i = 0; i < nRows * nCols; i++)
      data[i] = other.data[i];
  }

  // entries for matrices
  T &operator()(int i, int j) {
    assert(i >= 0 && i < nRows);
    assert(j >= 0 && j < nCols);
    return data[i * nCols + j];
  }

  const T &operator()(int i, int j) const {
    assert(i >= 0 && i < nRows);
    assert(j >= 0 && j < nCols);
    return data[i * nCols + j];
  }

  // direct access, and also usable for vectors
  T &operator()(int i) {
    assert(i >= 0 && i < nRows * nCols);
    return data[i];
  }

  const T &operator()(int i) const {
    assert(i >= 0 && i < nRows * nCols);
    return data[i];
  }

  Matrix &operator=(const Matrix<T, nRows, nCols> &other) {
    replace(other);
    return *this;
  }

  bool operator==(const Matrix &other) const {
    for (size_t i = 0; i < nRows * nCols; i++) {
      if (other.data[i] != data[i])
        return false;
    }
    return true;
  }

  template <size_t other_r, size_t other_c>
  bool operator==(const Matrix<T, other_r, other_c> &other) const {
    if (nRows != other_r) {
      return false;
    }
    if (nCols != other_c) {
      return false;
    }
    for (size_t i = 0; i < nCols * nRows; i++) {
      if (data[i] != other.data[i]) {
        return false;
      }
    }
    return true;
  }

  template <size_t other_r, size_t other_c>
  bool operator!=(const Matrix<T, other_r, other_c> &other) const {
    return true;
  }

  Matrix<T, nRows, nCols>
  operator+(const Matrix<T, nRows, nCols> &other) const {
    Matrix<T, nRows, nCols> result;
    for (size_t i = 0; i < nCols * nRows; i++) {
      T d1 = data[i];
      T d2 = other.data[i];
      result.data[i] = d1 + d2;
    }
    return result;
  }

  Matrix<T, nRows, nCols>
  operator-(const Matrix<T, nRows, nCols> &other) const {
    Matrix<T, nRows, nCols> result;
    for (size_t i = 0; i < nCols * nRows; i++) {
      T d1 = data[i];
      T d2 = other.data[i];
      result.data[i] = d1 - d2;
    }
    return result;
  }

  // vectorlike product, not templated ->> always defined ->> satisfies
  // MatrixLike abstract definition
  Matrix<T, nRows, 1> operator*(const Matrix<T, nCols, 1> &other) const {
    Matrix<T, nRows, 1> pm;
    for (size_t i = 0; i < nRows; i++) {
      for (size_t j = 0; j < nCols; j++) {
        T val = this->operator()(i, j) * other.data[j];
        pm.data[i] += val;
      }
    }
    return pm;
  }

  // general matrix product, templated and only instantiates functions as needed
  template <size_t other_c>
  Matrix<T, nRows, other_c>
  operator*(const Matrix<T, nCols, other_c> &other) const {
    Matrix<T, nRows, other_c> pm;
    // something....
    return pm;
  }

  Matrix<T, nRows, nCols> &operator+=(const Matrix<T, nRows, nCols> &other) {
    Matrix<T, nRows, nCols> tmp = *this + other;
    this->replace(tmp);
    return *this;
  }

  Matrix<T, nRows, nCols> &operator-=(const Matrix<T, nRows, nCols> &other) {
    Matrix tmp = *this - other;
    this->replace(tmp);
    return *this;
  }

  Matrix<T, nRows, nCols> &operator*=(const Matrix<T, nRows, nCols> &other) {
    Matrix<T, nRows, nCols> tmp = *this * other;
    replace(tmp);
    return *this;
  }

  size_t rows() { return nRows; }
  size_t cols() { return nCols; }

  friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix) {
    for (int i = 0; i < matrix.nRows * matrix.nCols; i++) {
      output << matrix.data[i] << "\t";
      if ((i + 1) % matrix.nCols == 0)
        output << std::endl;
    }
    return output;
  }

  friend std::istream &operator>>(std::istream &iss, Matrix &other) {
    std::string s(std::istreambuf_iterator<char>(iss), {});
    std::istringstream is(s);

    double z = -1;
    size_t num_entries = 0;
    size_t num_rows = 0;
    size_t string_length = s.length();
    while (is >> z) {
      std::cout << z << std::endl;
      num_entries++;
    }

    std::istringstream is2(s);
    size_t pos = 0;
    while (pos < string_length) {
      if (s.at(pos) == '\n')
        num_rows++;
      pos++;
    }
    assert(num_rows != 0);
    size_t temp_cols = num_entries / num_rows;
    assert(temp_cols * num_rows == num_entries);

    std::istringstream is3(s);
    std::string substring = "";
    size_t start = 0;
    size_t end = s.find("\n");
    std::string first_half = s.substr(0, end);
    std::string second_half = s.substr(end, string_length);
    size_t data_pos = 0;

    // TODO: const expressions needed
    Matrix<T, num_rows, temp_cols> other;
    while (true) {
      std::istringstream temp_stream(first_half);
      while (temp_stream >> other.data[data_pos]) {
        data_pos++;
      }
      if (data_pos == num_entries) {
        break;
      }
      assert(data_pos % num_rows == 0);
      first_half = second_half;
      start = end;
      end = second_half.find("\n");
      second_half = s.substr(start, string_length);
    }
    std::cout << other << std::endl;
    return iss;
  }

  Matrix<T, nRows, nCols> inverseDiagonal() const {
    return *this;
    //<< what should I do here?
    // Matrix is likely a diagonal matrix, diagonal elements are then simply
    // inverted. I can do that.
  }

  T l2Norm() const {
    T accumulator = 0;
    for (unsigned int i = 0; i < nRows * nCols; i++) {
      accumulator = accumulator + data[i] * data[i];
    }
    return std::sqrt(accumulator);
  }

  size_t size() const { return nRows * nCols; }
};
