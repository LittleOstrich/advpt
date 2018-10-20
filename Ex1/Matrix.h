#pragma once

class Matrix {

private:
  int m;
  int n;
  double *matrix = new double[0];

public:
  Matrix(std::size_t rows, std::size_t cols, double initValue) {
    m = rows;
    n = cols;
    if (matrix != nullptr) {
      std::cout << "I am not null!" << std::endl;
      delete matrix;
    }

    matrix = new double[m * n];

    for (int i = 0; i < m * n; i++) {
      matrix[i] = initValue;
    }
  }

  ~Matrix() {


	  }

  Matrix(const Matrix &mat) {
    if (matrix != nullptr) {
      delete matrix;
      std::cout << "I am not null!" << std::endl;
    }
    assert(mat.m > 0);
    assert(mat.n > 0);
    m = mat.m;
    n = mat.n;
    // delete matrix;
    matrix = new double[n * m];
    for (int i = 0; i < m * n; i++) {
      matrix[i] = mat.matrix[i];
    }
  }

  Matrix &operator=(const Matrix &mat) {
    if (matrix != nullptr) {
      std::cout << "I am not null!" << std::endl;
      delete matrix;
    }
    assert(mat.m > 0);
    assert(mat.n > 0);
    m = mat.m;
    n = mat.n;
    matrix = new double[n * m];
    for (int i = 0; i < m * n; i++) {
      matrix[i] = mat.matrix[i];
    };
    return *this;
  }

  double &operator()(std::size_t i, std::size_t j) {
    assert(i >= 0 && i < m);
    assert(j >= 0 && j < n);
    return matrix[i * n + j];
  }

  const double &operator()(std::size_t i, std::size_t j) const {
    assert(i >= 0 && i < m);
    assert(j >= 0 && j < n);
    const double ret = matrix[i * m + j];
    return ret;
  }

  bool operator==(const Matrix &mat) const {
    if (mat.m != m || mat.n != n)
      return false;
    else {
      for (int i = 0; i < m * n; i++) {
        if (mat.matrix[i] != matrix[i])
          return false;
      }
    }
    return true;
  }

  bool operator!=(const Matrix &mat) const {
    if (mat.m != m || mat.n != n)
      return true;
    else {
      for (int i = 0; i < m * n; i++) {
        if (mat.matrix[i] != matrix[i])
          return true;
      }
    }
    return false;
  }

  Matrix &operator+=(const Matrix &mat) {
    assert(mat.m == m);
    assert(mat.n == n);
    for (int i = 0; i < m * n; i++) {
      matrix[i] += mat.matrix[i];
    }
    return *this;
  }

  Matrix operator+(const Matrix &mat) const {
    Matrix a = Matrix(*this);
    a += mat;
    *matrix = *a.matrix;
    delete a.matrix;
    return *this;
  }

  Matrix &operator-=(const Matrix &mat) {
    assert(mat.m == m);
    assert(mat.n == n);
    for (int i = 0; i < m * n; i++) {
      matrix[i] -= mat.matrix[i];
    }
    return *this;
  }

  Matrix operator-(const Matrix &mat) const {

    Matrix a = Matrix(*this);
    a -= mat;
    delete matrix;
    *matrix = *a.matrix;
    return *this;
  }

  Matrix &operator*=(const Matrix &mat) {
    assert(mat.m == n);
    Matrix pm = Matrix(m, mat.n, 0);
    for (int i = 0; i < pm.n * pm.n; i++) {
      int lr = i / pm.m;
      int rc = i / mat.n;
      for (int j = 0; j < n; j++) {
        pm.matrix[i] += matrix[lr * n + j] * mat.matrix[rc * m + j];
      }
    }
    matrix = pm.matrix;
    m = pm.m;
    n = pm.n;
    delete pm.matrix;
    return *this;
  }

  Matrix operator*(const Matrix &mat) const {
    assert(mat.m == n);
    Matrix a = Matrix(*this);
    a *= mat;
    *matrix = *a.matrix;
    return a;

    /*Matrix pm = Matrix(m, mat.n, 0);
    for (int i = 0; i < pm.n * pm.n; i++) {
      int lr = i / pm.m;
      int rc = i / mat.n;
      for (int j = 0; j < n; j++) {
            pm.matrix[i] += matrix[lr * n + j] * mat.matrix[rc * m + j];
      }
    }
    return pm;
*/
  }

  std::size_t rows() const { return m; }
  std::size_t cols() const { return n; }

  friend std::ostream &operator<<(std::ostream &output, const Matrix &mat) {
    for (int i = 0; i < mat.m * mat.n; i++) {
      output << mat.matrix[i] << "\t";
      if (i % mat.m == 0)
        output << std::endl;
    }
    return output;
  }

  friend std::istream &operator>>(std::istream &is, Matrix &mat) { return is; };
};
