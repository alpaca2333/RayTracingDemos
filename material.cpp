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
    bool fromOut = r.Direction().Dot(hr.normal) < 0;
    Vector3 dir1 = Refrect(r.Direction(), hr.normal, this->r);
    Ray sr(hr.p, dir1, r);
    sr.refracted = true;
    hr.scatterInfos.push_back({
        {1, 1, 1},
        sr
    });

    if (fromOut)
    {
        Vector3 dir2 = Reflect(r.Direction(), hr.normal);
        Ray sr2(hr.p, dir2, r);
        hr.scatterInfos.push_back({
             {0.2, 0.2, 0.2},
             sr2
        });
}
    return true;
}
