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

        scattered = Ray(rec.p, scatter_direction, r_in.time());
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
        scattered = Ray(rec.p, refl, r_in.time());
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0.f);
    }
};

class Dielectric : public Material {
private:
    // in fact it's relative refraction_index, which is material refractive index over media.
    float refraction_index;
public:
    Dielectric(float refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, Color3f &attenuation, Ray &scattered)
    const override {
        attenuation = Color3f(1.f, 1.f, 1.f);
        float etai_over_etao = rec.front_face ? (1.f / refraction_index) : refraction_index;

        auto ray_in_direction = r_in.direction();

        float cos_theta = dot(-ray_in_direction, rec.normal);
        float sin_theta = sqrtf(1.f - cos_theta*cos_theta);

        Vec3f scattered_direction;
        bool total_reflection = etai_over_etao * sin_theta > 1.f;

        if (total_reflection || random_float() < schlick(cos_theta, etai_over_etao))   // total reflection
            scattered_direction = reflect(ray_in_direction, rec.normal);
        else 
            scattered_direction = refract(ray_in_direction, rec.normal, etai_over_etao);

        scattered = Ray(rec.p, scattered_direction, r_in.time());
        return true;
    } 
};

#endif