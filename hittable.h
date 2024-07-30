#ifndef HITTABLE_H
#define HITTABLE_H

#include "utilities.h"
#include "aabb.h"

class material;

class hit_record{
    public:
        point3 p;                  // Point in 3D space where intersection occurs
        vec3 normal;               // Normal vector between p and object surface  
        shared_ptr<material> mat;
        double t;                  // Distance along ray where intersection occurs
        double u;                  // 2D texture coordinate[0]
        double v;                  // 2D texture coordinate[1]
        bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // NOTE: 'outward_normal' is assumed to have unit length
        front_face = dot(r.direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
        virtual aabb bounding_box() const = 0;
};

#endif