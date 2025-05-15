#ifndef INTERVAL_H
#define INTERVAL_H

#include <cmath>

class Interval {
public:
    float min, max;
        
    Interval() : min(INFINITY), max(-INFINITY) {}   
    Interval(float min, float max) : min(min), max(max) {}
    Interval(const Interval& a, const Interval& b) {
        min = a.min < b.min ? a.min : b.min;
        max = a.max < b.max ? b.max : a.max;
    }

    float size() const {
        return max - min;
    }

    bool contains(float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const {
        return min < x && x < max;
    }

    float clamp(float x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    Interval expand(float delta) const {
        auto padding = delta / 2.f;
        return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
};

#endif
