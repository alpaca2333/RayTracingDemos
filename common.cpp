#include "common.h"

Vector3::Vector3(float a, float b, float c)
{
    e[0] = a;
    e[1] = b;
    e[2] = c;
}

bool Hittables::IsHit(const Ray &r, float minT, float maxT, HitRecord &hitRec)
{
    bool hit = false;
    float tempt = maxT;
    for (int i = 0; i < hittables.size(); ++i)
    {
        if (hittables[i]->IsHit(r, minT, tempt, hitRec))
        {
            hit = true;
            tempt = hitRec.t;
        }
    }
    return hit;
}
