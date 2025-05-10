#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray &r_in, const HitRecord &rec, Color &attenuation, Ray &scattered 
    ) const = 0;
};

#endif