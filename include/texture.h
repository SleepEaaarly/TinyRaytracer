#pragma once

#include "rtweekend.h"
#include "perlin.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual Color3f value(float u, float v, const Point3f& p) const = 0;

};

class SolidColor : public Texture {
public:
    SolidColor(const Color3f& albedo) : albedo(albedo) {}
    SolidColor(float red, float green, float blue) : albedo(Color3f(red, green, blue)) {}
    
    Color3f value(float u, float v, const Point3f& p) const override {
        return albedo;
    }

private:
    Color3f albedo;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(float scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
        : inv_scale(1.f / scale), even(even), odd(odd) {}
    
    CheckerTexture(float scale, const Color3f& c1, const Color3f& c2)
        : inv_scale(1.f / scale), even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}
    
    Color3f value(float u, float v, const Point3f& p) const override {
        auto x_int = static_cast<int>(floorf(inv_scale * p.x));
        auto y_int = static_cast<int>(floorf(inv_scale * p.y));
        auto z_int = static_cast<int>(floorf(inv_scale * p.z));
        // auto pos_int = static_cast<int>(floorf(inv_scale * (p.x + p.y + p.z)));     // stripe

        bool isEven = (x_int + y_int + z_int) % 2 == 0;     // checker
        // bool isEven = pos_int % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    float inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class ImageTexture : public Texture {
private:
    shared_ptr<Image> image;
public:
    ImageTexture(const char* filename) : image(make_shared<Image>(filename)) {}

    Color3f value(float u, float v, const Point3f& p) const override {
        if (image->get_height() <= 0)   return Color3f(0.f, 1.f, 1.f);

        u = Interval(0.f, 1.f).clamp(u);
        v = 1.f - Interval(0.f, 1.f).clamp(v);
        auto i = static_cast<int>(u * image->get_width());
        auto j = static_cast<int>(v * image->get_height());
        Color pixel = image->get(i, j);

        return color2Vec(pixel).cutVec3();
    }
};

class NoiseTexture : public Texture {
private:
    Perlin noise;
public:
    NoiseTexture() {}

    Color3f value(float u, float v, const Point3f& p) const {
        return Color3f(1.f, 1.f, 1.f) * noise.noise(p);
    }

};
