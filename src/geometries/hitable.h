#ifndef HITABLEH
#define HITABLEH

#include "../utilities/ray.h"

class material;

struct hit_record
{
    float t;
    vector3D p;
    vector3D normal;
    material *mat_ptr;
};

class hitable  {
    public:
        __device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
