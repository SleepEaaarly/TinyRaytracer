#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3f &static_center, float radius, shared_ptr<Material> mat) : center(static_center), radius(std::max(0.f, radius)), mat(mat) {}
    Sphere(const Point3f &center1, const Point3f &center2, float radius, shared_ptr<Material> mat) : center(center1, center2), radius(std::max(0.f, radius)), mat(mat) {}
    
    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override;

private:
    Path center;
    float radius;
    shared_ptr<Material> mat;
};

#endif