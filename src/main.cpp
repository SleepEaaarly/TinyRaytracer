#include "rtweekend.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "sphere.h"
#include "image.h"

int main() {
    Image image(800, 600, Image::RGB);
    HittableList world;

    world.add(make_shared<Sphere>(Point3f(0.f, 0.f, -1.f), 0.5f));
    world.add(make_shared<Sphere>(Point3f(0.f, -100.5f, -1.f), 100.f));

    RayTracer raytracer(image);
    raytracer.render(world);

    image.write_png_file("rst.png");

    return 0;
}