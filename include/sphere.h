#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3f &static_center, float radius, shared_ptr<Material> mat) : Sphere(static_center, static_center, radius, mat) {}

    Sphere(const Point3f &center1, const Point3f &center2, float radius, shared_ptr<Material> mat) : center(center1, center2), radius(std::max(0.f, radius)), mat(mat) {
        Vec3f rvec(this->radius, this->radius, this->radius);
        aabb box1(center.dst - rvec, center.dst + rvec);
        aabb box2(center.src - rvec, center.src + rvec);
        bbox = aabb(box1, box2);
    }
    
    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override;

    aabb bounding_box() const override;
private:
    Path center;
    float radius;
    shared_ptr<Material> mat;
    aabb bbox;
};

#endif