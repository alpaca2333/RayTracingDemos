#include "common.h"

using namespace std;

Vector3 RandomUnitVector()
{
    Vector3 p;
    do
    {
        p = 2.f * Vector3((double) drand48(), (double) drand48(), (double) drand48()) - Vector3(1, 1, 1);
    } while (p.Length() >= 1);
    return p;
}

Vector3::Vector3(double a, double b, double c)
{
    e[0] = a;
    e[1] = b;
    e[2] = c;
}

bool Objects::IsHit(const Ray &r, double minT, double maxT, HitRecord &hitRec)
{
    bool hit = false;
    double tempt = maxT;
    int lastI = -1 ;
    for (int i = 0; i < objects.size(); ++i)
    {
        if (objects[i]->IsHit(r, minT, tempt, hitRec))
        {
            hit = true;
            tempt = hitRec.t;
            lastI = i;
        }
    }
    if (lastI > -1)
    {
        objects[lastI]->material.Scatter(r, hitRec);
    }
    return hit;
}

Camera::Camera(const Vector3& lookFrom, const Vector3& lookAt,
         const Vector3& vup, float vfov, float aperture, float focusDist,
         int nx, int ny) : nx(nx), ny(ny)
{
    lensRadius = aperture / 2;
    double theta = vfov * M_PI / 180;
    double halfHeight = tan(theta / 2);
    double aspect = (double) nx / ny;
    double halfWidth = aspect * halfHeight;
    origin = lookFrom;
    w = (lookFrom - lookAt).UnitVector();
    u = vup.Cross(w).UnitVector();
    v = w.Cross(u);
    downLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
    hv = 2 * halfWidth * focusDist * u;
    vv = 2 * halfHeight * focusDist * v;
    // default color generator
    getColor = [](const Ray& r, Objects& os, int depth)
    {
        return Color(0, 0, 0);
    };
}

// rand ray of the pixel
Ray Camera::GetRay(double u, double v)
{
    Vector3 rd = lensRadius * RandomUnitVector();
    Vector3 offset = this->u * rd.e[0] + this->v * rd.e[1];
    return {origin + offset, downLeftCorner + u * hv + v * vv - origin - offset};
}

Vector3 vsqrt(const Vector3& v)
{
    return {sqrt(v.e[0]), sqrt(v.e[1]), sqrt(v.e[2])};
}

void Camera::Render(CachedPPM& ppm, Objects& objects)
{
    int samples = antiAliasing ? this->aaSamples : 1;
    int pi = 0;

    #pragma omp parallel for schedule(dynamic)
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Color tmp{0, 0, 0};
            // anti-aliasing
            for (int k = 0; k < samples; ++k)
            {
                double u = (double) i / nx;
                double v = (double) j / ny;
                double a = (2 * 3.1415926535 * (k + drand48()) / samples);
                u += (drand48() * cos(a)) / nx;
                v += (drand48() * sin(a)) / ny;
                Ray r = GetRay(u, v);
                tmp += vsqrt(getColor(r, objects, 0)) * 255.99;
            }
            tmp /= samples;
            ppm.Write(i, j, tmp);
            pi++;
            LogProgress(ppm.Progress());
        }
    }

    // render finished
    ppm.WriteToFile();
}

void Camera::LogProgress(double percent)
{
    std::cout << "\33[2K\r";
    printf("%.2f%%", percent * 100);
}

Vector3 Refrect(const Vector3& income, const Vector3& n, double r)
{
    if (income.Parallel(n)) return income;
    double indot = income.Dot(n);
    double cos1 = indot / income.Length() / n.Length();
    if (indot < 0)
    {
        double sin1 = sqrt(1 - cos1 * cos1);
        cos1 = -cos1;
        double sin2 = sin1 / r;
        double tan1 = abs(sin1 / cos1);
        double tan2 = abs(sin2 / sqrt(1 - sin2 * sin2));
        if (abs(sin2) >= 1)
        {
            return Reflect(income, n);
        }
        Vector3 n2 = n / n.Length() * income.Length() * cos1;
        Vector3 delta = income + n2;
        delta = delta / tan1 * tan2;
        return delta - n2;
    }
    else
    {
        double sin1 = sqrt(1 - cos1 * cos1);
        double sin2 = sin1 * r;
        double tan1 = abs(sin1 / cos1);
        double tan2 = abs(sin2 / sqrt(1 - sin2 * sin2));
        if (abs(sin2) >= 1)
        {
            return Reflect(income, n);
        }
        double cos1 = sqrt(1 - sin1 * sin1);
        Vector3 n2 = -n / n.Length() * income.Length() * cos1;
        Vector3 delta = income + n2;
        delta = delta / tan1 * tan2;
        return delta - n2;
    }
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "{" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "}";
    return os;
}


PPM::PPM(int nx, int ny, const char *filePath) : filePath(filePath)
{
    fout = new ofstream(filePath);
    *fout << "P3\n" << nx << " " << ny << "\n255\n";
}

void PPM::Write(Color &pix)
{

    double max = pix.e[0];
    max = max < pix.e[1] ? pix.e[1] : max;
    max = max < pix.e[2] ? pix.e[2] : max;
    if (max > 255.99)
    {
        pix.e[0] /= max / 255.99;
        pix.e[1] /= max / 255.99;
        pix.e[2] /= max / 255.99;
    }
    *fout << (int) pix.e[0] << "\t" << (int) pix.e[1] << "\t" << (int) pix.e[2] << "\n";
}

CachedPPM::CachedPPM(int nx, int ny, const char *filePath)
{
    this->nx = nx;
    this->ny = ny;
    this->filePath = filePath;
    colorCache = new Color[nx * ny];
    sum = 0;
}

#define max(a, b) (a > b ? a : b)
void CachedPPM::Write(int x, int y, const Color &c)
{
    int offset = y * nx + x;
    colorCache[offset] = c;
    double max = max(c.e[0], c.e[1]);
    max = max(max, c.e[2]);
    if (max > 255.99)
    {
        colorCache[offset] /= max / 255.99;
    }
    recursive_mutex _m;
    synchronized {
        ++this->sum;
    }
}

void CachedPPM::WriteToFile()
{
    ofstream fout(filePath);
    fout << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            int offset = j * nx + i;
            fout << (int) colorCache[offset].e[0] << "\t"
                 << (int) colorCache[offset].e[1] << "\t"
                 << (int) colorCache[offset].e[2] << "\n";
        }
    }
}