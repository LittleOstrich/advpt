#pragma once

template <typename T, class Derived, size_t r, size_t c> class MatrixLike {
public:
  /// c'tor/ d'tor
  virtual ~MatrixLike() noexcept = 0; // pure virtual destructor

  /// virtual operators
  //  virtual Vector<T> operator*(const Vector<T> &o) const = 0;
  // feel free to extend as required

  // TODO: optimize the () operator
  virtual const T &operator()(int r, int c) const = 0;
  virtual T &operator()(int r, int c) = 0;

  /// other functions
  virtual Derived inverseDiagonal() const = 0;
  // feel free to extend as required

protected:
};

// MatrixLike d'tor implementation
template <typename T, class Derived, size_t r, size_t c>
inline void ~MatrixLike() noexcept {}
