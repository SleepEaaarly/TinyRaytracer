#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ std usings
using std::make_shared;
using std::shared_ptr;

// Constants

const float pi = 3.1415927;

// Utility Functions
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.f;
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "geometry.h"

#endif
