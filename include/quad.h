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

    void translate(const Vec3f& offset) override {
        Q = Q + offset;
        bbox = bbox + offset;
    }

    void rotate_y(float theta, const Point3f& p) {
        theta = degrees_to_radians(theta);
        auto Q_x = Q.x - p.x;
        auto Q_z = Q.z - p.z;
        Q.x = Q_x * std::cos(theta) + Q_z * std::sin(theta) + p.x;
        Q.z = Q_x * -std::sin(theta) + Q_z * std::cos(theta) + p.z;
        float u_x, u_z, v_x, v_z;
        u_x = u.x; u_z = u.z; v_x = v.x; v_z = v.z;
        u.x = u_x * std::cos(theta) + u_z * std::sin(theta);
        u.z = u_x * -std::sin(theta) + u_z * std::cos(theta);
        v.x = v_x * std::cos(theta) + v_z * std::sin(theta);
        v.z = v_x * -std::sin(theta) + v_z * std::cos(theta);

        auto n = cross(u, v);
        w = n / dot(n, n);
        normal = n.unit();

        auto bbox1 = aabb(Q, Q + u + v);
        auto bbox2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox1, bbox2);
    }

    void rotate_y(float theta) override {   // rotate around mass_point y_axis
        Point3f mass_point = Q + u / 2.f + v / 2.f;
        rotate_y(theta, mass_point);
    }
};

class Box : public Hittable {
private:
    shared_ptr<HittableList> sides;
    Point3f mass_point;
public:
    Box(const Point3f& a, const Point3f& b, shared_ptr<Material> mat) {
        sides = make_shared<HittableList>();
        auto min = Point3f(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
        auto max = Point3f(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
        
        mass_point = (a + b) / 2.f;

        auto dx = Vec3f(max.x - min.x, 0.f, 0.f);
        auto dy = Vec3f(0.f, max.y - min.y, 0.f);
        auto dz = Vec3f(0.f, 0.f, max.z - min.z);

        sides->add(make_shared<Quad>(Point3f(min.x, min.y, max.z), dx, dy, mat)); // front
        sides->add(make_shared<Quad>(Point3f(max.x, min.y, max.z), -dz, dy, mat)); // right
        sides->add(make_shared<Quad>(Point3f(max.x, min.y, min.z), -dx, dy, mat)); // back
        sides->add(make_shared<Quad>(Point3f(min.x, min.y, min.z), dz, dy, mat)); // left
        sides->add(make_shared<Quad>(Point3f(min.x, max.y, max.z), dx, -dz, mat)); // top
        sides->add(make_shared<Quad>(Point3f(min.x, min.y, min.z), dx, dz, mat)); // bottom
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override {
        return sides->hit(ray, ray_t, rec);
    }

    aabb bounding_box() const override {
        return sides->bbox;
    }

    void translate(const Vec3f& offset) override {
        sides->translate(offset);
        mass_point = mass_point + offset;
    }

    void rotate_y(float theta) override {
        sides->bbox = aabb();
        for (auto &quad : sides->objects) {
            std::dynamic_pointer_cast<Quad>(quad)->rotate_y(theta, mass_point);
            sides->bbox = aabb(sides->bbox, quad->bounding_box());
        }
    }
};