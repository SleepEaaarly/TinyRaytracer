#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "image.h"
#include "hittable.h"

class RayTracer {
public:
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

    Color ray_color(const Ray &r, const Hittable &world);
};

#endif
