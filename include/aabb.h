#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
public:
    Interval x, y, z;

    aabb() {}
    
    aabb(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z) {}

    aabb(const Point3f &a, const Point3f &b) {
        x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }

    aabb(const aabb &a, const aabb &b) {
        x = Interval(a.x, b.x);
        y = Interval(a.y, b.y);
        z = Interval(a.z, b.z);
    }

    const Interval& axis_interval(int n) const {
        return n == 0 ? x : n == 1 ? y : z;
    }

    bool hit(const Ray& r, Interval ray_t) const {
        const Point3f& ray_orig = r.origin();
        const Vec3f& ray_dir = r.direction();
        for (int axis = 0; axis < 3; ++axis) {
            const Interval& inter = axis_interval(axis);
            if (fabsf(ray_dir[axis]) < 1e-6f) {
                if (ray_orig[axis] < inter.min || ray_orig[axis] > inter.max) {
                    ray_t = Interval::empty;
                    return false;
                }
                continue;
            } 
            auto adinv = 1.f / ray_dir[axis];
            auto t0 = (inter.min - ray_orig[axis]) * adinv;
            auto t1 = (inter.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {      // Notice: t0 < t1 not ray_dir[axis] < 0 !!!
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }

    int longest_axis() const {
        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else 
            return y.size() > z.size() ? 1 : 2;
    }

    static const aabb empty, universe;
};

#endif 