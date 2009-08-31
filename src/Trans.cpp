// Trans.cc

#include "Trans.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "trace.h"

//
// class Matrix
// 
// Basic constructor
Matrix::Matrix(int row, int column)
{
	_row = row;
	_column = column;
	_element = new double*[row];
	for(int i = 0; i < row; i++) _element[i] = new double[column];
}

Matrix::Matrix(int row, int column, double const * const m)
{
	int i, j;
	_row = row;
	_column = column;
	_element = new double*[row];
	for(i = 0; i < row; i++) _element[i] = new double[column];
	
	for(i = 0; i < row; i++)
		for(j = 0; j < column; j++) _element[i][j] = m[i * column + j];
}

// Copy constructor
Matrix::Matrix(const Matrix& m)
{
	_row = m.row();
	_column = m.column();
	_element = new double*[_row];
	for(int i = 0; i < _row; i++) {
		_element[i] = new double[_column];
		for(int j = 0; j < _column; j++) _element[i][j] = m[i][j];
	}
}

// Destructor
Matrix::~Matrix()
{
	for(int i = 0; i < _row; i++) delete [] _element[i];
	delete [] _element;
}

// Operator =
Matrix& Matrix::operator =(const Matrix& m)
{
	assert(_row == m.row() && _column == m.column());
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] = m[i][j];
		return *this;
}

// Operator =(double)
Matrix& Matrix::operator =(const double& x)
{
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++)
			if(i == j) _element[i][j] = x;
			else _element[i][j] = 0.0;
			return *this;
}

// Operator +=
Matrix& Matrix::operator +=(const Matrix& m)
{
	assert(_row == m.row() && _column == m.column());
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] += m[i][j];
		return *this;
}

// Operator -=
Matrix& Matrix::operator -=(const Matrix& m)
{
	assert(_row == m.row() && _column == m.column());
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] -= m[i][j];
		return *this;
}

// Operator *= (multiply a constant)
Matrix& Matrix::operator *=(const double& x)
{
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] *= x;
		return *this;
}

// Operator /= (devide by a constant)
Matrix& Matrix::operator /=(const double& x)
{
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] /= x;
		return *this;
}

// Operator +
Matrix operator +(const Matrix& m1, const Matrix& m2)
{
	Matrix result(m1);
	result += m2;
	return result;
}

// Operator -
Matrix operator -(const Matrix& m1, const Matrix& m2)
{
	Matrix result(m1);
	result -= m2;
	return result;
}

// Operator * (multiply a constant)
Matrix operator *(const Matrix& m, const double& x)
{
	Matrix result(m);
	result *= x;
	return result;
}

// Operator * (multiply a constant)
Matrix operator *(const double& x, const Matrix& m)
{
	Matrix result(m);
	result *= x;
	return result;
}

// Operator / (devide by a constant)
Matrix operator /(const Matrix& m, const double& x)
{
	Matrix result(m);
	result /= x;
	return result;
}

// Operator *
Matrix operator *(const Matrix& m1, const Matrix& m2)
{
	assert(m1.column() == m2.row());
	Matrix result(m1.row(), m2.column());
	for(int i = 0; i < result.row(); i++)
		for(int j = 0; j < result.column(); j++) {
			result[i][j] = 0.0;
			for(int k = 0; k < m1.column(); k++) result[i][j] += m1[i][k] * m2[k][j];
		}
		return result;
}

// Inverse
Matrix& Matrix::inv()
{
	int	i, j, k, m, order;
	double hold;
	
	assert(_row == _column);
	order = _row;
	Matrix R(order, 2 * order);
	
	for(i = 0; i < order; i++) {
		for(j = 0; j < order; j++) {
			R[i][j+order] = 0.0;
			R[i][j] = _element[i][j];
		}
		R[i][i+order] = 1.0;
	}
	
	for(k = 0; k < order; k++) {
		if(k != order - 1) {	
			m = k ;
			for(i = k + 1; i < order; i++)
				if(fabs(R[i][k]) > fabs(R[m][k])) m = i;
				if(m != k) {
					for(j = k; j < 2 * order; j++) {
						hold = R[k][j];
						R[k][j] = R[m][j];
						R[m][j] = hold;
					}
				}
		}
		for(j = k + 1; j < 2 * order; j++) {
			if(R[k][k] == 0.0) {
				for(i = 0; i < order; i++)
					for(j = 0; j < order; j++) _element[i][j] = (i == j);
					return *this;
			}
			R[k][j] /= R[k][k];
		}
		if(k != 0) {	
			for(i = 0; i < k; i++)
				for(j=k+1; j<2*order; j++ ) R[i][j] -= R[i][k] * R[k][j];
		}
		if(k != order - 1) {
			for(i=k+1 ; i<order ; i++ )
				for(j=k+1; j<2*order; j++ ) R[i][j] -= R[i][k] * R[k][j];
		}
	}
	
	for(i = 0; i < order; i++)
		for(j = 0; j < order; j++) _element[i][j] = R[i][j+order];
		
		return *this;
}

// Inverse
Matrix inv(const Matrix& m)
{
	Matrix result(m);
	result.inv();
	return result;
}

// Operator <<
ostream& operator <<(ostream& os, const Matrix& m)
{
	for(int i = 0; i < m.row(); i++) {
		os << "(";
		for(int j = 0; j < m.column(); j++) {
			if(j > 0) os << " ";
			os << m[i][j];
		}
		os << ")\n";
	}
	return os;
}

// Transposition 
Matrix t(const Matrix& m)
{
	Matrix result(m.column(), m.row());
	for(int i = 0; i < m.column(); i++)
		for(int j = 0; j < m.row(); j++) result[i][j] = m[j][i];
		return result;
}

//
// class Affine
//
// Constructors
Affine::Affine(const double& a00, const double& a01, 
			   const double& a02, const double& a03,
			   const double& a10, const double& a11,
			   const double& a12, const double& a13,
			   const double& a20, const double& a21,
			   const double& a22, const double& a23,
			   const double& a30, const double& a31,
			   const double& a32, const double& a33) : Matrix(4, 4)
{
	_element[0][0] = a00; _element[0][1] = a01;
	_element[0][2] = a02; _element[0][3] = a03;
	_element[1][0] = a10; _element[1][1] = a11;
	_element[1][2] = a12; _element[1][3] = a13;
	_element[2][0] = a20; _element[2][1] = a21;
	_element[2][2] = a22; _element[2][3] = a23;
	_element[3][0] = a30; _element[3][1] = a31;
	_element[3][2] = a32; _element[3][3] = a33;
}

Affine::Affine(const Point& p0, const Point& p1, const Point& p2, const Point& p3) : Matrix(4, 4)
{
	for(int i = 0; i < 4; i++) {
		_element[i][0] = p0[i];
		_element[i][1] = p1[i];
		_element[i][2] = p2[i];
		_element[i][3] = p3[i];
	}
}

// Copy constructor
Affine::Affine(const Matrix& m) : Matrix(4, 4)
{
	assert(m.row() >= 4 && m.column() >= 4);
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] = m[i][j];
}

Affine::Affine(const double& x) : Matrix(4, 4)
{
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) 
			if(i == j && i < 3) _element[i][j] = x;
			else if(i == 3 && j == 3) _element[i][j] = 1.0;
			else _element[i][j] = 0.0;
}

// Operator =
Affine& Affine::operator =(const Affine& m)
{
	assert(_row == m.row() && _column == m.column());
	for(int i = 0; i < _row; i++)
		for(int j = 0; j < _column; j++) _element[i][j] = m[i][j];
		return *this;
}

// Translation matrix
Affine translate(const double& x, const double& y, const double& z)
{
	Affine R(1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0);
	return R;
}

// Scaling matrix
Affine scale(const double& x, const double& y, const double& z)
{
	Affine R(x, 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0,
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return R;
}

// Rotation matrix
Affine rotateX(const double& a)
{
	Affine R(1.0, 0.0, 0.0, 0.0,
		0.0, cos(a), -sin(a), 0.0,
		0.0, sin(a), cos(a), 0.0,
		0.0, 0.0, 0.0, 1.0);
	return R;
}

Affine rotateY(const double& a)
{
	Affine R( cos(a),	 0.0,  sin(a),	 0.0,
		0.0,	 1.0,	 0.0,	 0.0,
		-sin(a),	 0.0,  cos(a),	 0.0,
		0.0,	 0.0,	 0.0,	 1.0);
	return R;
}

Affine rotateZ(const double& a)
{
	Affine R( cos(a), -sin(a), 0.0, 0.0,
		sin(a), cos(a), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return R;
}

//
// class Vector
//
// Copy constructor
Vector::Vector(const Matrix& m) : Matrix(m.row(), 1)
{
	assert(m.column() == 1);
	for(int i = 0; i < n(); i++) (*this)[i] = *(m[i]);
}

// Operator = 
Vector& Vector::operator =(const Vector& v)
{
	assert(n() == v.n());
	for(int i = 0; i < n(); i++) (*this)[i] = v[i];
	return *this;
}

// inter product
double interProduct(const Vector& v1, const Vector& v2)
{
	assert(v1.n() == v2.n());
	double result = 0.0;
	for(int i = 0; i < v1.n(); i++) result += v1[i] * v2[i];
	return result;
}

// Extra product
Vector extraProduct(const Vector& v1, const Vector& v2)
{
	assert(v1.n() >= 3 && v2.n() >= 3);
	Vector result(4);
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	result[3] = 1.0;
	return result;
}

// Normalize
Vector& Vector::normalize()
{
	double l = length(*this);
	if(l != 0.0) 
		for(int i = 0; i < n(); i++) (*this)[i] /= l;
		return *this;
}

// Operator << 
ostream& operator <<(ostream& os, const Vector& m)
{
	os << "(";
	for(int j = 0; j < m.row(); j++) {
		if(j > 0) os << ", ";
		os << m[j];
	}
	os << ")\n";
	return os;
}

// Length
double length(const Vector& v)
{
	double result = 0.0;
	for(int i = 0; i < v.n(); i++) result += v[i] * v[i];
	return sqrt(result);
}

// Distance
double distance(const Vector& v1, const Vector& v2) 
{
	assert(v1.n() == v2.n());
	double result = 0.0;
	for(int i = 0; i < v1.n(); i++) result += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	return sqrt(result);
}

// Angle
double angle(const Vector& v1, const Vector& v2) 
{
	assert(v1.n() == v2.n());
	double result;
	double l = length(v1) * length(v2);
	if(l > 0.0) result = acos(interProduct(v1, v2) / l);
	else result = 0.0;
	return result;
}

//
// class Point
//
// Constructor
Point::Point(const double& x, const double& y, const double& z, const double& w) : Vector(4)
{
	*(_element[0]) = x;
	*(_element[1]) = y;
	*(_element[2]) = z;
	*(_element[3]) = w;
}

// Copy constructor
Point::Point(const Matrix& m) : Vector(4)
{
	assert(m.row() >= 3 && m.column() == 1);
	x() = *(m[0]);
	y() = *(m[1]);
	z() = *(m[2]);
	if(m.row() >= 4) w() = *(m[3]);
	else w() = 1.0;
}

Point::Point(const double * const p) : Vector(4)
{
	x() = p[0];
	y() = p[1];
	z() = p[2];
	w() = 1.0;
}

// Operator =
Point& Point::operator =(const Point& p)
{
	x() = p.x();
	y() = p.y();
	z() = p.z();
	w() = p.w();
	return *this;
}

Point& Point::operator =(const double * const p)
{
	x() = p[0];
	y() = p[1];
	z() = p[2];
	w() = 1.0;
	return *this;
}

