#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>

class BVHNode : public Hittable {
public:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    aabb bbox;

    BVHNode(HittableList hittable_list) : BVHNode(hittable_list.objects, 0, hittable_list.objects.size()) {}

    BVHNode(std::vector<shared_ptr<Hittable>> &hittables, int start, int end) {
        auto span = end - start;
        // 当内部只有1(2)个物体时，将子节点设置为这1(2)个物体，同时别忘了在该节点计算物体的包围盒
        if (span == 1) {
            left = hittables[start];
            right = nullptr;
            bbox = left->bounding_box();
        } else if (span == 2) {
            left = hittables[start];
            right = hittables[start + 1];
            bbox = aabb(left->bounding_box(), right->bounding_box());
        } else {
            bbox = aabb::empty;
            for (int i = start; i < end; ++i) {
                bbox = aabb(bbox, hittables[i]->bounding_box());
            }
            auto axis = bbox.longest_axis();
            auto compare = axis == 0 ? x_box_compare :
                           axis == 1 ? y_box_compare :
                                       z_box_compare ;
            std::sort(hittables.begin() + start, hittables.begin() + end, compare);
            auto mid = start + span / 2;
            left = make_shared<BVHNode>(hittables, start, mid);
            right = make_shared<BVHNode>(hittables, mid, end);
        }
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override {
        // 前序遍历：先看本节点bvh，再看子节点是否相交
        if (!bbox.hit(ray, ray_t))  return false;       // bvh 加速点：如果不碰bbox则认为bbox之内的物体都碰不到，则不继续递归
        bool hit1 = left->hit(ray, ray_t, rec);
        if (right != nullptr) {
            bool hit2 = right->hit(ray, Interval(ray_t.min, hit1 ? rec.t : ray_t.max), rec);
            return hit1 || hit2;
        }
        return hit1;
    }

    aabb bounding_box() const override {
        return bbox;
    }

    void translate(const Vec3f& offset) override {
        left->translate(offset);
        right->translate(offset);
        bbox = bbox + offset;
    }

    void rotate_y(float theta) override {
        left->rotate_y(theta);
        right->rotate_y(theta);
        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

private:

    static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) {
        auto a_axis_interval = a->bounding_box().axis_interval(axis);
        auto b_axis_interval = b->bounding_box().axis_interval(axis);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool x_box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool y_box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool z_box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif
