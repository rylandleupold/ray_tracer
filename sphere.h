#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "utilities.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius, shared_ptr<material> mat) 
            : center(center), radius(fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - (radius * radius);
            auto discriminant = h*h - a*c; // Square root portion of quadratic equation, determines if we have 0, 1, or 2 solutions
            
            if (discriminant < 0) {
                return false;
            }
            
            auto root = ((h - sqrt(discriminant)) / a);
            if (!ray_t.surrounds(root)) {
                root = ((h + sqrt(discriminant)) / a);
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius; // Special way to normalize a vector for spheres only
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;
            return true;
        };
    
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
};

#endif