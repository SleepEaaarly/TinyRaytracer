#include "sphere.h"

bool Sphere::hit(const Ray &ray, Interval ray_t, HitRecord &rec) const {
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
    rec.mat = mat;

    return true;
}

aabb Sphere::bounding_box() const {
    return bbox;
}