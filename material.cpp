//
// Created by qwertysun on 18-7-21.
//

#include "material.h"
#include "object.h"

using namespace std;

bool Lambertian::Scatter(const Ray &r, HitRecord &hr) const
{
    Vector3 dir = hr.normal + RandomUnitVector();
    hr.scatterInfos.push_back({
                                      attenuation,
                                      Ray(hr.p, dir)
                              });
    return true;
}

bool Metal::Scatter(const Ray &r, HitRecord &hr) const
{
    Vector3 dir = Reflect(r.Direction(), hr.normal);
    hr.scatterInfos.push_back({
            attenuation,
            Ray(hr.p, dir)
    });
    return true;
}

bool Glass::Scatter(const Ray &r, HitRecord &hr) const
{
    Vector3 dir = Refrect(r.Direction(), hr.normal, this->r);
    Ray sr(hr.p, dir, r);
    sr.refracted = true;
    hr.scatterInfos.push_back({
            {1, 1, 1},
            sr
    });
    return true;
}
