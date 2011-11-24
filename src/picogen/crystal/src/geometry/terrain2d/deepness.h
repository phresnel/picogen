#ifndef DEEPNESS_H_20111122
#define DEEPNESS_H_20111122

#include "crystal_math.h"

namespace crystal { namespace geometry { namespace terrain2d {
struct Deepness {
        int maxRecursion;
        int minRecursion;
        real maxDetailRange,
             minDetailRange; // just a quick-fix, we should really be maximizing on
                             // actual recursion levels reached

        Deepness (int r, real maxD, real minD)
                : maxRecursion (r), minRecursion(3),
                  maxDetailRange(maxD), minDetailRange(minD)
        {
                assert (maxD < minD);
        }

        real maxMinRange() const { return minDetailRange - maxDetailRange; }

        template <typename Rect> // TODO: make this a non-template
        int deepness (const Point &camera, Rect const &rect) const {
                return deepness (camera,
                                 rect.left, rect.right, rect.front, rect.back);
        }

        int deepness (Point const &camera,
                      real left, real right,
                      real front, real back
        ) const {
                assert (left <= right);
                assert (front <= back);
                const real x = camera.x,
                           z = camera.z;
                real dist;
                if (x>=left && x<=right &&
                    z>=front && z<=back)
                {
                        dist = 0;
                } else if (x>=left && x<=right) {
                        dist = std::min(std::fabs(z-front),
                                        std::fabs(z-back));
                } else if (z>=front && z<=back) {
                        dist = std::min(std::fabs(x-left),
                                        std::fabs(x-right));
                } else {
                        const real xl = std::fabs (x-left),
                                   xr = std::fabs (x-right),
                                   zf = std::fabs (z-front),
                                   zb = std::fabs (z-back);
                        const real a = xl*xl + zb*zb,
                                   b = xl*xl + zf*zf,
                                   c = xr*xr + zb*zb,
                                   d = xr*xr + zf*zf;
                        real min = a;
                        if (b < min) min = b;
                        if (c < min) min = c;
                        if (d < min) min = d;

                        dist = std::sqrt(min);
                }

                //const real dist = length (p - camera);
                const real l_ = std::max (real(0), dist - maxDetailRange)
                               / maxMinRange();
                const real l = l_ < 0 ? 0 : l_ > 1 ? 1 : l_;

                const int ret = int(maxRecursion - real(maxRecursion-minRecursion) * l);
                /*qDebug() << "[" << camera.z() << ", l:" << l << l_ << ", dparams:"
                         << maxRecursion << maxDetailRange << minDetailRange << "]";*/
                /*qDebug() //<< dist << " --> " << l << " --> "
                         << ret
                            << "(" << 0.5*(left+right)
                                   << 0.5*(front+back) << ")"
                            << "|" << dist << "|";*/
                return ret;
        }
};

} } }

#endif // DEEPNESS_H_20111122
