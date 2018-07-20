#pragma once

#include "stdafx.h"

class Color;
class Vector3;

/**
 * Common 3-d vector definition
 */
class Vector3 
{
public:
    Vector3() = default;
    Vector3(float a, float b, float c);
    float e[3];
    inline float& operator[](int i) { return e[i]; }
    inline Vector3& operator=(const Vector3& vec);
    inline Vector3 operator-() const;
    friend inline Vector3 operator+(const Vector3& vec1, const Vector3& vec2);
    friend inline Vector3 operator-(const Vector3& vec1, const Vector3& vec2);
    inline Vector3 operator*(float k) const;
    inline Vector3 operator/(float k) const;
    inline Vector3& operator+=(const Vector3& vec);
    inline Vector3& operator-=(const Vector3& vec);
    inline Vector3& operator*=(float k);
    inline Vector3& operator/=(float k);
    inline float Dot(const Vector3 &vec) const;
    inline Vector3 Cross(const Vector3 &vec) const;
    inline Vector3 UnitVector();
    inline float Length();
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

class Color : public Vector3
{
public:
    Color() = default;
    Color(const Vector3& vec) : Vector3(vec) { }
    Color(float a, float b, float c) : Vector3(a, b, c) { }
};

/**
 * stores information about at which point a ray hits
 * an object and what the t-param is in the ray.
 */
struct HitRecord
{
    float t;
    Vector3 p, normal;
};

/**
 * common object definition.
 */
class Object
{
public:
    // decide whether the ray r hits this object.
    virtual bool IsHit(const Ray& r, float minT, float maxT, HitRecord& hitRec) = 0;
};

class Objects
{
public:
    virtual bool IsHit(const Ray& r, float minT, float maxT, HitRecord& hitRec);
    void Add(Object* hittable) { objects.push_back(hittable); }
    void Release() { for (auto* p: objects) delete p; }
    ~Objects() { Release(); }
protected:
    std::vector<Object*> objects;
};


typedef std::function<Color(const Ray&, Objects&)> ColorHandler;

class Camera
{
public:
    Camera(int nx, int ny, Color* buffer);
    Ray GetRay(float u, float v);
    void SetAntiAliasing(bool aa) { antiAliasing = aa; }
    void SetAaSamples(int samples) { aaSamples = samples; }
    void SetColorHandler(const ColorHandler& handler) { getColor = handler; }
    void Render(Objects& objects);
    void LogProgress(float percent);
protected:
    bool antiAliasing = true;
    int aaSamples = 100;    // 抗锯齿采样数量
    Vector3 origin;
    Vector3 downLeftCorner;
    Vector3 hv, vv;
    int nx, ny;
    Color* buffer;
    ColorHandler getColor;  // 根据光纤获得颜色的策略
};


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

Vector3 operator+(const Vector3& vec1, const Vector3& vec2)
{
    return {vec1.e[0] + vec2.e[0], vec1.e[1] + vec2.e[1], vec1.e[2] + vec2.e[2]};
}

Vector3 operator-(const Vector3& vec1, const Vector3& vec2)
{
    return {vec1.e[0] - vec2.e[0], vec1.e[1] - vec2.e[1], vec1.e[2] - vec2.e[2]};
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

float Vector3::Length()
{
    return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
}

Vector3 Vector3::UnitVector()
{
    return (*this) / Length();
}

Vector3 &Vector3::operator+=(const Vector3 &vec)
{
    e[0] += vec.e[0];
    e[1] += vec.e[1];
    e[2] += vec.e[2];
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &vec)
{
    e[0] -= vec.e[0];
    e[1] -= vec.e[1];
    e[2] -= vec.e[2];
    return *this;
}

Vector3 &Vector3::operator*=(float k)
{
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

Vector3 &Vector3::operator/=(float k)
{
    e[0] /= k;
    e[1] /= k;
    e[2] /= k;
    return *this;
}

inline Vector3 operator*(float k, const Vector3& vec)
{
    return vec * k;
}