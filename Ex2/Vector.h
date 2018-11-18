#pragma once
#include "Matrix.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template <typename T, size_t r> class Vector : public Matrix<T, r, 1> {

public:
  size_t row = 0;
  size_t col = 1;
  T *data;
  template <size_t new_row> void replace(const Vector<T, new_row> &other) {
    delete[] data;
    row = other.row;
    data = new T[col * row];

    for (size_t i = 0; i < row * col; i++)
      data[i] = other.data[i];
  }

  Vector<T, r>(T t) : row(r) {
    data = new T[row];
    for (size_t i = 0; i < row; i++) {
      data[i] = t;
    }
  }

  template <size_t new_row>
  Vector<T, r>(const Vector<T, new_row> &other) : row(new_row) {
    data = new T[row];
    for (size_t i = 0; i < row; i++) {
      data[i] = other.data[i];
    }
  }

  template <size_t> Vector<T, r>(T t) { assert(false); }
  template <typename T, size_t r> Vector() { assert(false); }
  template <typename T, size_t r> Vector(T t) { assert(false); }

  template <typename> Vector(T t) { assert(false); }
  //  template <typename T> Vector(T t) { assert(false); }

  ~Vector() { delete[] data; }

  Vector<T, r>() : row(r) {
    data = new T[row];
    for (size_t i = 0; i < row; i++) {
      data[i] = T();
    }
  }

  template <typename, size_t new_row> Vector<T, r>(T t) : row(new_row) {
    data = new T[r];
    for (size_t i = 0; i < row; i++) {
      data[i] = T(t);
    }
  }
  template <typename, size_t new_row>
  Vector<T, r>(const Vector<T, new_row> &other) : row(new_row) {
    data = new T[row];
    for (size_t i = 0; i < row; i++) {
      data[i] = other.data[i];
    }
  }

  template <typename, size_t> void replace(const Vector<T, r> &other) {
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

  T &operator()(size_t i) {
    assert(i >= 0 && i < row);
    return data[i];
  }

  T &operator()(size_t i, size_t j) const {
    assert(i >= 0 && i < row);
    const T val = data[i];
    return val;
  }

  Vector &operator=(const Vector &other) {
    assert(other.row > 0);
    assert(other.col > 0);
    replace(other);
    return *this;
  }

  bool operator==(const Vector &other) const {
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

  bool operator!=(const Vector<T, r> &other) const { return !(*this == other); }

  Vector<T, r> operator+(const Vector<T, r> &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Vector<T, r> result(row, col, 0);
    for (size_t i = 0; i < col * row; i++) {
      result.data[i] = this->data[i] + other.data[i];
    }
    return result;
  }

  Vector<T, r> operator-(const Vector<T, r> &other) const {
    assert(this->col == other.col);
    assert(this->row == other.row);
    Vector<T, r> result(row, col, 0);
    for (size_t i = 0; i < col * row; i++) {
      result.data[i] = this->data[i] - other.data[i];
    }
    return result;
  }

  Vector<T, r> operator*(const Vector<T, r> &other) const {
    Vector<T, r> pm = Vector(row, other.col, 0);
    for (size_t i = 0; i < pm.row * pm.col; i++) {
      int lr = i / pm.col;
      int rc = i % pm.col;
      for (size_t j = 0; j < col; j++) {
        int li = lr * col + j;
        int ri = j * other.col + rc;
        pm.data[i] += data[li] * other.data[ri];
      }
    }
    return pm;
  }

  Vector<T, r> &operator+=(const Vector<T, r> &other) {
    Vector<T, r> tmp = *this + other;
    this->replace(tmp);
    return *this;
  }

  Vector<T, r> &operator-=(const Vector<T, r> &other) {
    Vector tmp = *this - other;
    this->replace(tmp);
    return *this;
  }

  Vector<T, r> &operator*=(const Vector<T, r> &other) {
    Vector tmp = *this * other;
    this->replace(tmp);
    return *this;
  }

  template <typename, size_t> size_t rows() { return row; }
  template <typename, size_t> size_t cols() { return col; }

  friend std::ostream &operator<<(std::ostream &output,
                                  const Vector<T, r> &Vector) {
    for (int i = 0; i < Vector.row * Vector.col; i++) {
      output << Vector.data[i] << "\t";
    }
    output << std::endl;
    return output;
  }

  friend std::istream &operator>>(std::istream &iss, Vector<T, r> &other) {
    size_t s1 = 0;
    size_t s2 = 0;
    size_t s3 = 0;
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
    constexpr size_t const_s1 = make_const(s1);
    constexpr size_t const_s2 = make_const(s2);
    constexpr size_t const_s3 = make_const(s3);

    Vector<T, const_s1> a(nullptr);
    Vector<T, const_s3> b(nullptr);

    for (size_t i = 0; i < s1 * s2; i++) {
      is >> a.data[i];
      if (!a.data[i] && a.data[i] != 0)
        assert(false);
    }

    for (size_t i = 0; i < s2 * s3; i++) {
      is >> b.data[i];
      if (!b.data[i] && b.data[i] != 0)
        assert(false);
    }

    other = a * b;
    return is;
  }
};
