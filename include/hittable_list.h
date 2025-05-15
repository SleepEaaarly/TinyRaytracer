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
};

#endif 
