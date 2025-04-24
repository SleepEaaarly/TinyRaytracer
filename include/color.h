#ifndef COLOR_H
#define COLOR_H

#include "geometry.h"

using Color = Vec4uc;

Color vec4f2Color(Vec4f &vec) {
    float r, g, b, a;
    r = std::max(0.f, std::min(1.f, vec.r));
    g = std::max(0.f, std::min(1.f, vec.g));
    b = std::max(0.f, std::min(1.f, vec.b));
    a = std::max(0.f, std::min(1.f, vec.a));
    return Color((unsigned char)(r*255+.5f), (unsigned char)(g*255+.5f), (unsigned char)(b*255+.5f), (unsigned char)(a*255+.5f));
}

Vec4f color2Vec4f(Color &c) {
    return Vec4f(c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f);
}

#endif