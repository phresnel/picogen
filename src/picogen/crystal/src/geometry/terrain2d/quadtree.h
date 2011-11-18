#ifndef QUADTREE_H_20111118
#define QUADTREE_H_20111118

#include "geometry.h"
#include "patch.h"

namespace crystal { namespace geometry { namespace terrain2d {

        class Quadtree : public Geometry
        {
        public:
                Quadtree(std::function<real (real, real)>,
                         int res);

                // Offer non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }
        private:
                PIntersection intersect_(const Ray &ray) const;

        private:
                Patch patch_;
        };

} } }

#endif // QUADTREE_H_20111118
