#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "image.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "material.h"
#include "pdf.h"

class RayTracer {
public:
    int samples_per_pixel = 30;
    int sqrt_spp;
    float recip_sqrt_spp;
    float pixel_samples_scale;
    int max_depth = 50;
    float fovY = 90.f;
    Point3f eye = Point3f(0.f, 0.f, 0.f);
    Point3f lookat = Point3f(0.f, 0.f, -1.f);

    float defocus_angle = 0.f;
    float focus_dist = 10.f;
    Color3f background = Color3f(0.7f, 0.8f, 1.0f);

    RayTracer(shared_ptr<Image> image);
    void render(const Hittable &world, const HittableList& highlights);

private:
    shared_ptr<Image> image;
    int image_width;
    int image_height;

    Point3f center;
    Vec3f pixel_delta_u;
    Vec3f pixel_delta_v;
    Vec3f pixel00_loc;
    Vec3f x_cam, y_cam, z_cam;
    Vec3f defocus_disk_u;
    Vec3f defocus_disk_v;

    Color3f ray_color(const Ray &r, int depth, const Hittable& world, const HittableList& highlights);   
    Ray get_sample_ray(int i, int j) const;
    Ray get_sample_ray(int i, int j, int s_i, int s_j) const;
    Vec3f sample_square_stratified(int s_i, int s_j) const;
    void init();
};

#endif
