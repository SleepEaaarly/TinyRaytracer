#pragma once

#include "rtweekend.h"
#include "hittable.h"

class PDF {
public:
    virtual ~PDF() {}

    virtual float value(const Vec3f& direction) const = 0;
    virtual Vec3f generate() const = 0;
};

class SpherePDF : public PDF {
public:
    SpherePDF() {}

    float value(const Vec3f& direction) const override {
        return 1.f / (4 * pi);
    }

    Vec3f generate() const override {
        return random_unit_vector();
    }
};

class CosinePDF : public PDF {
public:
    CosinePDF(const Vec3f& n) {
        Vec3f a = std::fabs(n.x) <= 0.9f ? Vec3f(1.f, 0.f, 0.f) : Vec3f(0.f, 1.f, 0.f);
        axis_y = cross(n, a).unit();
        axis_x = cross(axis_y, n);
        normal = n;
    }

    float value(const Vec3f& direction) const override {
        auto cosine_theta = dot(direction.unit(), normal);
        return cosine_theta > 0.f ? 0.f : cosine_theta;
    }

    Vec3f generate() const override {
        return normal_to_world_dir(random_cosine_direction(), axis_x, axis_y, normal);
    }

private:
    Vec3f axis_x;
    Vec3f axis_y;
    Vec3f normal;
};

class HittablePDF : public PDF {
public:
    HittablePDF(const Hittable& objects, const Point3f& origin)
        : objects(objects), origin(origin) {}

    float value(const Vec3f& direction) const override {
        return objects.pdf_value(origin, direction);
    }

    Vec3f generate() const override {
        return objects.random(origin);
    }

private:
    const Hittable& objects;
    Point3f origin;
};

class MixturePDF : public PDF {
public:
    MixturePDF(shared_ptr<PDF> p0, shared_ptr<PDF> p1) {
        p[0] = p0;
        p[1] = p1;
    }

    // 求pdf值采用线性加权
    float value(const Vec3f& direction) const override {
        return 0.5f * p[0]->value(direction) + 0.5f * p[1]->value(direction);
    }

    // 采样采用随机生成
    Vec3f generate() const override {
        if (random_float() < 0.5f)
            return p[0]->generate();
        else 
            return p[1]->generate();
    }

private:
    shared_ptr<PDF> p[2];
};
