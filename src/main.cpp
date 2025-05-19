#include "rtweekend.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "sphere.h"
#include "image.h"
#include "material.h"
#include "bvh.h"
#include "quad.h"
#include "mesh.h"
#include <chrono>

void bouncing_spheres() {
    Image image(1200, 675, Image::RGB);
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32f, Color3f(.2f, .3f, .1f), Color3f(.9f, .9f, .9f));
    auto ground_material = make_shared<Lambertian>(checker);
    world.add(make_shared<Sphere>(Point3f(0.f, -1000.f, 0.f), 1000.f, ground_material));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            Point3f center(a + 0.9f*random_float(), 0.2f, b + 0.9f*random_float());
            if ((center - Point3f(4.f, 0.2f, 0.f)).norm() > 0.9f) {
                shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = random_vector() * random_vector();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3f(0.f, random_float(0.f, 0.5f), 0.f);
                    world.add(make_shared<Sphere>(center, center2, 0.2f, sphere_material));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = random_vector(0.5f, 1.f);
                    auto fuzz = random_float(0.f, 0.5f);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5f);
                    world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }
    auto material1 = make_shared<Dielectric>(1.5f);
    world.add(make_shared<Sphere>(Point3f(0.f, 1.f, 0.f), 1.0f, material1));
    auto material2 = make_shared<Lambertian>(Color3f(0.4f, 0.2f, 0.1f));
    world.add(make_shared<Sphere>(Point3f(-4.f, 1.f, 0.f), 1.0f, material2));
    auto material3 = make_shared<Metal>(Color3f(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<Sphere>(Point3f(4.f, 1.f, 0.f), 1.0f, material3));

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;

    raytracer.fovY = 20.f;
    raytracer.eye = Point3f(13.f, 2.f, 3.f);
    raytracer.lookat = Point3f(0.f, 0.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void checkered_spheres() {
    Image image(400, 225, Image::RGB);
    HittableList world;   
    
    auto checker = make_shared<CheckerTexture>(0.32f, Color3f(.2f, .3f, .1f), Color3f(.9f, .9f, .9f));

    // world.add(make_shared<Sphere>(Point3f(0.f, -10.f, 0.f), 10.f, make_shared<Lambertian>(checker)));
    // world.add(make_shared<Sphere>(Point3f(0.f, 10.f, 0.f), 10.f, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3f(0.f, 0.f, 0.f), 5.f, make_shared<Lambertian>(checker)));

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;

    raytracer.fovY = 20.f;
    raytracer.eye = Point3f(13.f, 2.f, 3.f);
    raytracer.lookat = Point3f(0.f, 0.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void earth() {
    Image image(400, 225, Image::RGB);
    HittableList world;  

    auto earth_texture = make_shared<ImageTexture>("../images/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point3f(0.f,0.f,0.f), 2.f, earth_surface);
    world.add(globe);

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;

    raytracer.fovY = 20.f;
    raytracer.eye = Point3f(0.f, 0.f, 12.f);
    raytracer.lookat = Point3f(0.f, 0.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void perlin_spheres() {
    Image image(400, 225, Image::RGB);
    HittableList world;
    
    auto pertext = make_shared<NoiseTexture>(4.f);
    world.add(make_shared<Sphere>(Point3f(0.f, -1000.f, 0.f), 1000.f, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point3f(0.f, 2.f, 0.f), 2.f, make_shared<Lambertian>(pertext)));

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;

    raytracer.fovY = 20.f;
    raytracer.eye = Point3f(13.f, 2.f, 3.f);
    raytracer.lookat = Point3f(0.f, 0.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void quad_mesh() {
    Image image(400, 400, Image::RGB);
    HittableList world;

    auto left_red = make_shared<Lambertian>(Color3f(1.0, 0.2, 0.2));
    auto back_green = make_shared<Lambertian>(Color3f(0.2, 1.0, 0.2));
    auto right_blue = make_shared<Lambertian>(Color3f(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(Color3f(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<Lambertian>(Color3f(0.2, 0.8, 0.8));

    world.add(make_shared<Quad>(Point3f(-3,-2, 5), Vec3f(0, 0,-4), Vec3f(0, 4, 0), left_red));
    world.add(make_shared<Mesh>(Point3f(-2,-2, 0), Vec3f(4, 0, 0), Vec3f(0, 4, 0), back_green));
    world.add(make_shared<Quad>(Point3f( 3,-2, 1), Vec3f(0, 0, 4), Vec3f(0, 4, 0), right_blue));
    world.add(make_shared<Quad>(Point3f(-2, 3, 1), Vec3f(4, 0, 0), Vec3f(0, 0, 4), upper_orange));
    world.add(make_shared<Mesh>(Point3f(-2,-3, 5), Vec3f(4, 0, 0), Vec3f(0, 0,-4), lower_teal));

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;

    raytracer.fovY = 80.f;
    raytracer.eye = Point3f(0.f, 0.f, 9.f);
    raytracer.lookat = Point3f(0.f, 0.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

int main() {
    switch (5) {
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
        case 4: perlin_spheres(); break;
        case 5: quad_mesh(); break;
    }

    return 0;
}