#pragma once

#include "hittable.h"
#include "material.h"
#include "texture.h"

class ConstantMedium : public Hittable {
private:
    shared_ptr<Hittable> boundary;
    float neg_inv_density;
    shared_ptr<Material> phase_function;
public:
    ConstantMedium(shared_ptr<Hittable> boundary, float density, shared_ptr<Texture> tex)
        : boundary(boundary), neg_inv_density(-1.f / density), 
          phase_function(make_shared<Isotropic>(tex)) {}
    ConstantMedium(shared_ptr<Hittable> boundary, float density, const Color3f& albedo) 
        : boundary(boundary), neg_inv_density(-1.f / density),
          phase_function(make_shared<Isotropic>(albedo)) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        HitRecord rec1, rec2;
        
        // 注意这里先用无限区间判断是否与边界相交，再用ray_t区间约束交点
        if (!boundary->hit(r, Interval::universe, rec1)) 
            return false;
        if (!boundary->hit(r, Interval(rec1.t+0.0001f, INFINITY), rec2)) 
            return false;
        
        if (rec1.t < ray_t.min) rec1.t = ray_t.min;
        if (rec2.t > ray_t.max) rec2.t = ray_t.max;    

        if (rec1.t >= rec2.t)
            return false;
        if (rec1.t < 0.f)
            rec1.t = 0.f;
        
        // 模型概率散射，体积云越厚(dis...越大)以及密度越大(neg_...越小)，散射概率越大(false概率越小)
        auto distance_inside_boundary = rec2.t - rec1.t;
        auto hit_distance = neg_inv_density * std::log(random_float());
        if (distance_inside_boundary < hit_distance)
            return false;
        
        rec.t = rec1.t + hit_distance;
        rec.p = r.at(rec.t);

        rec.normal = Vec3f(1.f, 0.f, 0.f);  // arbitrary
        rec.front_face = true;
        rec.mat = phase_function;

        return true;
    }

    aabb bounding_box() const override {
        return boundary->bounding_box();
    }

    void translate(const Vec3f& offset) override {
        boundary->translate(offset);
    }

    void rotate_y(float theta) override {
        boundary->rotate_y(theta);
    }
};
