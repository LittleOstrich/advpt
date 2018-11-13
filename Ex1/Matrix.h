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

  friend std::istream &operator>>(std::istream &iss, Matrix &other) {
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;
    std::string s(std::istreambuf_iterator<char>(iss), {});
    std::istringstream is(s);
    is >> s1;
    if (!s1)
      assert(false);
    is >> s2;
    if (!s2)
      assert(false);
    is >> s3;
    if (!s3)
      assert(false);

    Matrix a(s1, s2, 0);
    Matrix b(s2, s3, 0);

    for (int i = 0; i < s1 * s2; i++) {
      is >> a.data[i];
      if (!a.data[i] && a.data[i] != 0)
        assert(false);
    }

    for (int i = 0; i < s2 * s3; i++) {
      is >> b.data[i];
      if (!b.data[i] && b.data[i] != 0)
        assert(false);
    }

    other = a * b;
    return is;
  }

  /*
  friend std::istream &operator>>(std::istream &is, Matrix &other) {

    std::string s(std::istreambuf_iterator<char>(is), {});
    int slen = s.length();
    int count_changes = 0;
    int cc = -1;  // col counter
    int rc = 0;   // row counter
    int tnoe = 0; // total number of elements
    int start = 0;
    int end = 0;

    for (int i = 0; i < slen; i++) {
      if (s.at(i) == '\n') {
        rc++; // we encountered the end of the line

        // prepare string to be extracted
        start = end;     // start index of our row in string form
        end = i + 1;     // last index of our row in string form
        int temp_cc = 0; // used for checking, if all "cols" are of same length
        double z;        // dump for doubles

        // create a substring and abuse, that std::istringstream parses double
        std::string t = s.substr(start, end - start);
        std::istringstream iss(t);
        while (iss >> z) {
          temp_cc++;
          tnoe++;
        }
        if (cc == -1)
          cc = temp_cc;
        else
          assert(temp_cc == cc);
      }
    }

    // create a substring and abuse, that std::istringstream parses double
    start = end;
    end = slen;
    std::string t = s.substr(start, end - start);
    std::istringstream iss(t);
    double z;
    int temp_cc = 0;

    // collect the elements of the last row
    while (iss >> z) {
      temp_cc++;
      tnoe++;
    }
    if (cc != -1 && t.length() > 0)
      assert(temp_cc == cc);

    std::istringstream fs(s); // stream containing all elements
    delete other.data;
    other.data = new double[tnoe];
    other.row = rc;
    other.col = cc;
    for (int i = 0; i < tnoe; i++)
      fs >> other.data[i];
    return is;
  }
*/
};
