#ifndef Vector3D_HPP
#define Vector3D_HPP

#include <math.h>
#include "Matrix3x3.hpp"
class Matrix3x3;    //****added by merttoka****

#include <iostream>
using namespace std;

class Vector3D {

public:
    // Constructors
    Vector3D();
    Vector3D(float x, float y, float z);
	Vector3D(float xyz);
    Vector3D(const Vector3D& v);

    // Assignment operator (can be chained)
    Vector3D& operator=(const Vector3D& v);

    // Accessors
    float x() const;
    float y() const;
    float z() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    // Misc. operations
    float dot(const Vector3D& v) const; // dot product
    Vector3D cross(const Vector3D& v) const; // cross product

    void normalize(); // makes this vector a unit vector
    float length() const; // returns the length of the vector
    float getDistance(const Vector3D& v) const; // returns the distance to the given vector

    // Arithmetic
    Vector3D operator+(const Vector3D& v) const;
    Vector3D operator-(const Vector3D& v) const;
	Vector3D operator- (void) const;
    Vector3D operator*(float num) const;
    Vector3D operator*(const Matrix3x3& m) const; // multiply given matrix with the vector

    Vector3D& operator+=(const Vector3D& v);
    Vector3D& operator-=(const Vector3D& v);
    Vector3D& operator*=(float num);


    // Logical
    bool operator==(const Vector3D& v); // equality operator
    bool operator!=(const Vector3D& v); // inequality operator

private:

    float _x, _y, _z;

};

// Output stream operator
// Should print vector to the output stream in the following format:
// (_x,_y,_z)
//******************
// operator overloading functions must be free functions
// taken outside of class by merttoka
//******************
std::ostream &operator<<(std::ostream &os, const Vector3D &v);

#endif // Vector3D_HPP
