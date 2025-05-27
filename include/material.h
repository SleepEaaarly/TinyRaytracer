#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "pdf.h"

class ScatterRecord {
public:
    Color3f attenuation;
    shared_ptr<PDF> pdf_ptr;
    bool skip_pdf;
    Ray skip_pdf_ray;
};

class Material {
public:
    virtual ~Material() = default;

    virtual Color3f emit(const Ray& r_in, const HitRecord& rec, float u, float v, const Point3f& p) const {
        return Color3f(0.f, 0.f, 0.f);
    }

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, ScatterRecord& srec
    ) const {
        srec.attenuation = Color3f(0.f, 0.f, 0.f);
        return false;
    }

    virtual float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const {
        return 0.f;
    }
};

class Lambertian : public Material {
private:
    shared_ptr<Texture> tex;
public:
    Lambertian(const Color3f &albedo) : tex(make_shared<SolidColor>(albedo)) {}
    Lambertian(const Color &albedo) : Lambertian(color2Vec(albedo).cutVec3()) {}
    Lambertian(shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord& srec) 
    const override {
        // auto scatter_direction = normal_to_world_dir(random_cosine_direction(), rec.normal);
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<CosinePDF>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const override {
        auto cos_theta = dot(rec.normal, scattered.direction());
        return cos_theta < 0.f ? 0.f : cos_theta / pi;
    }
};

class Metal : public Material {
private:
    Color3f albedo;
    float fuzz;
public:
    Metal(const Color3f &albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1.f ? fuzz : 1.f) {}
    Metal(const Color &albedo, float fuzz) : albedo(color2Vec(albedo).cutVec3()), fuzz(fuzz < 1.f ? fuzz : 1.f) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord& srec) 
    const override {
        Vec3f refl = reflect(r_in.direction(), rec.normal);
        refl = refl.unit() + fuzz * random_unit_vector();
        bool will_scatter = dot(refl, rec.normal) > 0.f;
        if (!will_scatter) {
            srec.attenuation = Color3f(0.f, 0.f, 0.f);
            return false;
        } 
        srec.attenuation = albedo;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        srec.skip_pdf_ray = Ray(rec.p, refl, r_in.time());

        return true;
    }
};

class Dielectric : public Material {
private:
    // in fact it's relative refraction_index, which is material refractive index over media.
    float refraction_index;
public:
    Dielectric(float refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord& srec)
    const override {
        srec.attenuation = Color3f(1.f, 1.f, 1.f);
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
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

        srec.skip_pdf_ray = Ray(rec.p, scattered_direction, r_in.time());
        return true;
    } 
};

class DiffuseLight : public Material {
private:
    shared_ptr<Texture> tex;
public:
    DiffuseLight(shared_ptr<Texture> tex) : tex(tex) {}
    DiffuseLight(const Color3f& emit) : tex(make_shared<SolidColor>(emit)) {}

    Color3f emit(const Ray& r_in, const HitRecord& rec, float u, float v, const Point3f& p) const override {
        if (!rec.front_face)
            return Color3f(0.f, 0.f, 0.f);
        return tex->value(u, v, p);
    }
};

class Isotropic : public Material {
private:
    shared_ptr<Texture> tex;
public:
    Isotropic(const Color3f& albedo) : tex(make_shared<SolidColor>(albedo)) {}
    Isotropic(shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord& srec) 
    const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<SpherePDF>();
        srec.skip_pdf = false;

        return true;
    }

    float scattering_pdf(const Ray& r_in, const HitRecord& rec, const Ray& scattered) const override {
        return 1.f / (4 * pi);
    }
};

#endif