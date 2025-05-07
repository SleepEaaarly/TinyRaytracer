#ifndef COLOR_H
#define COLOR_H

#include "geometry.h"

struct Color {
	union {
		struct {
			unsigned char r, g, b, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	Color() : val(0), bytespp(1) {
	}

	Color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : r(R), g(G), b(B), a(A), bytespp(4) {
	}

	Color(int v, int bpp) : val(v), bytespp(bpp) {
	}

	Color(const Color &c) : val(c.val), bytespp(c.bytespp) {
	}

	Color(const unsigned char *p, int bpp) : val(0), bytespp(bpp) {
		for (int i=0; i<bpp; i++) {
			raw[i] = p[i];
		}
	}

	Color & operator =(const Color &c) {
		if (this != &c) {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
};

inline Color vec2Color(const Vec4f &vec) {
    return Color((unsigned char)(std::max(0.f, std::min(1.f, vec.r))*255+.5f),
    (unsigned char)(std::max(0.f, std::min(1.f, vec.g))*255+.5f),
    (unsigned char)(std::max(0.f, std::min(1.f, vec.b))*255+.5f),
    (unsigned char)(std::max(0.f, std::min(1.f, vec.a))*255+.5f));
}

inline Color vec2Color(const Vec3f &vec) {
    return Color((unsigned char)(std::max(0.f, std::min(1.f, vec.x))*255+.5f),
    (unsigned char)(std::max(0.f, std::min(1.f, vec.y))*255+.5f),
    (unsigned char)(std::max(0.f, std::min(1.f, vec.z))*255+.5f),
    255);
}

inline Vec4f color2Vec(const Color &c) {
    return Vec4f(c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f);
}

#endif