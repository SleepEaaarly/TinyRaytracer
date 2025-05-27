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
#include "path.h"

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

inline int random_int(int min, int max) {
    static std::uniform_int_distribution<int> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vec3f random_vector() {
    return Vec3f(random_float(), random_float(), random_float());
}

inline Vec3f random_vector(float min, float max) {
    return Vec3f(random_float(min, max), random_float(min, max), random_float(min, max));
}

inline Vec3f random_in_unit_disk() {
    float theta = degrees_to_radians(random_float(0.f, 360.f));
    return Vec3f(cosf(theta), sinf(theta), 0.f);
}

// rejection method
// inline Vec3f random_unit_vector() {
//     // uniform sample in unit sphere
//     Vec3f sample_vec;
//     float vec_lensq;
//     do {
//         sample_vec = random_vector(-1.f, 1.f);
//         vec_lensq = sample_vec.norm_squared();
//     } while (vec_lensq < 1e-16 && vec_lensq > 1.f);

//     return sample_vec.unit();
// }

// inversion method
inline Vec3f random_unit_vector() {
    auto r1 = random_float();
    auto r2 = random_float();

    auto phi = 2*pi*r1;
    auto x = std::cos(phi)*2*std::sqrt(r2*(1-r2));
    auto y = std::sin(phi)*2*std::sqrt(r2*(1-r2));
    auto z = 1 - 2*r2;
    return Vec3f(x, y, z);
}

inline Vec3f random_cosine_direction() {
    auto r1 = random_float();
    auto r2 = random_float();

    auto phi = 2*pi*r1;
    auto x = std::cos(phi)*std::sqrt(r2);
    auto y = std::sin(phi)*std::sqrt(r2);
    auto z = std::sqrt(1-r2);

    return Vec3f(x, y, z);
}

// ensure pars are unit vectors
inline Vec3f reflect(const Vec3f &v, const Vec3f &n) {
    return v - 2 * dot(v, n) * n;
}

// ensure pars are unit vectors
inline Vec3f refract(const Vec3f &v, const Vec3f &n, float etai_over_etao) {
    auto cos_theta = dot(-v, n);
    auto r_out_perp = etai_over_etao * (v + cos_theta*n);
    auto r_out_parallel = -sqrtf(1 - dot(r_out_perp, r_out_perp)) * n;
    return r_out_perp + r_out_parallel;
}

inline float schlick(float cosine, float eta) {
    float f0 = (1 - eta) / (1 + eta);
    f0 = f0*f0;
    // (1-cosine)^5
    float x = 1 - cosine;
    float x2 = x * x;
    x = x2 * x2 * x;
    return f0 + (1-f0)*x;
}

inline Vec3f linear_to_gamma(const Vec3f &linear_component) {
    // approximate gamma = 2
    float gamma_r = linear_component.x > 0.f ? sqrtf(linear_component.x) : 0.f;
    float gamma_g = linear_component.y > 0.f ? sqrtf(linear_component.y) : 0.f;
    float gamma_b = linear_component.z > 0.f ? sqrtf(linear_component.z) : 0.f;
    return Vec3f(gamma_r, gamma_g, gamma_b);
}

inline Vec3f normal_to_world_dir(const Vec3f& v, const Vec3f& n) {
    Vec3f a = std::fabs(n.x) <= 0.9f ? Vec3f(1.f, 0.f, 0.f) : Vec3f(0.f, 1.f, 0.f);
    auto axis_y = cross(n, a).unit();
    auto axis_x = cross(axis_y, n);

    return v.x * axis_x + v.y * axis_y + v.z * n;
}

inline Vec3f normal_to_world_dir(const Vec3f& v, const Vec3f& axis_x, const Vec3f& axis_y, const Vec3f& n) {
    return v.x * axis_x + v.y * axis_y + v.z * n;
}

#endif
