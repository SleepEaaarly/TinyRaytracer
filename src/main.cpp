#include "rtweekend.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "sphere.h"
#include "image.h"
#include "material.h"

int main() {
    Image image(800, 600, Image::RGB);
    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color3f(0.8f, 0.8f, 0.0f));
    auto material_center = make_shared<Lambertian>(Color3f(0.1f, 0.2f, 0.5f));
    auto material_left = make_shared<Metal>(Color3f(0.8f, 0.8f, 0.8f));
    auto material_right = make_shared<Metal>(Color3f(0.8f, 0.6f, 0.2f));

    world.add(make_shared<Sphere>(Point3f(0.f, 0.f, -1.f), 0.5f, material_center));
    world.add(make_shared<Sphere>(Point3f(0.f, -100.5f, -1.f), 100.f, material_ground));
    world.add(make_shared<Sphere>(Point3f(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    world.add(make_shared<Sphere>(Point3f(1.0f, 0.0f, -1.0f), 0.5f, material_right));

    RayTracer raytracer(image);
    raytracer.render(world);

    image.write_png_file("rst.png");

    return 0;
}