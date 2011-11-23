#ifndef PATCH_H
#define PATCH_H

#include "geometry.h"
#include <functional>

namespace crystal { namespace geometry { namespace terrain2d {
        class Patch : public Geometry
        {
        public:
                Patch(real left, real right, real front, real back,
                      std::function<real (real, real)> fun,
                      int resolution);

                // Offer some non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }

                real min_h() const { return min_h_; }
                real max_h() const { return max_h_; }
        private:
                PIntersection intersect_ (const Ray &ray) const;

        private:
                struct Quad {
                        Point a, b, c, d;
                };
                Quad *quads_;

                real left_, right_, front_, back_;
                std::function<real (real, real)> fun_;
                int res_;
                real min_h_, max_h_;
        };
} } }
#endif // PATCH_H
