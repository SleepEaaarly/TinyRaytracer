#include "raytracer.h"
#include "ray.h"

RayTracer::RayTracer(Image &img) {
    image = &img;

    image_width = image->get_width();
    image_height = image->get_height();
    focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * (float(image_width)/image_height);
    center = Point3f(0.f, 0.f, 0.f);
    Vec3f viewport_u = Vec3f(viewport_width, 0.f, 0.f);
    Vec3f viewport_v = Vec3f(0.f, -viewport_height, 0.f);

    pixel_delta_u = viewport_u / float(image_width);
    pixel_delta_v = viewport_v / float(image_height);

    Vec3f viewport_upper_left = center - Vec3f(0.f, 0.f, focal_length) - viewport_u/2.f - viewport_v/2.f;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

void RayTracer::render(const Hittable &world) {
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            Vec3f pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
            Vec3f dir = (pixel_center - center).unit();
            Ray ray(center, dir);
            
            Color c = ray_color(ray, world);
            image->set(i, j, c);
        }
    }
}

Color RayTracer::ray_color(const Ray &r, const Hittable &world) {
    HitRecord rec;
    if (world.hit(r, 0.f, INFINITY, rec)) {
        return vec2Color(0.5f * (rec.normal + Vec3f(1.f, 1.f, 1.f)));
    }
    Vec3f ray_dir = r.direction();
    auto t = 0.5f * (ray_dir.y + 1.0f);
    Vec3f vec_c = (1.f-t)*Vec3f(1.0f, 1.0f, 1.0f)+t*Vec3f(0.5, 0.7, 1.0);
    return vec2Color(vec_c);
}
