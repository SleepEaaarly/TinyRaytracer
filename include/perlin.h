#pragma once

#include "rtweekend.h"

class Perlin {
private:
    static const int point_count = 256;
    float randfloat[point_count];
    int perm_x[point_count];
    int perm_y[point_count];
    int perm_z[point_count];

    static void perlin_generate_perm(int* p) {
        for (int i = 0; i < point_count; ++i) 
            p[i] = i;
        permute(p, point_count);   
    }

    static void permute(int* p, int n) {
        for (int i = n-1; i > 0; --i) {
            int target = random_int(0, i);
            std::swap(p[i], p[target]);
        }
    }

public:
    Perlin() {
        for (int i = 0; i < point_count; ++i)
            randfloat[i] = random_float();
        perlin_generate_perm(perm_x);
        perlin_generate_perm(perm_y);
        perlin_generate_perm(perm_z);
    }

    float noise(const Point3f& p) const {
        int x = int(p.x * 4) & 255;
        int y = int(p.y * 4) & 255;
        int z = int(p.z * 4) & 255;
        return randfloat[perm_x[x] ^ perm_y[y] ^ perm_z[z]];
    }
};
