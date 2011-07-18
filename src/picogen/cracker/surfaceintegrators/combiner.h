#ifndef COMBINER_H_20110713
#define COMBINER_H_20110713

#include "color.h"
#include <string>

namespace picogen { namespace cracker {

class Ray;
class Scene;

template <typename IntegratorA, typename IntegratorB>
class CombinerIntegrator {
public:
        CombinerIntegrator() = delete;
        CombinerIntegrator(IntegratorA a, IntegratorB b, real lerp)
                : a(a), b(b), lerp_(lerp)
        {
                assert(this->lerp_>=0 && this->lerp_<=1);
        }

        Color operator() (Ray const &ray, Scene const &scene) const {
                if (lerp_<=0) return a(ray,scene);
                if (lerp_>=1) return b(ray,scene);
                return (1-lerp_)*a(ray,scene)
                     + lerp_*b(ray,scene);
        }

        real lerp() const {
                return lerp_;
        }

private:
        IntegratorA a;
        IntegratorB b;
        real lerp_;
};

template <typename IntA, typename IntB>
inline
CombinerIntegrator<IntA, IntB> combine (IntA a, IntB b, real lerp) {
        return CombinerIntegrator<IntA,IntB>(a,b, lerp);
}

} }

#endif // COMBINER_H_20110713
