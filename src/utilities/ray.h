#ifndef RAYH
#define RAYH
#include "../math/vector3D.h"

class ray
{
    public:
        __device__ ray() {}
        __device__ ray(const vector3D& a, const vector3D& b) { A = a; B = b; }
        __device__ vector3D origin() const       { return A; }
        __device__ vector3D direction() const    { return B; }
        __device__ vector3D point_at_parameter(float t) const { return A + t*B; }

        vector3D A;
        vector3D B;
};

#endif
