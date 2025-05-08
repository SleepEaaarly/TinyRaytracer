#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "image.h"
#include "hittable.h"

class RayTracer {
public:
    int samples_per_pixel = 10;

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

    Vec3f ray_color(const Ray &r, const Hittable &world);   
    Ray get_sample_ray(int i, int j);
};

#endif
