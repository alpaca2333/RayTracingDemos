//
// Created by qwertysun on 2018/7/19.
//

#pragma once

#include "stdafx.h"
#include "common.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(Vector3 center, float radius) : center(center), radius(radius) { }
    bool IsHit(const Ray& r, float minT, float maxT, HitRecord& hitRec) override;
    Vector3 Center() { return center; }
    float Radius() { return radius; }
protected:
    Vector3 center;
    float radius;
};