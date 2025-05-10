#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "image.h"
#include "hittable.h"
#include "ray.h"
#include "material.h"

class RayTracer {
public:
    int samples_per_pixel = 30;
    int max_depth = 50;

    RayTracer(Image &img);
    void render(const Hittable &world);

private:
    Image *image;
    int image_width;
    int image_height;

    float focal_length;
    Point3f center;
    Vec3f pixel_delta_u;
    Vec3f pixel_delta_v;
    Vec3f pixel00_loc;

    Color3f ray_color(const Ray &r, int depth, const Hittable &world);   
    Ray get_sample_ray(int i, int j);
};

#endif
