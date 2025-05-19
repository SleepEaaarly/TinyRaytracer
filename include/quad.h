#pragma once

#include "hittable.h"

class Quad : public Hittable {
private:
    Point3f Q;
    Vec3f u, v;
    Vec3f w;
    Vec3f normal;
    shared_ptr<Material> mat;
    aabb bbox;
public:
    Quad(const Point3f& Q, const Vec3f& u, const Vec3f& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
        auto n = cross(u, v);
        w = n / dot(n, n);
        normal = n.unit();

        auto bbox1 = aabb(Q, Q + u + v);
        auto bbox2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox1, bbox2);
    }

    aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const {
        auto demon = dot(ray.direction(), normal);
        if (std::abs(demon) < 1e-6f) {
            return false;
        }

        auto orig = ray.origin();
        auto t = dot(Q - orig, normal) / demon;
        if (!ray_t.contains(t))
            return false;

        auto p = ray.at(t);
        auto alpha = dot(w, cross(p-Q, v));
        auto beta = dot(w, cross(u, p-Q));
        if (!Interval::unit.contains(alpha) || !Interval::unit.contains(beta)) 
            return false;
        
        rec.u = alpha;
        rec.v = beta;
        rec.mat = mat;
        rec.t = t;
        rec.p = p;
        rec.set_face_normal(ray, normal);

        return true;
    }

};