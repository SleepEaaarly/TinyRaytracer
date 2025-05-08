#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>
#include <iostream>
#include <cassert>

template <size_t N, typename T>
class vec {
public:
    T e[N];

    vec() { for (size_t i = 0; i < N; ++i) e[i] = T(); }
    
    T operator[](size_t i) const { assert(i<N); return e[i]; }
    T& operator[](size_t i) { assert(i<N); return e[i]; }
};

template<typename T>
class vec<3, T> {
public:
    T x,y,z;
    vec() { x=T(); y=T(); z=T(); }
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {} 
    vec<3, T> operator-() const { return vec<3, T>(-x, -y, -z); }
    T operator[](size_t i) const { assert(i < 3); return i<=0?x:(i==1?y:z); }
    T& operator[] (size_t i) { assert(i < 3); return i<=0?x:(i==1?y:z); }

    vec<3, T>& operator+=(const vec<3, T>& v) {
        x += v.x;  y += v.y;  z += v.z;
        return *this;
    }

    vec<3, T>& operator*=(T t) {
        x*=t; y*=t; z*=t;
        return *this;
    }

    vec<3, T>& operator/=(T t) {
        x/=t; y/=t; z/=t;
        return *this;
    }

    float norm() const {
        return sqrtf(x*x+y*y+z*z);
    }

    float norm_squared() const {
        return x*x+y*y+z*z;
    }

    vec<3, T> unit() {
        vec<3, T> rst = *this;
        rst/=rst.norm();
        return rst;
    }
};

template<typename T>
class vec<4, T> {
public:
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
    };
    vec() { x=T(); y=T(); z=T(); w=T(); }
    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {} 
    vec(vec<3, T> vec3, T W) : x(vec3.x), y(vec3.y), z(vec3.z), w(W) {}
    vec<4, T> operator-() const { return vec<4, T>(-x, -y, -z, -w); }
    T operator[](size_t i) const { assert(i < 4); return i<=0?x:(i==1?y:(i==2?z:w)); }
    T& operator[] (size_t i) { assert(i < 4); return i<=0?x:(i==1?y:(i==2?z:w)); }

    vec<4, T>& operator+=(const vec<4, T>& v) {
        x += v.x;  y += v.y;  z += v.z; w += v.w;
        return *this;
    }

    vec<4, T>& operator*=(T t) {
        x*=t; y*=t; z*=t; w*=t;
        return *this;
    }

    vec<4, T>& operator/=(T t) {
        x/=t; y/=t; z/=t; w/=t;
        return *this;
    }
};

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& out, const vec<N,T> &v) {
    out << v[0];
    for (int i = 1; i < N; ++i) {
        out << " " << v[i];
    }
    return out;
}

template <size_t N, typename T>
vec<N, T> operator+(const vec<N, T> &lhs, const vec<N, T> &rhs) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = lhs[i] + rhs[i]; }
    return rst;
}

template <size_t N, typename T>
vec<N, T> operator-(const vec<N, T> &lhs, const vec<N, T> &rhs) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = lhs[i] - rhs[i]; }
    return rst;
}

template <size_t N, typename T>
vec<N, T> operator*(const vec<N, T> &lhs, const vec<N, T> &rhs) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = lhs[i] * rhs[i]; }
    return rst;
}

template <size_t N, typename T>
vec<N, T> operator*(T t, const vec<N, T> &rhs) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = t * rhs[i]; }
    return rst;
}

template <size_t N, typename T>
vec<N, T> operator*(const vec<N, T> &lhs, T t) {
    return t * lhs;
}

template <size_t N, typename T>
vec<N, T> operator*(int t, const vec<N, T> &rhs) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = t * rhs[i]; }
    return rst;
}

template <size_t N, typename T>
vec<N, T> operator*(const vec<N, T> &lhs, int t) {
    return t * lhs;
}

template <size_t N, typename T>
vec<N, T> operator/(const vec<N, T> &lhs, T t) {
    vec<N, T> rst;
    for (int i = 0; i < N; ++i) { rst[i] = lhs[i] / t; }
    return rst;
}

template <size_t N, typename T>
T dot(const vec<N, T> &lhs, const vec<N, T> &rhs) {
    T rst = T();
    for (int i = 0; i < N; ++i) {
        rst += (lhs[i]*rhs[i]);
    }
    return rst;
}

template <typename T>
vec<3, T> cross(const vec<3, T> &lhs, const vec<3, T> &rhs) {
    return vec<3, T>(lhs[1]*rhs[2]-lhs[2]*rhs[1], lhs[2]*rhs[0]-lhs[0]*rhs[2], lhs[0]*rhs[1]-lhs[1]*rhs[0]);
}

template <typename T>
vec<3, T> cross(const vec<2, T> &lhs, const vec<2, T> &rhs) {
    return vec<3, T>(0, 0, lhs[0]*rhs[1]-lhs[1]*rhs[0]);
}

using Vec3f = vec<3, float>;
using Vec4f = vec<4, float>;
using Vec3i = vec<3, int>;
using Vec4i = vec<4, int>;
using Vec4uc = vec<4, unsigned char>;
using Point3f = Vec3f;

#endif