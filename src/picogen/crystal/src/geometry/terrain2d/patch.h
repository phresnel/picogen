#ifndef PATCH_H
#define PATCH_H

#include "geometry.h"
#include "transition.h"
#include <functional>
#include <boost/scoped_array.hpp>

namespace crystal { namespace geometry { namespace terrain2d {
        class Patch : public Geometry
        {
        public:
                Patch(real left, real right, real front, real back,
                      std::function<real (real, real)> fun,
                      int resolution,
                      Transition const &transition);
                ~Patch();

                // Offer some non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }

                real min_h() const { return min_h_; }
                real max_h() const { return max_h_; }
        private:
                PIntersection intersect_ (const Ray &ray) const;

        private:

                #if 0
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
                int triangleCount_;*/
                #endif

                struct Fan {
                        Point *vertices;
                        char size;

                        Fan() : vertices(0), size(0) {}
                        ~Fan() { delete [] vertices; }

                        PIntersection intersect (Ray const &ray) const;
                };
                int fanCount_;
                boost::scoped_array<Fan> fans_;


                real min_h_, max_h_;
        };
} } }
#endif // PATCH_H
