#ifndef SPHERE_H_20110711
#define SPHERE_H_20110711

#include "intersection.h"
#include "ray.h"
#include "real.h"
#include "primitives/primitive.h"
#include "math3d.h"
#include "algorithm.h"
#include "materials/specularmirror.h"
#include "materials/lambertmaterial.h"

namespace picogen { namespace cracker {

class Sphere : public Primitive {
public:
        Sphere() = delete;
        Sphere (Point center, real radius) :
                center_(center), radius_(radius)
        {}

        Intersection::Optional operator() (Ray const &ray) const {
                return intersect(ray);
        }

private:
        Intersection::Optional intersect (Ray const &ray) const {
                /*if (ray.direction().x()<0) {
                        const Intersection isect(std::fabs(100*ray.direction().x()));
                        return isect;
                }*/
                const Vector    &dst = ray.origin() - center_;

                const real         B = mixed_dot (dst, ray.direction()),
                                   C = dot(dst,dst) - radius_*radius_,
                                   D = B*B - C,
                                   E = -B - sqrt (D);

                if ((D>=0) & (E>=0)) {
                        const Point &poi = ray(E);
                        const Normal n = normalize<Normal>(poi - center_);
                        const std::tuple<Normal,Normal,Normal>
                                 cs = coordinateSystem(n);
                        DifferentialGeometry dg(
                                n, n,
                                std::get<0>(cs),
                                std::get<2>(cs)
                        );
                        return Intersection (E, n,
                                             std::shared_ptr<Material>(
                                                 new LambertMaterial()),
                                             dg
                                             );
                }


                return Intersection::Optional();
        }

        Point center_;
        real radius_;
};

} }

#endif // SPHERE_H_20110711
