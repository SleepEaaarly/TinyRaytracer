#ifndef PATH_H
#define PATH_H

class Path {
public:
    Vec3f src;
    Vec3f dst;

    Path() = default;
    // static
    Path(const Vec3f &src) : src(src), dst(src) {}
    // moving
    Path(const Vec3f &src, const Vec3f &dst) : src(src), dst(dst) {}

    Point3f at(float t) const {
        return src + t * (dst - src);
    }
};

#endif
