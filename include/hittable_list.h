#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;
    aabb bbox;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { 
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    bool isEmpty() const {
        return objects.size() == 0;
    }

    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest = ray_t.max;
        for (const auto &object : objects) {
            if (object->hit(r, Interval(ray_t.min, closest), temp_rec)) {
                hit_anything = true;
                closest = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    aabb bounding_box() const override {
        return bbox;
    }

    void translate(const Vec3f& offset) override {
        for (auto &object : objects) {
            object->translate(offset);
        }
        bbox = bbox + offset;
    }

    void rotate_y(float theta) override {
        aabb bbox;
        for (auto& object : objects) {
            object->rotate_y(theta);
            bbox = aabb(bbox, object->bounding_box());
        }
    }

    virtual float pdf_value(const Point3f& origin, const Vec3f& direction) const {
        if (objects.size() == 0) {
            std::cout << "No objects in lights!" << std::endl;
            return 0.f;
        }
        auto weight = 1.f / objects.size();
        auto sum = 0.f;
        for (const auto& object : objects) {
            sum += weight * object->pdf_value(origin, direction);
        }
        return sum;
    }

    virtual Vec3f random(const Point3f& origin) const {
        auto obj_size = objects.size();
        if (obj_size == 0)
            return Vec3f(1.f, 0.f, 0.f);

        return objects[random_int(0, obj_size - 1)]->random(origin);
    }
};

#endif 
