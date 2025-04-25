#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    Point3f p;
    Vec3f normal;
    float t;
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, float ray_tmin, float ray_tmax, HitRecord &rec) const = 0;
};



#endif
