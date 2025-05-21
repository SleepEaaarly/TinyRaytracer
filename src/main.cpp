#include "rtweekend.h"
#include "hittable_list.h"
#include "raytracer.h"
#include "Sphere.h"
#include "image.h"
#include "material.h"
#include "bvh.h"
#include "Quad.h"
#include "mesh.h"
#include "constant_medium.h"
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

void cornell_box() {
    Image image(600, 600, Image::RGB);

    HittableList world;
    auto red = make_shared<Lambertian>(Color3f(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color3f(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color3f(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color3f(15, 15, 15));

    world.add(make_shared<Quad>(Point3f(555,0,0), Vec3f(0,555,0), Vec3f(0,0,555), green));
    world.add(make_shared<Quad>(Point3f(0,0,0), Vec3f(0,555,0), Vec3f(0,0,555), red));
    world.add(make_shared<Quad>(Point3f(343, 554, 332), Vec3f(-130,0,0), Vec3f(0,0,-105), light));
    world.add(make_shared<Quad>(Point3f(0,0,0), Vec3f(555,0,0), Vec3f(0,0,555), white));
    world.add(make_shared<Quad>(Point3f(555,555,555), Vec3f(-555,0,0), Vec3f(0,0,-555), white));
    world.add(make_shared<Quad>(Point3f(0,0,555), Vec3f(555,0,0), Vec3f(0,555,0), white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 330, 165), white);
    box1->rotate_y(15);
    box1->translate(Vec3f(265, 0, 295));
    world.add(box1);
    
    shared_ptr<Hittable> box2 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 165, 165), white);
    box2->rotate_y(-18);
    box2->translate(Vec3f(130, 0, 65));
    world.add(box2);

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 64;
    raytracer.max_depth = 10;
    raytracer.background = Color3f(0.0f, 0.0f, 0.0f);

    raytracer.fovY = 40.f;
    raytracer.eye = Point3f(278.f, 278.f, -800.f);
    raytracer.lookat = Point3f(278.f, 278.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void box() {
    Image image(600, 600, Image::RGB);

    HittableList world;
    auto red = make_shared<Lambertian>(Color3f(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color3f(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color3f(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color3f(15, 15, 15));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 330, 165), white);
    box1->translate(Vec3f(265, 0, 295));
    box1->rotate_y(15);
    world.add(box1);
    
    shared_ptr<Hittable> box2 = make_shared<Box>(Point3f(0, 0, 0), Point3f(165, 165, 165), white);
    box2->translate(Vec3f(130, 0, 65));
    box2->rotate_y(-18);
    world.add(box2);

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 30;
    raytracer.max_depth = 10;
    raytracer.background = Color3f(0.7f, 0.8f, 1.f);

    raytracer.fovY = 40.f;
    raytracer.eye = Point3f(278.f, 278.f, -800.f);
    raytracer.lookat = Point3f(278.f, 278.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");
}

void final_scene() {
    Image image(800, 800, Image::RGB);

    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color3f(0.48, 0.83, 0.53));
    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_float(1,101);
            auto z1 = z0 + w;
            boxes1.add(make_shared<Box>(Point3f(x0,y0,z0), Point3f(x1,y1,z1), ground));
        }
    }

    HittableList world;
    world.add(make_shared<BVHNode>(boxes1));
    auto light = make_shared<DiffuseLight>(Color3f(7, 7, 7));
    world.add(make_shared<Quad>(Point3f(123,554,147), Vec3f(300,0,0), Vec3f(0,0,265), light));
    auto center1 = Point3f(400, 400, 200);
    auto center2 = center1 + Vec3f(30,0,0);
    auto sphere_material = make_shared<Lambertian>(Color3f(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphere_material));
    world.add(make_shared<Sphere>(Point3f(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.add(make_shared<Sphere>(
        Point3f(0, 150, 145), 50, make_shared<Metal>(Color3f(0.8, 0.8, 0.9), 1.0)
    ));
    auto boundary = make_shared<Sphere>(Point3f(360,150,145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2f, Color3f(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3f(0,0,0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, .0001f, Color3f(1,1,1)));
    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("../images/earthmap.jpg"));
    world.add(make_shared<Sphere>(Point3f(400,200,400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point3f(220,280,300), 80, make_shared<Lambertian>(pertext)));
    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color3f(.73, .73, .73));
    int ns = 1000;

    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(random_vector(0, 165), 10.f, white));
    }

    boxes2.rotate_y(15);
    boxes2.translate(Vec3f(-100, 270, 395));
    world.add(make_shared<BVHNode>(boxes2));

    RayTracer raytracer(image);
    raytracer.samples_per_pixel = 500;
    raytracer.max_depth = 40;
    raytracer.background = Color3f(0.f, 0.f, 0.f);

    raytracer.fovY = 40.f;
    raytracer.eye = Point3f(478.f, 278.f, -600.f);
    raytracer.lookat = Point3f(278.f, 278.f, 0.f);
    
    raytracer.defocus_angle = 0.f;

    auto start = std::chrono::steady_clock::now();
    raytracer.render(BVHNode(world));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<double>(end - start).count();
    std::cout << "Raytracing time consumption: " << duration << " secs" << std::endl;

    image.write_png_file("rst.png");

}

int main() {
    switch (7) {
        case 1: bouncing_spheres(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
        case 4: perlin_spheres(); break;
        case 5: quad_mesh(); break;
        case 6: box(); break;
        case 7: cornell_box(); break;
        case 9: final_scene(); break;
    }

    return 0;
}