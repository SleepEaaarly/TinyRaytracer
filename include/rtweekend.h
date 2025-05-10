#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

// C++ std usings
using std::make_shared;
using std::shared_ptr;

// Common Headers
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "interval.h"

// Constants
const float pi = 3.1415927f;

// Utility Functions
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.f;
}

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

inline Vec3f random_unit_vector() {
    // uniform sample in unit sphere
    float theta = degrees_to_radians(random_float(0.f, 180.f));
    float phi = degrees_to_radians(random_float(0.f, 360.f));
    return Vec3f(sinf(theta)*cosf(phi), sinf(theta)*sinf(phi), cosf(theta));
}

inline Vec3f random_on_hemisphere(const Vec3f &normal) {
    // sample in sphere coords(normal coords) and transform it into world coords
    float theta = degrees_to_radians(random_float(0.f, 90.f));
    float phi = degrees_to_radians(random_float(0.f, 360.f));
    Vec3f v(sinf(theta)*cosf(phi), sinf(theta)*sinf(phi), cosf(theta));
    Vec3f up = abs(normal.y) <= 0.999f ? Vec3f(0.f, 1.f, 0.f) : Vec3f(0.f, 0.f, -1.f);
    auto x_axis = cross(up, normal);
    auto y_axis = cross(normal, x_axis);
    return v.x*x_axis + v.y*y_axis + v.z*normal;
}

inline Vec3f linear_to_gamma(const Vec3f &linear_component) {
    // approximate gamma = 2
    float gamma_r = linear_component.x > 0.f ? sqrtf(linear_component.x) : 0.f;
    float gamma_g = linear_component.y > 0.f ? sqrtf(linear_component.y) : 0.f;
    float gamma_b = linear_component.z > 0.f ? sqrtf(linear_component.z) : 0.f;
    return Vec3f(gamma_r, gamma_g, gamma_b);
}

#endif
