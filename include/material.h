#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Vec3f &attenuation, Ray &scattered 
    ) const = 0;
};

class Lambertian : public Material {
private:
    Color3f albedo;
public:
    Lambertian(const Color3f &albedo) : albedo(albedo) {}
    Lambertian(const Color &albedo) : albedo(color2Vec(albedo).cutVec3()) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color3f &attenuation, Ray &scattered) 
    const override {
        auto scatter_direction = random_unit_vector() + rec.normal;
        
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material {
private:
    Color3f albedo;
    float fuzz;
public:
    Metal(const Color3f &albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1.f ? fuzz : 1.f) {}
    Metal(const Color &albedo, float fuzz) : albedo(color2Vec(albedo).cutVec3()), fuzz(fuzz < 1.f ? fuzz : 1.f) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color3f &attenuation, Ray &scattered) 
    const override {
        Vec3f refl = reflect(r_in.direction(), rec.normal);
        refl = refl.unit() + fuzz * random_unit_vector();
        scattered = Ray(rec.p, refl);
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0.f);
    }
};

#endif