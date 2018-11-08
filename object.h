//
// Created by qwertysun on 2018/7/19.
//

#pragma once

#include "stdafx.h"
#include "common.h"

class Sphere : public Object
{
public:
    Sphere(Vector3 center, double radius, const Material& m) : center(center), radius(radius), Object(m) { }
    bool IsHit(const Ray& r, double minT, double maxT, HitRecord& hitRec) override;
    Vector3 Center() { return center; }
    double Radius() { return radius; }
protected:
    Vector3 center;
    double radius;
};