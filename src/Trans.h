// Trans.h
#ifndef __TRANS_H__
#define __TRANS_H__

#include <iostream>

using namespace std;

// class Matrix
class Matrix {
protected:
	int _row, _column;
	double **_element;
	
public:
	// Constructors and destructor
	Matrix(int row, int column);
	Matrix(int row, int column, const double * const m);
	Matrix(const Matrix& m);
	~Matrix();
	
	// Reference of data members
	int row() const { return _row; }
	int column() const { return _column; }
	double *operator [](int n) const { return _element[n]; }
	double *operator [](int n) { return _element[n]; }
	
	// Operators
	Matrix& operator =(const Matrix& m);
	Matrix& operator =(const double& x);
	Matrix& operator +=(const Matrix& m);
	Matrix& operator -=(const Matrix& m);
	Matrix& operator *=(const double& x);
	Matrix& operator /=(const double& x);
	
	friend Matrix operator +(const Matrix& m1, const Matrix& m2);
	friend Matrix operator -(const Matrix& m1, const Matrix& m2);
	friend Matrix operator *(const Matrix& m, const double& x);
	friend Matrix operator *(const double& x, const Matrix& m);
	friend Matrix operator /(const Matrix& m, const double& x);
	friend Matrix operator *(const Matrix& m1, const Matrix& m2);
	
	// Inverse
	Matrix& inv();
	friend Matrix inv(const Matrix& m);
	
	// Transposition
	friend Matrix t(const Matrix& m);
	
	// Print to stream
	friend ostream& operator <<(ostream& os, const Matrix& m);
};

class Point;

// class Affine
class Affine : public Matrix {
public:
	// Constructors
	Affine() : Matrix(4, 4) {}
	Affine(const double * const m) : Matrix(4, 4, m) {}
	Affine(int row, int column, const double * const m) : Matrix(row, column, m) {}
	Affine(const double& a00, const double& a01, 
		const double& a02, const double& a03,
		const double& a10, const double& a11,
		const double& a12, const double& a13,
		const double& a20, const double& a21,
		const double& a22, const double& a23,
		const double& a30, const double& a31,
		const double& a32, const double& a33);
	Affine(const Point& p0, const Point& p1, const Point& p2, const Point& p3);
	Affine(const Affine& a) : Matrix(a) {}
	Affine(const Matrix& m);
	Affine(const double& x);
	
	// Operators
	Affine& operator =(const Affine& a);
	
};

// Transformation matrix
Affine translate(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0);
Affine scale(const double& x = 1.0, const double& y = 1.0, const double& z = 1.0);
Affine rotateX(const double& a = 0.0);
Affine rotateY(const double& a = 0.0);
Affine rotateZ(const double& a = 0.0);


// class Vector
class Vector : public Matrix {
public:
	// Constructors
	Vector(int n) : Matrix(n, 1) {}
	Vector(const Vector& v) : Matrix(v) {}
	Vector(const Matrix& m);
	
	// Reference of data members
	int n() const { return _row; }
	double& operator [](int n) const { return *(_element[n]); }
	double& operator [](int n) { return *(_element[n]); }
	
	// Operators
	Vector& operator =(const Vector& v);
	friend double interProduct(const Vector& v1, const Vector& v2);
	friend Vector extraProduct(const Vector& v1, const Vector& v2);
	Vector& normalize();
	
	// Print to stream
	friend ostream& operator <<(ostream& os, const Vector& m);
	
	// Length
	friend double length(const Vector& v);
	
	// Distance
	friend double distance(const Vector& v1, const Vector& v2);
	
	// Angle
	friend double angle(const Vector& v1, const Vector& v2);
};

// class Point
class Point : public Vector {
public:
	// Constructors
	Point() : Vector(4) {}
	Point(const double& x, const double& y = 0.0, const double& z = 0.0, const double& w = 1.0);
	Point(const Point& p) : Vector(p) {}
	Point(const Matrix& m);
	Point(const double * const p);
	
	// Reference to data members
	const double& x() const { return *(_element[0]); }
	const double& y() const { return *(_element[1]); }
	const double& z() const { return *(_element[2]); }
	const double& w() const { return *(_element[3]); }
	double& x() { return *(_element[0]); }
	double& y() { return *(_element[1]); }
	double& z() { return *(_element[2]); }
	double& w() { return *(_element[3]); }
	
	// Operators
	Point& operator =(const Point& p);
	Point& operator =(const double * const p);
};

#endif

