// this file is just for documenting the required interface to compile with MatrixTest.cpp (declarations only)

#include <iostream>

class Matrix {
public:
	Matrix(std::size_t rows, std::size_t cols, double initValue);

	~Matrix();
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);

	double& operator()(std::size_t i, std::size_t j);
	const double& operator()(std::size_t i, std::size_t j) const;

	bool operator ==(const Matrix&) const;
	bool operator !=(const Matrix&) const;

	Matrix& operator +=(const Matrix&);
	Matrix operator +(const Matrix&) const;
	Matrix& operator -=(const Matrix&);
	Matrix operator -(const Matrix&) const;
	Matrix& operator *=(const Matrix&);
	Matrix operator *(const Matrix&) const;

	std::size_t rows() const;
	std::size_t cols() const;

	friend std::ostream& operator <<(std::ostream&, const Matrix&);
	friend std::istream& operator >>(std::istream&, Matrix&);
};
