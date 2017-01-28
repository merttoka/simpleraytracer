#include "Vector3D.hpp"

Vector3D::Vector3D()
{
    _x=0; _y=0; _z=0;
}

Vector3D::Vector3D(float x, float y, float z)
{
    _x=x; _y=y; _z=z;
}
Vector3D::Vector3D(float xyz)
{
	_x = xyz; _y=xyz; _z=xyz;
}
Vector3D::Vector3D(const Vector3D& v)
{
    _x=v._x; _y=v._y; _z=v._z;
}

float Vector3D::x() const{return _x;}
float Vector3D::y() const{return _y;}
float Vector3D::z() const{return _z;}

void Vector3D::setX(float x){_x=x;}
void Vector3D::setY(float y){_y=y;}
void Vector3D::setZ(float z){_z=z;}

Vector3D Vector3D::operator+(const Vector3D& v) const
{
    return Vector3D(_x+v._x, _y+v._y, _z+v._z);
}
Vector3D Vector3D::operator- (void) const 
{
	return (Vector3D(-_x, -_y, -_z));    
}
Vector3D Vector3D::operator-(const Vector3D& v) const
{
    return Vector3D(_x-v._x, _y-v._y, _z-v._z);
}
Vector3D Vector3D::operator*(float num) const
{
    return Vector3D(_x*num, _y*num, _z*num);
}
Vector3D Vector3D::operator*(const Matrix3x3& m) const
{
    return Vector3D(m.get(0,0)*_x+m.get(0,1)*_y+m.get(0,2)*_z,
                    m.get(1,0)*_x+m.get(1,1)*_y+m.get(1,2)*_z,
                    m.get(2,0)*_x+m.get(2,1)*_y+m.get(2,2)*_z);
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    if (this == &v) //if not self assignment
        return (*this);

    _x = v._x; _y = v._y; _z = v._z;

    return (*this);
}
Vector3D& Vector3D::operator+=(const Vector3D& v)
{
    return (*this) = (*this)+v;
}
Vector3D& Vector3D::operator-=(const Vector3D& v)
{
    return (*this) = (*this)-v;
}
Vector3D& Vector3D::operator*=(float num)
{
    return (*this) = (*this)*num;
}

float Vector3D::dot(const Vector3D& v) const
{
    return _x*v._x+_y*v._y+_z*v._z;
}

Vector3D Vector3D::cross(const Vector3D& v) const
{
    return Vector3D(_y*v._z-_z*v._y, _z*v._x-_x*v._z, _x*v._y-_y*v._x);
}

float Vector3D::length() const{
    return sqrt(_x*_x+_y*_y+_z*_z);
}

void Vector3D::normalize(){
    _x = _x * (1/length());
    _y = _y * (1/length());
    _z = _z * (1/length());
}

float Vector3D::getDistance(const Vector3D& v) const{
    Vector3D new_v(v-(*this)); //v-(*this) creates new matrix orijined in (*this) to v.
                               //therefore its length gives the distance between them.
    return new_v.length();
}

bool Vector3D::operator==(const Vector3D& v)
{
    if(this->_x == v._x && this->_y == v._y && this->_z == v._z)
        return true;
    else
        return false;
}
bool Vector3D::operator!=(const Vector3D& v)
{
    return !(operator==(v));
}

// Output stream operator
// Should print vector to the output stream in the following format:
// (_x,_y,_z)
std::ostream& operator<<(std::ostream &os, const Vector3D &v)
{
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
