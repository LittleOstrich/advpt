#pragma once
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Matrix {

public:
  size_t row;
  size_t col;

  double *data;
  Matrix(std::size_t row, std::size_t col, double initValue)
      : row(row), col(col), data(nullptr) {
    data = new double[row * col];
    for (size_t i = 0; i < row * col; i++)
      data[i] = initValue;
  }

  ~Matrix() { delete[] data; }

  Matrix(const Matrix &other) : row(other.row), col(other.col), data(nullptr) {

    assert(other.row > 0);
    assert(other.col > 0);

    data = new double[col * row];
    for (size_t i = 0; i < row * col; i++)
      data[i] = other.data[i];
  }

  void replace(const Matrix &other) {
    delete[] data;
    row = other.row;
    col = other.col;
    data = new double[col * row];

    for (size_t i = 0; i < row * col; i++)
      data[i] = other.data[i];
  }

  Matrix &operator=(const Matrix &other) {
    assert(other.row > 0);
    assert(other.col > 0);
    replace(other);
    return *this;
  }

  double &getReference(const std::size_t i, const std::size_t j) {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  double &operator()(std::size_t i, std::size_t j) {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    return data[i * col + j];
  }

  const double &operator()(std::size_t i, std::size_t j) const {
    assert(i >= 0 && i < row);
    assert(j >= 0 && j < col);
    const double val = data[i * col + j];
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

  bool operator!=(const Matrix &other) const { return !(*this == other); }

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
    Matrix pm = Matrix(row, mat.col, 0);
    for (size_t i = 0; i < pm.row * pm.col; i++) {
      int lr = i / pm.col;
      int rc = i % pm.col;
      for (size_t j = 0; j < col; j++) {
        int li = lr * col + j;
        int ri = j * mat.col + rc;
        pm.data[i] += data[li] * mat.data[ri];
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
      if ((i + 1) % matrix.col == 0)
        output << std::endl;
    }
    return output;
  }

  //  friend std::istream &operator>>(std::istream &iss, Matrix &other) {
  //    int s1 = -1;
  //    int s2 = -1;
  //    int s3 = -1;
  //    std::string s(std::istreambuf_iterator<char>(iss), {});
  //    std::istringstream is(s);

  //    while ()

  //      //    is >> s1;
  //      //    if (!s1 || s1 == 0)
  //      //      assert(true);
  //      //    is >> s2;
  //      //    if (!s2 || s2 == 0)
  //      //      assert(true);
  //      //    is >> s3;
  //      //    if (!s3 || s3 == 0)
  //      //      assert(true);

  //      Matrix a(s1, s2, 0);
  //    Matrix b(s2, s3, 0);

  //    for (int i = 0; i < s1 * s2; i++) {
  //      is >> a.data[i];
  //      if (!a.data[i] && a.data[i] != 0)
  //        assert(false);
  //    }

  //    for (int i = 0; i < s2 * s3; i++) {
  //      is >> b.data[i];
  //      if (!b.data[i] && b.data[i] != 0)
  //        assert(false);
  //    }

  //    other = a * b;
  //    return is;
  //  }

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
    int temp_cols = num_entries / num_rows;
    assert(temp_cols * num_rows == num_entries);

    std::istringstream is3(s);
    std::string substring = "";
    int start = 0;
    int end = s.find("\n");
    std::string first_half = s.substr(0, end);
    std::string second_half = s.substr(end, string_length);
    size_t data_pos = 0;
    other = Matrix(num_rows, temp_cols, 0);
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
    return is;
  }
};
