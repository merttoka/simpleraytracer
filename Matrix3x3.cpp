#include "Matrix3x3.hpp"

Matrix3x3::Matrix3x3()
{//default constructor sets matrix to identity
    data[0]=1; data[1]=0; data[2]=0;
    data[3]=0; data[4]=1; data[5]=0;
    data[6]=0; data[7]=0; data[8]=1;
}
Matrix3x3::Matrix3x3(float _00 , float _01 , float _02,
          float _10 , float _11 , float _12,
          float _20 , float _21 , float _22 )
{
    data[0]=_00; data[1]=_01; data[2]=_02;
    data[3]=_10; data[4]=_11; data[5]=_12;
    data[6]=_20; data[7]=_21; data[8]=_22;
}
Matrix3x3::Matrix3x3(const Matrix3x3& m)
{
    data[0]=m.data[0]; data[1]=m.data[1]; data[2]=m.data[2];
    data[3]=m.data[3]; data[4]=m.data[4]; data[5]=m.data[5];
    data[6]=m.data[6]; data[7]=m.data[7]; data[8]=m.data[8];
}

Matrix3x3& Matrix3x3::operator=(const Matrix3x3& m){
    if(this == &m)//if not self assignment
        return *this;

    data[0]=m.data[0]; data[1]=m.data[1]; data[2]=m.data[2];
    data[3]=m.data[3]; data[4]=m.data[4]; data[5]=m.data[5];
    data[6]=m.data[6]; data[7]=m.data[7]; data[8]=m.data[8];

    return (*this);
}


// Accessors
float Matrix3x3::get(int row, int col) const
{
    return data[row*3+col];
}
void Matrix3x3::set(int row, int col, float d)
{
    data[row*3+col] = d;
}
void Matrix3x3::setIdentity()
{
    data[0]=1; data[1]=0; data[2]=0;
    data[3]=0; data[4]=1; data[5]=0;
    data[6]=0; data[7]=0; data[8]=1;
}
Matrix3x3 Matrix3x3::getTranspose() const
{
    Matrix3x3 m;

    m.data[0]=data[0]; m.data[1]=data[3]; m.data[2]=data[6];
    m.data[3]=data[1]; m.data[4]=data[4]; m.data[5]=data[7];
    m.data[6]=data[2]; m.data[7]=data[5]; m.data[8]=data[8];

    return m;
}

float Matrix3x3::getDeterminant() const
{//used Sarrus Rule for determinant
    return (data[0]*data[4]*data[8] + data[1]*data[5]*data[6] + data[2]*data[3]*data[7]
          - data[2]*data[4]*data[6] - data[0]*data[5]*data[7] - data[1]*data[3]*data[8]);
}

Matrix3x3 Matrix3x3::getInverse() const
// inv(A) = adj(A)/det(A)
{
    float det = this->getDeterminant();
    if(det == 0)//Singular matrices are non-invertable
        throw "Not invertible - Singular Matrix";
    else
    {
        float b[9]; //adjoint

        //Cofactors of matrix
        b[0] = data[4] * data[8] - data[5] * data[7];
        b[1] = data[5] * data[6] - data[3] * data[8];
        b[2] = data[3] * data[7] - data[4] * data[6];
        b[3] = data[2] * data[7] - data[1] * data[8];
        b[4] = data[0] * data[8] - data[2] * data[6];
        b[5] = data[1] * data[6] - data[0] * data[7];
        b[6] = data[1] * data[5] - data[2] * data[4];
        b[7] = data[2] * data[3] - data[0] * data[5];
        b[8] = data[0] * data[4] - data[1] * data[3];

        Matrix3x3 inv(b[0]/det, b[3]/det, b[6]/det,
                      b[1]/det, b[4]/det, b[7]/det,
                      b[2]/det, b[5]/det, b[8]/det);
        return inv;
    }
}
Matrix3x3 Matrix3x3::operator+(const Matrix3x3& m) const
{
    return Matrix3x3(data[0]+m.data[0], data[1]+m.data[1], data[2]+m.data[2],
                     data[3]+m.data[3], data[4]+m.data[4], data[5]+m.data[5],
                     data[6]+m.data[6], data[7]+m.data[7], data[8]+m.data[8]);
}
Matrix3x3 Matrix3x3::operator-(const Matrix3x3& m) const
{
    return Matrix3x3(data[0]-m.data[0], data[1]-m.data[1], data[2]-m.data[2],
                     data[3]-m.data[3], data[4]-m.data[4], data[5]-m.data[5],
                     data[6]-m.data[6], data[7]-m.data[7], data[8]-m.data[8]);
}
Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) const
{
    Matrix3x3 m2;
    m2.data[0] = data[0]*m.data[0]+data[1]*m.data[3]+data[2]*m.data[6];
    m2.data[1] = data[0]*m.data[1]+data[1]*m.data[4]+data[2]*m.data[7];
    m2.data[2] = data[0]*m.data[2]+data[1]*m.data[5]+data[2]*m.data[8];

    m2.data[3] = data[3]*m.data[0]+data[4]*m.data[3]+data[5]*m.data[6];
    m2.data[4] = data[3]*m.data[1]+data[4]*m.data[4]+data[5]*m.data[7];
    m2.data[5] = data[3]*m.data[2]+data[4]*m.data[5]+data[5]*m.data[8];

    m2.data[6] = data[6]*m.data[0]+data[7]*m.data[3]+data[8]*m.data[6];
    m2.data[7] = data[6]*m.data[1]+data[7]*m.data[4]+data[8]*m.data[7];
    m2.data[8] = data[6]*m.data[2]+data[7]*m.data[5]+data[8]*m.data[8];

    return m2;
}
Matrix3x3 Matrix3x3::operator*(float num) const
{
    Matrix3x3 m2;
    m2.data[0] = data[0]*num;
    m2.data[1] = data[1]*num;
    m2.data[2] = data[2]*num;
    m2.data[3] = data[3]*num;
    m2.data[4] = data[4]*num;
    m2.data[5] = data[5]*num;
    m2.data[6] = data[6]*num;
    m2.data[7] = data[7]*num;
    m2.data[8] = data[8]*num;

    return m2;
}
Vector3D Matrix3x3::operator*(const Vector3D& v) const
{
    Vector3D v2;
    v2.setX(data[0]*v.x()+data[1]*v.y()+data[2]*v.z());
    v2.setY(data[3]*v.x()+data[4]*v.y()+data[5]*v.z());
    v2.setZ(data[6]*v.x()+data[7]*v.y()+data[8]*v.z());
    return v2;
}
void Matrix3x3::mulVector3D(const Vector3D& v, Vector3D& out)
{
    out = (*this)*v;
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& m)
{
    data[0] = data[0]+m.data[0]; data[1]= data[1]+m.data[1]; data[2]= data[2]+m.data[2];
    data[3] = data[3]+m.data[3]; data[4]= data[4]+m.data[4]; data[5]= data[5]+m.data[5];
    data[6] = data[6]+m.data[6]; data[7]= data[7]+m.data[7]; data[8]= data[8]+m.data[8];
    return (*this);
}
Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& m)
{
    data[0] = data[0]-m.data[0]; data[1]= data[1]-m.data[1]; data[2]= data[2]-m.data[2];
    data[3] = data[3]-m.data[3]; data[4]= data[4]-m.data[4]; data[5]= data[5]-m.data[5];
    data[6] = data[6]-m.data[6]; data[7]= data[7]-m.data[7]; data[8]= data[8]-m.data[8];
    return (*this);
}
Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& m)
{
    return (*this) = (*this)*m;
}
Matrix3x3& Matrix3x3::operator*=(const float num)
{
    return (*this) = (*this)* num;
}

// Output stream operator
// Should print matrix to the output stream in the following format:
// [[data(0) data(1) data(2)] [data(3) data(4) data(5)] [data(6) data(7) data(8)]]
// ========row1==============  ========= row2 ========= ========= row3 =========
std::ostream& operator<<(std::ostream &os, const Matrix3x3 &m)
{
    os <<"[["<<m.get(0,0)<<"\t"<<m.get(0,1)<<"\t"<<m.get(0,2)<<"] " << endl
       <<" ["<<m.get(1,0)<<"\t"<<m.get(1,1)<<"\t"<<m.get(1,2)<<"] " << endl
       <<" ["<<m.get(2,0)<<"\t"<<m.get(2,1)<<"\t"<<m.get(2,2)<<"]]";
    return os;
}
