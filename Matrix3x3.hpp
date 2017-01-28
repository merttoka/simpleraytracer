#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include <iostream>
#include "Vector3D.hpp"
class Vector3D;     //****added by merttoka****

class Matrix3x3 {

public:
    // Constructors
    Matrix3x3();
    Matrix3x3(float _00 , float _01 , float _02,
              float _10 , float _11 , float _12,
              float _20 , float _21 , float _22 ); // assign values in row-major order: data[0] = _00, data[1] = _01 etc...
    Matrix3x3(const Matrix3x3& m);

    // Assignment operator
    Matrix3x3& operator=(const Matrix3x3& m);


    // Accessors
    float get(int row, int col) const;
    void set(int row, int col, float d);


    // Misc. operations
    void setIdentity(); // makes the matrix identity matrix
    Matrix3x3 getTranspose() const; // get transpose of current matrix
    float getDeterminant() const; // calculate determinant
                                  // ****const added by merttoka****
    Matrix3x3 getInverse() const; // get inverse of current matrix

    // Arithmetic
    Matrix3x3 operator+(const Matrix3x3& m) const;
    Matrix3x3 operator-(const Matrix3x3& m) const;
    Matrix3x3 operator*(const Matrix3x3& m) const;
    Matrix3x3 operator*(float num) const;
    Vector3D operator*(const Vector3D& v) const; // multiply given vector with the matrix
    void mulVector3D(const Vector3D& v, Vector3D& out); // multiply given vector v with the matrix and put result to vector out

    Matrix3x3& operator+=(const Matrix3x3& m);
    Matrix3x3& operator-=(const Matrix3x3& m);
    Matrix3x3& operator*=(const Matrix3x3& m);
    Matrix3x3& operator*=(const float num);

private:
    float data[9];

};

// Output stream operator
// Should print matrix to the output stream in the following format:
// [[data(0) data(1) data(2)] [data(3) data(4) data(5)] [data(6) data(7) data(8)]]
// ========row1==============  ========= row2 ========= ========= row3 =========
//*****************
// operator overloading functions must be free functions
// taken outside of class by merttoka
//*****************
std::ostream &operator<<(std::ostream &os, const Matrix3x3 &m);

#endif // MATRIX3X3_HPP
