#pragma once

#include "stdafx.h"

class Color;
class Vector3;

class Vector3 
{
public:
    Vector3() = default;
    Vector3(float a, float b, float c);
    float e[3];
    inline float& operator[](int i) { return e[i]; }
    inline Vector3& operator=(const Vector3& vec);
    inline Vector3 operator-() const;
    inline Vector3 operator+(const Vector3& vec) const;
    inline Vector3 operator-(const Vector3& vec) const;
    inline Vector3 operator*(float k) const;
    inline Vector3 operator/(float k) const;
    inline Vector3& operator+=(const Vector3& vec) { (*this) = *this + vec; return *this; }
    inline Vector3& operator-=(const Vector3& vec) { (*this) = *this - vec; return *this; }
    inline Vector3& operator*=(float k) { (*this) = *this * k; return *this; }
    inline Vector3& operator/=(float k) { (*this) = *this / k; return *this; }
    inline float Dot(const Vector3 &vec) const;
    inline Vector3 Cross(const Vector3 &vec) const;
};

/*
 * described by P = A + k·B
 * P is any point on the ray.
 * A is the origin point. B is the direction.
 */
class Ray
{
public:
    Ray() = default;
    Ray(const Vector3& A, const Vector3& B) : A(A), B(B) { }
    Vector3 Origin() const { return A; }
    Vector3 Direction() const { return B; }
    Vector3 P(float k) const { return A + B * k; }
    Vector3 operator[](float k) const { return P(k); }
protected:
    Vector3 A, B;
};

class Color : public Vector3 { };


//=========================== inline functions definitions ==============================
inline float Dot(const Vector3 &vec1, const Vector3 &vec2)
{
    return vec1.Dot(vec2);
}

inline Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2)
{
    return vec1.Cross(vec2);
}

Vector3 Vector3::operator-() const
{
    return {-e[0], -e[1], -e[2]};
}

Vector3 Vector3::operator+(const Vector3 &vec) const
{
    return {e[0] + vec.e[0], e[1] + vec.e[1], e[2] + vec.e[2]};
}

Vector3 Vector3::operator-(const Vector3 &vec) const
{
    return {e[0] - vec.e[0], e[1] - vec.e[1], e[2] - vec.e[2]};
}

Vector3 Vector3::operator*(float k) const
{
    return {e[0] * k, e[1] * k, e[2] * k};
}

Vector3 Vector3::operator/(float k) const
{
    return {e[0] / k, e[1] / k, e[2] / k};
}

Vector3 &Vector3::operator=(const Vector3 &vec)
{
    e[0] = vec.e[0];
    e[1] = vec.e[1];
    e[2] = vec.e[2];
    return *this;
}

float Vector3::Dot(const Vector3 &vec) const
{
    return e[0] * vec.e[0] + e[1] * vec.e[1] + e[2] * vec.e[2];
}

Vector3 Vector3::Cross(const Vector3 &vec) const
{
    return {
            e[1] * vec.e[2] - e[2] * vec.e[1],
            e[2] * vec.e[0] - e[0] * vec.e[2],
            e[0] * vec.e[1] - e[1] * vec.e[0]
    };
}
