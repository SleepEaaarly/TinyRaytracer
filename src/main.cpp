#include "geometry.h"

int main() {
    Vec3f v(1.f, 2.f, 3.f);
    Vec3i u(1, 2, 3);
    Vec3i w(1, 1, 1);
    Vec4f a;

    std::cout << ((v+=Vec3f(4.f, 2.f, 1.f)).unit() /= 3) << std::endl;

    std::cout << v << std::endl;

    std::cout << (u-w)/2 << std::endl;

    return 0;
}