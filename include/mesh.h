#pragma once

#include "hittable.h"

class Mesh : public Hittable {
private:
    Point3f Q;
    Vec3f u, v;
    Vec3f w;
    Vec3f normal;
    shared_ptr<Material> mat;
    aabb bbox;
public:
    Mesh(const Point3f& Q, const Vec3f& u, const Vec3f& v, shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
        auto n = cross(u, v);
        w = n / dot(n, n);
        normal = n.unit();

        auto bbox1 = aabb(Q, Q + u);
        auto bbox2 = aabb(Q, Q + v);
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
        if (!(alpha >= 0.f && beta >= 0.f && alpha + beta <= 1.f)) 
            return false;
        
        rec.u = alpha;
        rec.v = beta;
        rec.mat = mat;
        rec.t = t;
        rec.p = p;
        rec.set_face_normal(ray, normal);

        return true;
    }
    
    void translate(const Vec3f& offset) override {
        Q = Q + offset;
        bbox = bbox + offset;
    }

    void rotate_y(float theta) override {   // rotate around mass_point y_axis
        theta = degrees_to_radians(theta);
        Point3f mass_point = Q + u / 3.f + v / 3.f;
        auto Q_x = Q.x - mass_point.x;
        auto Q_z = Q.z - mass_point.z;
        Q.x = Q_x * std::cos(theta) + Q_z * std::sin(theta) + mass_point.x;
        Q.z = Q_x * -std::sin(theta) + Q_z * std::cos(theta) + mass_point.z;
        float u_x, u_z, v_x, v_z;
        u_x = u.x; u_z = u.z; v_x = v.x; v_z = v.z;
        u.x = u_x * std::cos(theta) + u_z * std::sin(theta);
        u.z = u_x * -std::sin(theta) + u_z * std::cos(theta);
        v.x = v_x * std::cos(theta) + v_z * std::sin(theta);
        v.z = v_x * -std::sin(theta) + v_z * std::cos(theta);

        auto n = cross(u, v);
        w = n / dot(n, n);
        normal = n.unit();

        auto bbox1 = aabb(Q, Q + u);
        auto bbox2 = aabb(Q, Q + v);
        bbox = aabb(bbox1, bbox2);
    }
};
