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
public:
    Metal(const Color3f &albedo) : albedo(albedo) {}
    Metal(const Color &albedo) : albedo(color2Vec(albedo).cutVec3()) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color3f &attenuation, Ray &scattered) 
    const override {
        Vec3f scatter_direction = reflect(r_in.direction(), rec.normal);

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;

        return true;
    }
};

#endif