#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class HitRecord {
public:
    Point3f p;
    Vec3f normal;
    float t;
    bool front_face;

    // set the hit record normal direction
    void set_face_normal(const Ray &r, const Vec3f &outward_normal) {
        // 'outward_normal' is assumed to be unit vector
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, float ray_tmin, float ray_tmax, HitRecord &rec) const = 0;
};



#endif
