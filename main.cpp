/*
 * Ray tracing demo.
 *
 * @author  qwerty sun
 * @date    2018-07-19
 */


#include "stdafx.h"
#include "ppm.h"
#include "object.h"
#include "common.h"
#include "material.h"

using namespace std;

Glass m{1};
Sphere sun{{-1, 8, -5}, 3, m};

Color ColorSky(const Ray& r)
{
    HitRecord hr;
    if (sun.IsHit(r, 0, MAXFLOAT, hr))
    {
        return {1, 1, 1};
    }
    Vector3 unitDir = r.Direction().UnitVector();
    auto t = 0.5 * (unitDir[1] + 1.0f);
    Vector3 result = {((1 - t) * Color(1, 1, 1) + t * Color(0.4, 0.6, 0.9))};
    return result;
}


int RenderSky(int nx, int ny, Color* buffer, const char* filePath)
{
    // Render
    double k = ((double) nx) / ny;
    Vector3 origin(0, 0, 0);
    Vector3 downLeft(-k, -k / 2, -k / 2);
    Vector3 hv(k * 2, 0, 0), vv(0, 2, 0);
    int pi = 0;
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            double u = (double) i / nx;
            double v = (double) j / ny;
            Ray r(origin, downLeft + u * hv + v * vv);
            buffer[pi++] = ColorSky(r) * 255.99;
            int a = 0;
        }
    }

    WriteRGBImg(filePath, nx, ny, buffer);
    return 0;
}


Color ColorBalls2(const Ray& r, Objects& os, int depth = 0)
{
    HitRecord hr;
    if (os.IsHit(r, 0, MAXFLOAT, hr))
    {
        if (depth > 50) {
            if (r.refracted)
            {
                bool a = r.refracted;
            }
            return {0, 0, 0};
        };
        Color c{0, 0, 0};
        for (auto& scatterInfo: hr.scatterInfos)
        {
            c += scatterInfo.attenuation * ColorBalls2(scatterInfo. outRay, os, ++depth);
        }
        return c;
    }
    else
    {
        return ColorSky(r);
    }
}


int DrawBalls(const char *filePath, int nx, int ny)
{
    // Init camera
    Vector3 lookFrom{-5, 0.2, -5};
    Vector3 lookAt{0, 0, -1};
    double focus = (lookAt - lookFrom).Length();
    float aperture = 0.2;
    Camera camera(lookFrom, lookAt, {0, 1, 0}, 20, aperture, focus,
                  nx, ny);

    // generate balls
    Objects objects;
    Lambertian m1({0.6, 0.6, 0.8});
    Lambertian m2({0.8, 0.5, 0.5});
    Metal m3({0.8, 0.6, 0.2});
    Glass m4(1.5); Glass m5(1 / 1.9);
    Object *sp1 = new Sphere({0, 0, -1}, 0.5, m4);
    Object *sp2 = new Sphere({0, -1000.5f, -1}, 1000, m1);
    Object *sp3 = new Sphere({1, 0, -1}, 0.5, m2);
    Object *sp4 = new Sphere({-1, 0, -1}, 0.5, m3);
    Object *sp5 = new Sphere({-1, 0, -1}, 0.45, m5);
    objects.Add(sp1);
    objects.Add(sp2);
    objects.Add(sp3);
    objects.Add(sp4);

    for (int i = 0; i < 100; ++i)
    {
        Material *m = NULL;
        double mrand = drand48();
        if (0 <= mrand && mrand < 0.33) m = new Lambertian{{drand48(), drand48(), drand48()}};
        else if (0.33 <= mrand && mrand < 0.66) m = new Glass(1 + drand48());
        else m = new Metal({drand48(), drand48(), drand48()});
        Object *tmp = new Sphere({drand48() * 10 - 5, -0.3, drand48() * 10 - 5}, 0.2, *m);
        objects.Add(tmp);
    }

    // configure camera
    camera.SetColorHandler(ColorBalls2);
    camera.SetAntiAliasing(true);
    camera.SetAaSamples(100);
    CachedPPM ppm(nx, ny, filePath);
    camera.Render(ppm, objects);
}

int main()
{
    const int nx = 1920, ny = 1080;
    DrawBalls("/mnt/c/Users/qwertysun/Desktop/balls.ppm", nx, ny);
    return 0;
}