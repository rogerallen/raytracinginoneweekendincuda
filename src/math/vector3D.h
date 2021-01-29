#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vector3D  {

public:
    float _x, _y, _z;

    __host__ __device__ vector3D() {}
    __host__ __device__ vector3D(float a, float b, float c) { _x = a; _y = b; _z = c; }
    __host__ __device__ inline float x() const { return _x; }
    __host__ __device__ inline float y() const { return _y; }
    __host__ __device__ inline float z() const { return _z; }

    __host__ __device__ inline const vector3D& operator+() const { return *this; }
    __host__ __device__ inline vector3D operator-() const { return vector3D(-_x, -_y, -_z); }
    __host__ __device__ inline float operator[](int i) const { return ((&_x)[i]); }
    __host__ __device__ inline float& operator[](int i) { return ((&_x)[i]); };

    __host__ __device__ inline vector3D& operator+=(const vector3D &v);
    __host__ __device__ inline vector3D& operator-=(const vector3D &v);
    __host__ __device__ inline vector3D& operator*=(const vector3D &v);
    __host__ __device__ inline vector3D& operator/=(const vector3D &v);
    __host__ __device__ inline vector3D& operator*=(const float t);
    __host__ __device__ inline vector3D& operator/=(const float t);

    __host__ __device__ inline float length() const { return sqrt(x()*x() + y()*y() + z()*z()); }
    __host__ __device__ inline float squared_length() const { return x()*x() + y()*y() + z()*z(); }
    __host__ __device__ inline void make_unit_vector();
};

/*
inline std::istream& operator>>(std::istream &is, const vector3D &t) {
    is >> t.x() >> t.y() >> t.z();
    return is;
}
*/

inline std::ostream& operator<<(std::ostream &os, const vector3D &t) {
    os << t.x() << " " << t.y() << " " << t.z();
    return os;
}

__host__ __device__ inline void vector3D::make_unit_vector() {
    float k = 1.0 / sqrt(x()*x() + y()*y() + z()*z());
    _x *= k; _y *= k; _z *= k;
}

__host__ __device__ inline vector3D operator+(const vector3D &v1, const vector3D &v2) {
    return vector3D(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

__host__ __device__ inline vector3D operator-(const vector3D &v1, const vector3D &v2) {
    return vector3D(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

__host__ __device__ inline vector3D operator*(const vector3D &v1, const vector3D &v2) {
    return vector3D(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

__host__ __device__ inline vector3D operator/(const vector3D &v1, const vector3D &v2) {
    return vector3D(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

__host__ __device__ inline vector3D operator*(float t, const vector3D &v) {
    return vector3D(t*v.x(), t*v.y(), t*v.z());
}

__host__ __device__ inline vector3D operator/(vector3D v, float t) {
    t = 1.0f / t;
    return vector3D(t * v.x(), t * v.y(), t * v.z());
}

__host__ __device__ inline vector3D operator*(const vector3D &v, float t) {
    return vector3D(t * v.x(), t * v.y(), t * v.z());
}

__host__ __device__ inline vector3D& vector3D::operator+=(const vector3D &v){
    _x  += v.x();
    _y  += v.y();
    _z  += v.z();
    return *this;
}

__host__ __device__ inline vector3D& vector3D::operator*=(const vector3D &v){
    _x  *= v.x();
    _y  *= v.y();
    _z  *= v.z();
    return *this;
}

__host__ __device__ inline vector3D& vector3D::operator/=(const vector3D &v){
    _x  /= v.x();
    _y  /= v.y();
    _z  /= v.z();
    return *this;
}

__host__ __device__ inline vector3D& vector3D::operator-=(const vector3D& v) {
    _x  -= v.x();
    _y  -= v.y();
    _z  -= v.z();
    return *this;
}

__host__ __device__ inline vector3D& vector3D::operator*=(const float t) {
    _x  *= t;
    _y  *= t;
    _z  *= t;
    return *this;
}

__host__ __device__ inline vector3D& vector3D::operator/=(const float t) {
    float k = 1.0f / t;

    _x  *= k;
    _y  *= k;
    _z  *= k;
    return *this;
}

__host__ __device__ inline float dot(const vector3D &v1, const vector3D &v2) {
    return v1.x() * v2.x() + v1.y() * v2.y()  + v1.z() * v2.z();
}

__host__ __device__ inline vector3D cross(const vector3D &v1, const vector3D &v2) {
    return vector3D( (v1.y()*v2.z() - v1.z()*v2.y()),
                   (-(v1.x()*v2.z() - v1.z()*v2.x())),
                     (v1.x()*v2.y() - v1.y()*v2.x()));
}

__host__ __device__ inline vector3D unit_vector(vector3D v) {
    return v / v.length();
}

__host__ __device__ inline vector3D project(const vector3D& a, const vector3D& b) {
	return (b * (dot(a, b) / dot(b, b)));
}

__host__ __device__ inline vector3D reject(const vector3D& a, const vector3D& b) {
	return (a - b * (dot(a, b) / dot(b, b)));
}

//__host__ __device__ inline vector3D reflect(const vector3D &i, const vector3D &n) {
//    //return i - 2.0f * dot(i, n) * n;
//    return  2.0f * dot(i, n) * n - i; // i vettore uscente dalla normale
//}
