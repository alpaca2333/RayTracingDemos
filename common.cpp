#include "common.h"

using namespace std;

Vector3::Vector3(float a, float b, float c)
{
    e[0] = a;
    e[1] = b;
    e[2] = c;
}

bool Objects::IsHit(const Ray &r, float minT, float maxT, HitRecord &hitRec)
{
    bool hit = false;
    float tempt = maxT;
    for (int i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->IsHit(r, minT, tempt, hitRec))
        {
            hit = true;
            tempt = hitRec.t;
        }
    }
    return hit;
}

Camera::Camera(int nx, int ny, Color* buffer) : nx(nx), ny(ny), buffer(buffer)
{
    float k = ((float) nx) / ny;
    downLeftCorner = Vector3(-k, -k / 2, -k / 2);
    vv = Vector3(0, 2, 0);
    hv = Vector3(2 * k, 0, 0);
    origin = Vector3(0, 0, 0);
    getColor = [](const Ray& r, Objects& os)
    {
        return Color(0, 0, 0);
    };
}

Ray Camera::GetRay(float u, float v)
{
    return {origin, downLeftCorner + u * hv + v * vv - origin};
}

void Camera::Render(Objects& objects)
{
    int samples = antiAliasing ? this->aaSamples : 1;
    int pi = 0;
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            buffer[pi] = {0, 0, 0};
            for (int k = 0; k < samples; ++k)
            {
                float u = (float) i / nx;
                float v = (float) j / ny;
                if (k > 0)
                {
                    u += (drand48() - 0.5) / nx;
                    v += (drand48() - 0.5) / ny;
                }
                Ray r = GetRay(u, v);
                buffer[pi] += getColor(r, objects) * 255.99;
            }
            buffer[pi] /= samples;
            pi++;
            LogProgress(((float) (ny - j - 1) * nx + i) / (nx * ny));
        }
    }
}

void Camera::LogProgress(float percent)
{
    std::cout << "\33[2K\r";
    printf("%.2f", percent * 100);
}
