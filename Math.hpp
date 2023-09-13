#ifndef MATH_HPP
#define MATH_HPP

#include<iostream>

namespace Orange
{

template<typename T, size_t N>
struct Vec;

template<typename T>
struct Vec<T, 2>
{
public:
    union
    {
        struct { T X; T Y; };
        struct { T U; T V; };
        T Raw[2];
    };

    Vec() : X{0}, Y{0} {}
    Vec(T scale) : X{scale}, Y{scale} {}
    Vec(T x, T y) : X{x}, Y{y} {}

    inline Vec<T, 2> operator+ (const Vec<T, 2> &other) const 
        { return Vec<T, 2>(X+other.X, Y+other.Y); }
    inline Vec<T, 2> operator- (const Vec<T, 2> &other) const 
        { return Vec<T, 2>(X-other.X, Y-other.Y); }
    inline Vec<T, 2> operator* (float f) const 
        { return Vec<T, 2>(X*f, Y*f); }
    
    template<typename U> 
    friend std::ostream& operator<<(std::ostream&, const Vec<U, 2>&);
};

template<typename U>
std::ostream& operator<<(std::ostream &os, const Vec<U, 2> &vec)
{
    std::cout << "Vec2{" << vec.X << ", " << vec.Y << "}";
    return os;
}

template<typename T>
struct Vec<T, 3>
{
public:
    union
    {
        struct { T X; T Y; T Z; };
        T Raw[3];
    };

    Vec() : X{0}, Y{0}, Z{0} {}
    Vec(T scale) : X{scale}, Y{scale}, Z{scale} {}
    Vec(T x, T y, T z) : X{x}, Y{y}, Z{z} {}

    inline Vec<T, 3> operator+ (const Vec<T, 3> &other) const 
        { return Vec<T, 3>(X+other.X, Y+other.Y, Z+other.Z); }
    inline Vec<T, 3> operator- (const Vec<T, 3> &other) const 
        { return Vec<T, 3>(X-other.X, Y-other.Y, Z+other.Z); }
    inline Vec<T, 3> operator* (float r) const 
        { return Vec<T, 3>(X*r, Y*r, Z*r); }
    
    template<typename U> 
    friend std::ostream& operator<<(std::ostream&, const Vec<U, 3>&);
};

template<typename U>
std::ostream& operator<<(std::ostream &os, const Vec<U, 3> &vec)
{
    std::cout << "Vec3{" << vec.X << ", " << 
        vec.Y << ", " << vec.Z << "}";
    return os;
}

template<typename T>
struct Vec<T, 4>
{
public:
    union
    {
        struct { T X; T Y; T Z; T W; };
        T Raw[4];
    };

    Vec() : X{0}, Y{0}, Z{0}, W{0} {}
    Vec(T scale) : X{scale}, Y{scale}, Z{scale}, W{scale} {}
    Vec(T x, T y, T z, T w) : X{x}, Y{y}, Z{z}, W{w} {}

    inline Vec<T, 4> operator+ (const Vec<T, 4> &other) const 
        { return Vec<T, 3>(X+other.X, Y+other.Y, Z+other.Z, W+other.W); }
    inline Vec<T, 4> operator- (const Vec<T, 4> &other) const 
        { return Vec<T, 3>(X-other.X, Y-other.Y, Z+other.Z, W+other.W); }
    inline Vec<T, 4> operator* (float r) const 
        { return Vec<T, 3>(X*r, Y*r, Z*r, W*r); }

    template<typename U> 
    friend std::ostream& operator<<(std::ostream&, const Vec<U, 4>&);
};

template<typename U>
std::ostream& operator<<(std::ostream &os, const Vec<U, 4> &vec)
{
    std::cout << "Vec3{" << vec.X << ", " << vec.Y << ", " <<
        vec.Z << ", " << vec.W << "}";
    return os;
}

using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

}

#endif