#include "raytracer.h"

RayTracer::RayTracer(Image &img) {
    image = &img;
}

void RayTracer::init() {
    image_width = image->get_width();
    image_height = image->get_height();

    sqrt_spp = int(std::sqrt(samples_per_pixel));
    recip_sqrt_spp = 1.f / sqrt_spp;
    pixel_samples_scale = 1.f / (sqrt_spp * sqrt_spp);

    float viewport_height = 2 * tanf(degrees_to_radians(fovY/2)) * focus_dist;
    float viewport_width = viewport_height * (float(image_width)/image_height);
    center = eye;
    z_cam = (center - lookat).unit();
    Vec3f up = fabsf(z_cam.y) <= 0.999f ? Vec3f(0.f, 1.f, 0.f) : Vec3f(0.f, 0.f, -1.f);
    x_cam = cross(up, z_cam);
    y_cam = cross(z_cam, x_cam);

    // viewport_u(_v)是代表像平面的长宽，而image_width(_height)代表的是长宽像素数(分辨率)
    Vec3f viewport_u = viewport_width * x_cam;
    Vec3f viewport_v = -viewport_height * y_cam;

    pixel_delta_u = viewport_u / float(image_width);
    pixel_delta_v = viewport_v / float(image_height);

    Vec3f viewport_upper_left = center - (focus_dist * z_cam) - viewport_u/2.f - viewport_v/2.f;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
    auto defocus_radius = focus_dist * tanf(degrees_to_radians(defocus_angle / 2.f));
    defocus_disk_u = x_cam * defocus_radius;
    defocus_disk_v = y_cam * defocus_radius;
}

void RayTracer::render(const Hittable &world) {
    init();
    for (int i = 0; i < image_width; ++i) {
        for (int j = 0; j < image_height; ++j) {
            Vec3f pixel_color;
            // for (int sample = 0; sample < samples_per_pixel; ++sample) {
            //     Ray r = get_sample_ray(i, j);
            //     pixel_color += ray_color(r, max_depth, world);
            // }
            // pixel_color = pixel_color / float(samples_per_pixel);
            for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                    Ray r = get_sample_ray(i, j, s_i, s_j);
                    pixel_color += ray_color(r, max_depth, world);
                }
            }
            pixel_color = pixel_color * pixel_samples_scale;
            image->set(i, j, vec2Color(linear_to_gamma(pixel_color)));
        }
    }
}

Ray RayTracer::get_sample_ray(int i, int j) const {
    Vec3f pixel_center = pixel00_loc + 
                        (i+random_float(-0.5f, 0.5f)) * pixel_delta_u +
                        (j+random_float(-0.5f, 0.5f)) * pixel_delta_v;
    Vec3f ray_origin;
    if (defocus_angle <= 0.f) {
        ray_origin = center;
    } else {
        auto p = random_in_unit_disk();
        ray_origin = center + p[0] * defocus_disk_u + p[1] * defocus_disk_v;
    }

    Vec3f dir = pixel_center - ray_origin;
    float ray_time = random_float();

    return Ray(ray_origin, dir, ray_time);
}

Ray RayTracer::get_sample_ray(int i, int j, int s_i, int s_j) const {
    auto offset = sample_square_stratified(s_i, s_j);
    
    Vec3f pixel_center = pixel00_loc + 
                        (i+offset.x) * pixel_delta_u +
                        (j+offset.y) * pixel_delta_v;
    Vec3f ray_origin;
    if (defocus_angle <= 0.f) {
        ray_origin = center;
    } else {
        auto p = random_in_unit_disk();
        ray_origin = center + p[0] * defocus_disk_u + p[1] * defocus_disk_v;
    }

    Vec3f dir = (pixel_center - ray_origin).unit();
    float ray_time = random_float();

    return Ray(ray_origin, dir, ray_time);
}

Vec3f RayTracer::sample_square_stratified(int s_i, int s_j) const {
    auto px = (((s_i + random_float())) * recip_sqrt_spp) - 0.5f;
    auto py = (((s_j + random_float())) * recip_sqrt_spp) - 0.5f;

    return Vec3f(px, py, 0.f);
}

Color3f RayTracer::ray_color(const Ray &r, int depth, const Hittable &world) {
    // it means ray bounces between objects all the time and no light(emit or background) is touched.
    if (depth <= 0) 
        return Color3f(0.f, 0.f, 0.f);
    
    // Raytracing process
    HitRecord rec;
    if (!world.hit(r, Interval(0.001f, INFINITY), rec))
        return background;

    Vec3f attenuation;
    Ray scattered;
    auto emit_color = rec.mat->emit(rec.u, rec.v, rec.p);
    if (!rec.mat->scatter(r, rec, attenuation, scattered))
        return emit_color;
    
    auto scatter_pdf = rec.mat->scattering_pdf(r, rec, scattered);
    auto pdf_value = scatter_pdf;

    Color3f scatter_color;
    if (pdf_value < 1e-6)   scatter_color = Color3f(0.f, 0.f, 0.f);  
    else scatter_color = attenuation * ray_color(scattered, depth-1, world) / pdf_value;

    return scatter_color + emit_color;      // in fact, no material designed emit and scatter light at the same time.
}
