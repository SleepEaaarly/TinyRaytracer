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
    
    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override {
        auto time = ray.time();
        auto center_t = center.at(time);

        Vec3f oc = center_t - ray.origin();
        auto a = ray.direction().norm_squared();
        auto h = dot(ray.direction(), oc);
        auto c = oc.norm_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0.f) {
            return false;
        }

        auto sqrtd = sqrtf(discriminant);

        // judge two roots
        auto root = (h - sqrtd) / a;
        if (root <= ray_t.min || ray_t.max <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_t.min || ray_t.max <= root) {
                return false;
            }
        }

        rec.t = root;
        rec.p = ray.at(rec.t);
        Vec3f outward_normal = (rec.p - center_t) / radius;
        rec.set_face_normal(ray, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

    void translate(const Vec3f& offset) override {
        center.src = center.src + offset;
        center.dst = center.dst + offset;
        bbox = bbox + offset;
    }

    void rotate_y(float theta) override {}

    float pdf_value(const Point3f& origin, const Vec3f& direction) const {
        HitRecord rec;
        if (!this->hit(Ray(origin, direction), Interval(0.001f, INFINITY), rec))
            return 0.f;

        auto dist_squared = (center.at(0) - origin).norm_squared();
        auto cos_theta_max = std::sqrt(1 - radius*radius/dist_squared);
        auto solid_angle = 2*pi*(1-cos_theta_max);
        if (solid_angle < 1e-6f) 
            return 0.f;

        return 1 / solid_angle;
    }

    Vec3f random(const Point3f& origin) const {
        Vec3f direction = center.at(0) - origin;
        auto distance_squared = direction.norm_squared();
        auto normal = direction.unit();

        // generate uniform random rays within solid angle
        auto r1 = random_float();
        auto r2 = random_float();
        auto z = 1 + r2*(std::sqrt(1-radius*radius/distance_squared) - 1);

        auto phi = 2*pi*r1;
        auto x = std::cos(phi) * std::sqrt(1-z*z);
        auto y = std::sin(phi) * std::sqrt(1-z*z);

        return normal_to_world_dir(Vec3f(x, y, z), normal);
    }

private:
    Path center;
    float radius;
    shared_ptr<Material> mat;
    aabb bbox;

    static void get_sphere_uv(const Point3f& p, float& u, float& v) {
        // u: return value [0,1] of angle around Y axis from X=-1
        // v: return value [0,1] of angle from Y=-1 to Y=+1

        auto theta = acosf(-p.y);
        auto phi = atan2f(-p.z, p.x) + pi;

        u = phi / (2.f * pi);
        v = theta / pi;
    }
};

#endif