#include "geometry.h"
#include "ray.h"
#include "color.h"
#include "raytracer.h"
#include "image.h"

int main() {
    Image image(800, 800, Image::RGB);
    
    RayTracer raytracer(image);
    raytracer.render();

    image.write_png_file("rst.png");

    return 0;
}