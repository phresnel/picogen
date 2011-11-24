#ifndef PATCH_H
#define PATCH_H

#include "geometry.h"
#include "transition.h"
#include <functional>

namespace crystal { namespace geometry { namespace terrain2d {
        class Patch : public Geometry
        {
        public:
                Patch(real left, real right, real front, real back,
                      std::function<real (real, real)> fun,
                      int resolution,
                      Transition const &transition);

                // Offer some non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }

                real min_h() const { return min_h_; }
                real max_h() const { return max_h_; }
        private:
                PIntersection intersect_ (const Ray &ray) const;

        private:

                struct Triangle {
                        Point a,b,c;

                        Triangle() = default;

                        Triangle (Point const &a,
                                  Point const &b,
                                  Point const &c)
                                : a(a), b(b), c(c)
                        {
                        }

                };
                Triangle *triangles_;
                int triangleCount_;
                real min_h_, max_h_;
        };
} } }
#endif // PATCH_H
