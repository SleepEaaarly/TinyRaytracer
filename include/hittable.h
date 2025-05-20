#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class Material;

class HitRecord {
public:
    Point3f p;
    Vec3f normal;
    shared_ptr<Material> mat;
    float t;
    float u;
    float v;
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

    virtual bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const = 0;

    virtual aabb bounding_box() const = 0;

    virtual void translate(const Vec3f& offset) = 0;

    virtual void rotate_y(float theta) = 0;
};


#endif
