#pragma once

#include "stdafx.h"

#define SURFACE_THICKNESS 0.00000001

class Color;
class Vector3;
class Material;
class HitRecord;

extern double drand48(void);

//#define synchronized(m) \
//    for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

static int __mutexIndex__ = 0;
#define synchronized std::recursive_mutex _m_##__mutexIndex__;\
    ++__mutexIndex__;\
    for (std::unique_lock<std::recursive_mutex> lk(_m_##__mutexIndex__); lk; lk.unlock())
/**
 * Common 3-d vector definition
 */
class Vector3 
{
public:
    Vector3() = default;
    Vector3(double a, double b, double c);
    double e[3];
    inline double& operator[](int i) { return e[i]; }
    inline Vector3& operator=(const Vector3& vec);
    inline Vector3 operator-() const;
    friend inline Vector3 operator+(const Vector3& vec1, const Vector3& vec2);
    friend inline Vector3 operator-(const Vector3& vec1, const Vector3& vec2);
    inline Vector3 operator*(double k) const;
    inline Vector3 operator*(const Vector3& vec);
    inline Vector3 operator/(double k) const;
    inline Vector3 operator/(const Vector3& vec);
    inline Vector3& operator+=(const Vector3& vec);
    inline Vector3& operator-=(const Vector3& vec);
    inline Vector3& operator*=(double k);
    inline Vector3& operator/=(double k);
    inline double Dot(const Vector3 &vec) const;
    inline Vector3 Cross(const Vector3 &vec) const;
    inline Vector3 UnitVector();
    inline double Length() const;
    inline bool operator!=(const Vector3& v);
    inline bool Parallel(const Vector3 &v) const;
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};

Vector3 RandomUnitVector();

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
    Ray(const Vector3& A, const Vector3& B, const Ray& previous): Ray(A, B)
    {
        refracted = previous.refracted;
    }
    Vector3 Origin() const { return A; }
    Vector3 Direction() const { return B; }
    Vector3 P(double k) const { return A + B * k; }
    Vector3 operator[](double k) const { return P(k); }
    bool refracted = false;
protected:
    Vector3 A, B;
};

/*
 * I'm wondering if the definition of Color is meaningful...
 * Maybe a typedef is enough :(
 */
class Color : public Vector3
{
public:
    Color() = default;
    Color(const Vector3& vec) : Vector3(vec) { }
    Color(double a, double b, double c) : Vector3(a, b, c) { }
};
// material definition
struct ScatterInfo
{
    Vector3 attenuation;
    Ray outRay;
};


/**
 * Stores information about at which point a ray hits
 * an object and what the t-param is in the ray.
 */
struct HitRecord
{
    HitRecord() = default;
    HitRecord(
            double t, const Vector3& p, const Vector3 normal, const Material& m
    ) : t(t), p(p), normal(normal), scatterInfos(scatterInfos) { }
    double t;
    Vector3 p, normal;
    std::vector<ScatterInfo> scatterInfos;
};

/*
 * Base definition of material.
 * Concrete definitions of materials should be put in 'material.h'
 */
class Material
{
public:
    virtual bool Scatter(
            const Ray &r, HitRecord &hr) const { hr.scatterInfos = {}; };
};

/**
 * common object definition.
 */
class Object
{
public:
    Object(const Material& m): material(m) {   }
    // decide whether the ray r hits this object.
    virtual bool IsHit(const Ray& r, double minT, double maxT, HitRecord& hitRec) = 0;
    const Material& material;
};

class Objects
{
public:
    virtual bool IsHit(const Ray& r, double minT, double maxT, HitRecord& hitRec);
    void Add(Object* hittable) { objects.push_back(hittable); }
    void Release() { for (auto* p: objects) delete p; }
    ~Objects() { Release(); }
protected:
    std::vector<Object*> objects;
};

/*
 * A color handler is directly called by camera's render function.
 * It decides the color of each tracing ray.
 */
typedef std::function<Color(const Ray&, Objects&, int)> ColorHandler;

// PPM writer
class PPM
{
public:
    PPM(int nx, int ny, const char* filePath);
    // Could change the value of color if rgb values exceed 255
    void Write(Color &c);
    ~PPM() { delete fout; }
protected:
    const char* filePath;
    std::ofstream *fout;
};

/* All Write operations are done in memory first.
 * Call WriteToFile() to save changes to file.
 * Being add to support multi-thread rendering.
 */
class CachedPPM
{
public:
    CachedPPM(int nx, int ny, const char* filePath);

    void WriteToFile();

    /* not thread-safe */
    void Write(int x, int y, const Color &c);

    inline double Progress() { return sum / ((double) nx * ny); }

    inline void ClearProgress() { sum = 0; }

    ~CachedPPM() { delete colorCache; }
private:
    int nx, ny;
    const char* filePath;
    Color* colorCache;
    int sum;
};

class Camera
{
public:
    Camera(const Vector3& lookFrom, const Vector3& lookAt,
           const Vector3& vup, float vfov, float aperture, float focusDist,
           int nx, int ny);
    Ray GetRay(double u, double v);
    void SetAntiAliasing(bool aa) { antiAliasing = aa; }

    /* Set number of samples for each pixel the camera would take when rendering */
    void SetAaSamples(int samples) { aaSamples = samples; }
    void SetColorHandler(const ColorHandler& handler) { getColor = handler; }
    void Render(CachedPPM& ppm, Objects& objects);
    void LogProgress(double percent);
protected:
    bool antiAliasing = true;
    int aaSamples = 100;    // amount of sample token for each pixel
    Vector3 origin;
    Vector3 downLeftCorner;
    Vector3 u, v, w;
    Vector3 hv, vv;
    int nx, ny;
    // the color handler will decide each pixel's color
    // it is the most important method
    ColorHandler getColor;
    float lensRadius;
};


//=========================== inline function definitions ==============================

inline double Dot(const Vector3 &vec1, const Vector3 &vec2)
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

Vector3 Vector3::operator*(double k) const
{
    return {e[0] * k, e[1] * k, e[2] * k};
}

Vector3 Vector3::operator/(double k) const
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

double Vector3::Dot(const Vector3 &vec) const
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

double Vector3::Length() const
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

Vector3 &Vector3::operator*=(double k)
{
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

Vector3 &Vector3::operator/=(double k)
{
    e[0] /= k;
    e[1] /= k;
    e[2] /= k;
    return *this;
}

Vector3 Vector3::operator*(const Vector3 &vec)
{
    return {e[0] * vec.e[0], e[1] * vec.e[1], e[2] * vec.e[2]};
}

Vector3 Vector3::operator/(const Vector3 &vec)
{
    return {e[0] / vec.e[0], e[1] / vec.e[1], e[2] / vec.e[2]};
}

bool Vector3::operator!=(const Vector3 &v)
{
    return e[0] == v.e[0] && e[0] == v.e[1] && e[2] == v.e[2];
}

bool Vector3::Parallel(const Vector3 &v) const
{
    double k = 0;
    for (int i = 0; i < 3; ++i)
    {
        if (e[i])
        {
            if (k && (k - v.e[i] / e[i]) > SURFACE_THICKNESS)
            {
                return false;
            }
            k = v.e[i] / e[i];
        }
    }
    return true;
}

inline Vector3 operator*(double k, const Vector3& vec)
{
    return vec * k;
}

inline Vector3 Reflect(const Vector3& income, const Vector3& n)
{
    Vector3 n2 = income.Dot(n) < 0 ? -n : n;
    return income - 2 * income.Dot(n2) * n2;
}

Vector3 Refrect(const Vector3& income, const Vector3& n, double r);

