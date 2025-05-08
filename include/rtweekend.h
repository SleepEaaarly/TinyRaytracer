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

// Common Headers
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "interval.h"

#endif
