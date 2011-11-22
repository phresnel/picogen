#ifndef QUADTREE_H_20111118
#define QUADTREE_H_20111118

#include "geometry.h"
#include "patch.h"
#include <memory>

namespace crystal { namespace geometry { namespace terrain2d {

        class Quadtree : public Geometry
        {
        public:
                Quadtree(std::function<real (real, real)>,
                         int res);
                ~Quadtree();

                // Offer non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }

                Quadtree ()                            = delete;
                Quadtree (Quadtree const&)             = delete;
                Quadtree& operator = (Quadtree const&) = delete;
        private:
                PIntersection intersect_(const Ray &ray) const;

        private:
                union {
                        Patch *patch_;
                        Quadtree *children_;
                };
                bool leaf_;
        };

} } }

#endif // QUADTREE_H_20111118
