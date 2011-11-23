#ifndef QUADTREE_H_20111118
#define QUADTREE_H_20111118

#include "geometry.h"
#include "patch.h"
#include <memory>
#include <array>

namespace crystal { namespace geometry { namespace terrain2d {

        class Deepness;

        struct Rect {
                real left, right, front, back;

                Rect () = default;
                Rect (real left, real right,
                      real front, real back)
                        : left(left), right(right), front(front), back(back)
                {}
        };
        typedef std::array<Rect,4> ChildRects;
        inline ChildRects child_boxen (
                real left, real right, real front, real back)
        {
                using std::array;
                using std::vector;

                //const Point c = aabb.center();
                const real cx = 0.5*right + 0.5*left,
                           cz = 0.5*front + 0.5*back;
                const real oldW = right-left,
                           //oldH = aabb.height(),
                           oldD = back-front,
                           newW = oldW / 2,
                           //newH = oldH,
                           newD = oldD / 2
                          ;
                std::array<Rect,4> ret;
                ret [0] = {cx - newW, cx       ,  cz     , cz + newD};
                ret [1] = {cx       , cx + newW,  cz     , cz + newD};
                ret [2] = {cx       , cx + newW,  cz-newD, cz       };
                ret [3] = {cx - newW, cx       ,  cz-newD, cz       };
                return ret;
        }


        class Quadtree : public Geometry
        {
        public:
                Quadtree(Deepness const &,
                         std::function<real (real, real)>,
                         int res);
                ~Quadtree();

                // Offer non-virtual access.
                PIntersection intersect (const Ray &ray) const {
                        return intersect_ (ray);
                }

                Quadtree (Quadtree const&)             = delete;
                Quadtree& operator = (Quadtree const&) = delete;
        private:
                Quadtree ()                            = default;
                PIntersection intersect_(const Ray &ray) const;

                PIntersection intersect_ (const Ray &ray, int *ordering) const;

        private:
                void create (terrain2d::Deepness const &,
                             Rect const &rect,
                             std::function<real(real,real)> fun,
                             int patchRes,
                             int depth);
                void make_inner (terrain2d::Deepness const &,
                                 std::function<real(real,real)> fun,
                                 int patchRes,
                                 int depth);
                void make_leaf  (std::function<real(real,real)> fun,
                                 int patchRes);
        private:
                union {
                        Patch *patch_;
                        Quadtree *children_;
                };
                bool leaf_;
                real min_h_, max_h_;
                Rect rect_;
        };

} } }

#endif // QUADTREE_H_20111118
