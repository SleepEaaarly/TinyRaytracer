#include "raytracer.h"

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
            Vec3f pixel_color;
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                Ray r = get_sample_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            pixel_color = pixel_color / float(samples_per_pixel);
            image->set(i, j, vec2Color(linear_to_gamma(pixel_color)));
        }
    }
}

Ray RayTracer::get_sample_ray(int i, int j) {
    Vec3f pixel_center = pixel00_loc + 
                        (i+random_float(-0.5f, 0.5f)) * pixel_delta_u +
                        (j+random_float(-0.5f, 0.5f)) * pixel_delta_v;
    Vec3f dir = (pixel_center - center).unit();
    return Ray(center, dir);
}

Color3f RayTracer::ray_color(const Ray &r, int depth, const Hittable &world) {
    if (depth <= 0) 
        return Color3f(0.f, 0.f, 0.f);
    
    // Raytracing process
    HitRecord rec;
    if (world.hit(r, Interval(0.001f, INFINITY), rec)) {
        Ray scattered;
        Color3f attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth - 1, world);
        }
        return Color3f(0.f, 0.f, 0.f);
    }

    // Background Color
    Vec3f ray_dir = r.direction();
    auto t = 0.5f * (ray_dir.y + 1.0f);
    return (1.f-t)*Color3f(1.0f, 1.0f, 1.0f)+t*Color3f(0.5f, 0.7f, 1.0f);
}
