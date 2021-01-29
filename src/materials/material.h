#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "../utilities/ray.h"
#include "../geometries/hitable.h"


__device__ float schlick(float cosine, float ref_idx) {
    float r0 = (1.0f-ref_idx) / (1.0f+ref_idx);
    r0 = r0*r0;
    return r0 + (1.0f-r0)*pow((1.0f - cosine),5.0f);
}

__device__ bool refract(const vector3D& v, const vector3D& n, float ni_over_nt, vector3D& refracted) {
    vector3D uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

#define RANDvector3D vector3D(curand_uniform(local_rand_state),curand_uniform(local_rand_state),curand_uniform(local_rand_state))

__device__ vector3D random_in_unit_sphere(curandState *local_rand_state) {
    vector3D p;
    do {
        p = 2.0f*RANDvector3D - vector3D(1,1,1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

__device__ vector3D reflect(const vector3D& v, const vector3D& n) {
     return v - 2.0f*dot(v,n)*n;
}

class material  {
    public:
        __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vector3D& attenuation, ray& scattered, curandState *local_rand_state) const = 0;
};

class lambertian : public material {
    public:
        __device__ lambertian(const vector3D& a) : albedo(a) {}
        __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vector3D& attenuation, ray& scattered, curandState *local_rand_state) const  {
             vector3D target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
             scattered = ray(rec.p, target-rec.p);
             attenuation = albedo;
             return true;
        }

        vector3D albedo;
};

class metal : public material {
    public:
        __device__ metal(const vector3D& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
        __device__ virtual bool scatter(const ray& r_in, const hit_record& rec, vector3D& attenuation, ray& scattered, curandState *local_rand_state) const  {
            vector3D reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(local_rand_state));
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0.0f);
        }
        vector3D albedo;
        float fuzz;
};

class dielectric : public material {
public:
    __device__ dielectric(float ri) : ref_idx(ri) {}
    __device__ virtual bool scatter(const ray& r_in,
                         const hit_record& rec,
                         vector3D& attenuation,
                         ray& scattered,
                         curandState *local_rand_state) const  {
        vector3D outward_normal;
        vector3D reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vector3D(1.0, 1.0, 1.0);
        vector3D refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0.0f) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1.0f - ref_idx*ref_idx*(1-cosine*cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx);
        else
            reflect_prob = 1.0f;
        if (curand_uniform(local_rand_state) < reflect_prob)
            scattered = ray(rec.p, reflected);
        else
            scattered = ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
};
#endif
