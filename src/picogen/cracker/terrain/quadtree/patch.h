#ifndef PATCH_H_INCLUDED_20110826
#define PATCH_H_INCLUDED_20110826

#include "../intersection.h"

namespace picogen { namespace cracker {
        class Ray;
} }

namespace picogen { namespace cracker { namespace detail {

        class Patch {
        public:
                Patch ()                         = delete;
                Patch (Patch const &)            = delete;
                Patch& operator= (Patch const &) = delete;


                Patch (real left, real right,
                       real front, real back,
                       unsigned int res_x,
                       unsigned int res_z,
                       std::function<real (real, real)> fun,
                       real &y_min, real &y_max);
                ~Patch();
                Intersection::Optional operator() (Ray const &ray,
                                                   real min, real max) const;

        private:
                real left_, right_, front_, back_;
                unsigned int res_x_, res_z_, stride_;
                Vector *h_;

                std::shared_ptr<Material> material_;


                template <int step_x, int step_z>
                Intersection::Optional intersect (Ray const &ray,
                                                  int left, int right,
                                                  int front, int back) const;

                Intersection::Optional intersect_quad (Ray const &ray, int X, int Z) const ;
                Intersection::Optional intersect_amanatides (Ray const &ray,
                                                   real minT,
                                                   real maxT) const;

                Vector const * ph (unsigned int x, unsigned z) const;
                Vector& h (unsigned int x, unsigned z);
        };

} } }

#endif // PATCH_H_INCLUDED_20110826
