#ifndef TRI_H
#define TRI_H

#include "utilities.h"
#include "hittable.h"

class tri : public hittable {
public:
        tri(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) 
            : Q(Q), u(u), v(v), mat(mat)
        {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n, n);

            set_bounding_box();
        }

        virtual void set_bounding_box() {
            // Compute the bounding box of all four vertices.
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());
            
            // No hit if the ray is parallel to the plane (denom of solution is 0)
            if (std::fabs(denom) < 1e-8) {
                return false;
            }

            // Return false if the hit point parameter t is outside the ray interval.
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t)) {
                return false;
            }

            // Determine if the hit point lies within the planar shape using its plane coordinates.
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec)) {
                return false;
            }

            // Ray hits the 2D shape; set the rest of the hit record and return true.
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

    private:
        point3 Q;
        vec3 u, v; 
        vec3 w;       // Needed for determining if intersection w/ plane is inside quad
        shared_ptr<material> mat;
        aabb bbox;
        vec3 normal;  // Normal vector to this plane
        double D;     // Value of D in equation for a plane: Ax + By + Cz = D

        bool is_interior(double alpha, double beta, hit_record& rec) const {
            // Given the hit point in plane coordinates, return false if it is outside the
            // primitive, otherwise set the hit record UV coordinates and return true.
            if (alpha > 0 && beta > 0 && (alpha + beta) < 1) {
                rec.u = alpha;
                rec.v = beta;
                return true;
            }
            return false;
        }
};

#endif