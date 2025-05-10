#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3f &center, float radius, shared_ptr<Material> mat) : center(center), radius(std::max(0.f, radius)), mat(mat) {}
    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override;

private:
    Point3f center;
    float radius;
    shared_ptr<Material> mat;
};

#endif