//
// Created by qwertysun on 2018/7/19.
//

#include "object.h"

bool Sphere::IsHit(const Ray &r, float minT, float maxT, HitRecord &hitRec)
{
    Vector3 oc = r.Origin() - center;
    float a = r.Direction().Dot(r.Direction());
    float b = oc.Dot(r.Direction());
    float c = oc.Dot(oc) - radius * radius;
    float delta = b * b - a * c;
    if (delta > 0)
    {
        float root = static_cast<float>((-b - sqrt(delta)) / a);
        if (minT <= root && root <= maxT)
        {
            hitRec.t = root;
            hitRec.p = r.P(root);
            hitRec.normal = (hitRec.p - center).UnitVector();
            return true;
        }
        root = static_cast<float>((-b + sqrt(delta)) / a);
        if (minT <= root && root <= maxT)
        {
            hitRec.t = root;
            hitRec.p = r.P(root);
            hitRec.normal = (hitRec.p - center).UnitVector();
            return true;
        }
    }
    return false;
}
