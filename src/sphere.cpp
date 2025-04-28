#include "sphere.h"

bool Sphere::hit(const Ray &ray, float ray_tmin, float ray_tmax, HitRecord &rec) const {
    Vec3f oc = center - ray.origin();
    auto a = ray.direction().norm_squared();
    auto h = dot(ray.direction(), oc);
    auto c = oc.norm_squared() - radius*radius;

    auto discriminant = h*h - a*c;
    if (discriminant < 0.f) {
        return false;
    }

    auto sqrtd = std::sqrtf(discriminant);

    // judge two roots
    auto root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            return false;
        }
    }

    rec.t = root;
    rec.p = ray.at(rec.t);
    Vec3f outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);

    return true;
}