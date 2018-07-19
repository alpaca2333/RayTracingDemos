#include "stdafx.h"
#include "ppm.h"
#include "object.h"
using namespace std;


Color ColorSky(const Ray& r)
{
    Vector3 unitDir = r.Direction().UnitVector();
    auto t = static_cast<float>(0.5 * (unitDir[1] + 1.0f));
    return {((1 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1))};
}


int RenderSky(int nx, int ny, Color* buffer, const char* filePath)
{
    // Render
    float k = ((float) nx) / ny;
    Vector3 origin(0, 0, 0);
    Vector3 downLeft(-k, -k / 2, -k / 2);
    Vector3 hv(k * 2, 0, 0), vv(0, k, 0);
    int pi = 0;
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = (float) i / nx;
            float v = (float) j / ny;
            Ray r(origin, downLeft + u * hv + v * vv);
            buffer[pi++] = ColorSky(r) * 255.99;
        }
    }

    WriteRGBImg(filePath, nx, ny, buffer);
    return 0;
}

Color ColorBalls(const Ray& r, Hittables& h)
{
    HitRecord rec;
    if (h.IsHit(r, 0, MAXFLOAT, rec))
    {
        return {0.5 * Vector3(rec.normal[0] + 1, rec.normal[1] + 1, rec.normal[2] + 1)};
    }
    else
    {
        return ColorSky(r);
    }
}

int RenderBalls(const char* filePath, int nx, int ny, Color *buffer)
{
    float k = ((float) nx) / ny;
    Vector3 origin(0, 0, 0);
    Vector3 downLeft(-k, -k / 2, -k / 2);
    Vector3 hv(k * 2, 0, 0), vv(0, k, 0);
    // generate balls
    Hittables objects;
    Hittable *sp1 = new Sphere(Vector3(0, 0, -1), 0.5);
    Hittable *sp2 = new Sphere(Vector3(0, -100.5f, -1), 100);
    objects.Add(sp1);
    objects.Add(sp2);
    int pi = 0;
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = (float) i / nx;
            float v = (float) j / ny;
            Ray r(origin, downLeft + u * hv + v * vv);
            buffer[pi++] = ColorBalls(r, objects) * 255.99;
        }
    }
    WriteRGBImg(filePath, nx, ny, buffer);
}

int main()
{
    const int nx = 800, ny = 400;
    Color buffer[nx * ny];
    RenderBalls("/mnt/d/sky.ppm", nx, ny, buffer);
    return 0;
}