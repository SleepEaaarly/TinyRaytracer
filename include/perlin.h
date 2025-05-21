#pragma once

#include "rtweekend.h"

class Perlin {
private:
    static const int point_count = 256;
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

    static float grad(int hash, float x, float y, float z) {
        switch (hash & 0xF) {
            case 0x0: return x+y;
            case 0x1: return -x+y;
            case 0x2: return x-y;
            case 0x3: return -x-y;
            case 0x4: return x+z;
            case 0x5: return -x+z;
            case 0x6: return x-z;
            case 0x7: return -x-z;
            case 0x8: return y+z;
            case 0x9: return -y+z;
            case 0xA: return y-z;
            case 0xB: return -y-z;
            case 0xC: return y+x;
            case 0xD: return -y+z;
            case 0xE: return y-x;
            case 0xF: return -y-z;
            default: return 0.f; // never happens
        }
    }

    static float fade(float x) {
        float x2 = x*x;
        return x2*x*(10-15*x+6*x2);
    }

    static float trilinear_interpolate(float c[2][2][2], float u, float v, float w) {
        float rst = 0.f;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    rst += (i*u + (1-i)*(1-u)) *
                           (j*v + (1-j)*(1-v)) * 
                           (k*w + (1-k)*(1-w)) * 
                           c[i][j][k];
                }
            }
        }
        return rst;
    }

public:
    Perlin() {
        perlin_generate_perm(perm_x);
        perlin_generate_perm(perm_y);
        perlin_generate_perm(perm_z);
    }

    float noise(const Point3f& p) const {
        auto X = int(std::floor(p.x));
        auto Y = int(std::floor(p.y));
        auto Z = int(std::floor(p.z));

        float x = p.x - std::floor(p.x);
        float y = p.y - std::floor(p.y);
        float z = p.z - std::floor(p.z);
        
        auto u = fade(x);
        auto v = fade(y);
        auto w = fade(z);

        float c[2][2][2];

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    auto hash = perm_z[(perm_y[(perm_x[(X+i)&255]+Y+j)&255]+Z+k)&255];
                    c[i][j][k] = grad(hash, x-i, y-j, z-k);
                }
            }
        }

        return trilinear_interpolate(c, u, v, w);
    }

    float turb(const Point3f& p, int layer) const {
        auto accum = 0.f;
        auto temp_p = p;
        auto weight = 1.f;

        for (int i = 0; i < layer; ++i) {
            accum += weight * noise(temp_p);
            temp_p *= 2.f;
            weight *= 0.5f;
        }
        return std::fabs(accum);
    }
};
