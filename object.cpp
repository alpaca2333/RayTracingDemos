//
// Created by qwertysun on 2018/7/19.
//

#include "object.h"

bool Sphere::IsHit(const Ray &r, double minT, double maxT, HitRecord &hitRec)
{
    Vector3 oc = r.Origin() - center;
    double a = r.Direction().Dot(r.Direction());
    double b = oc.Dot(r.Direction());
    double c = oc.Dot(oc) - radius * radius;
    double delta = b * b - a * c;
    if (delta > 0)
    {
        double root = (-b - sqrt(delta)) / a;
        if (minT + SURFACE_THICKNESS < root && root < maxT) {
            hitRec.t = root;
            hitRec.p = r.P(root);
            hitRec.normal = (hitRec.p - center).UnitVector();
            return true;
        }
        root = (-b + sqrt(delta)) / a;
        if (minT + SURFACE_THICKNESS < root && root < maxT)
        {
            hitRec.t = root;
            hitRec.p = r.P(root);
            hitRec.normal = (hitRec.p - center).UnitVector();
            return true;
        }
    }
    return false;
}
