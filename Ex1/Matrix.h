#pragma once
#include <cassert>
#include <iostream>

class Matrix {

public:
  size_t row; // rename to row!
  size_t col; // rename to collums. lange aber deskriptive namen sind keine
              // schande!

  double *data;
  Matrix(std::size_t row, std::size_t col, double initValue)
      : row(row), col(col), data(nullptr) {
    data = new double[row * col];
    for (size_t i = 0; i < row * col; i++) {
      data[i] = initValue;
    }
  }

  // jedes objekt muss sich selbst aufräumen!
  ~Matrix() { delete[] data; }

  Matrix(const Matrix &other) : row(other.row), col(other.col), data(nullptr) {

    assert(other.row > 0);
    assert(other.col > 0);

    data = new double[col * row];
    for (size_t i = 0; i < row * col; i++) {
      data[i] = other.data[i];
    }
  }

  void replace(const Matrix &other) {
    delete[] data;
    row = other.row;
    col = other.col;
    data = new double[col * row];

    for (size_t i = 0; i < row * col; i++) {
      data[i] = other.data[i];
    }
  }

  // other ist quasi standard für derartige funktionen
  Matrix &operator=(const Matrix &other) {
    assert(other.row > 0);
    assert(other.col > 0);
    replace(other);
    return *this;
  }

  // diese funktion ist nur mit vorsicht zu verwenden
  double &getReference(const std::size_t i, const std::size_t j) {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  const double &operator()(std::size_t i, std::size_t j) {
    // den operator() sollte man nie überladen, verwirrt meistens nur
    // besser: .at(i,j) oder getData(i,j)
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * row + j];
  }

  const double &operator()(std::size_t i, std::size_t j) const {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    const double val = data[i * row + j];
    return val;
  }

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

  bool operator!=(const Matrix &other) const {
    // immer schön refactorisieren
    return !(*this == other);
  }

  Matrix operator+(const Matrix &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Matrix result(row, col, 0);
    for (size_t i = 0; i < col * row; i++) {
      result.data[i] = this->data[i] + other.data[i];
    }
    return result;
  }

  Matrix operator-(const Matrix &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Matrix result(row, col, 0);
    for (size_t i = 0; i < col * row; i++) {
      result.data[i] = this->data[i] - other.data[i];
    }
    return result;
  }

  Matrix operator*(const Matrix &mat) const {
    // uff mach das selber
    Matrix pm = Matrix(row, mat.col, 0);
    for (int i = 0; i < pm.row * pm.col; i++) {
      int lr = i / pm.row;
      int rc = i / mat.col;
      for (int j = 0; j < col; j++) {
        pm.data[i] += data[lr * col + j] * mat.data[rc * row + j];
      }
    }
    return pm;
  }

  Matrix &operator+=(const Matrix &other) {
    Matrix tmp = *this + other;
    this->replace(tmp);
    return *this;
  }

  Matrix &operator-=(const Matrix &other) {
    Matrix tmp = *this - other;
    this->replace(tmp);
    return *this;
  }

  Matrix &operator*=(const Matrix &other) {
    Matrix tmp = *this * other;
    this->replace(tmp);
    return *this;
  }

  std::size_t rows() const { return row; }
  std::size_t cols() const { return col; }

  friend std::ostream &operator<<(std::ostream &output, const Matrix &matrix) {
    for (int i = 0; i < matrix.row * matrix.col; i++) {
      output << matrix.data[i] << "\t";
      if (i % matrix.row == 0)
        output << std::endl;
    }
    return output;
  }

  friend std::istream &operator>>(std::istream &is, Matrix &mat) { return is; }
};
