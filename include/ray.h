#ifndef RAY_H
#define RAY_H

#include "geometry.h"

class Ray {
public:
    Ray() = default;
    Ray(const Point3f &origin, const Vec3f &direction) : orig(origin), dir(direction.unit()) {}

    const Point3f& origin() const { return orig; }
    const Vec3f& direction() const { return dir; }

    Point3f at(float t) const {
        return orig + t * dir;
    }

private:
    Point3f orig;
    Vec3f dir;
};

#endif