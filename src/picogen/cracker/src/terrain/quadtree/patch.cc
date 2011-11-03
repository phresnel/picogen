
#include "patch.h"
#include "ray.h"
#include "materials/lambertmaterial.h"
#include <QDebug>
#include <algorithm>

namespace picogen { namespace cracker {




namespace detail {


Patch::Patch (real left, real right,
              real front, real back,
              unsigned int res_x,
              unsigned int res_z,
              std::function<real (real, real)> fun,
              real &y_min, real &y_max,
              LodSmoothing)
: left_(left), right_(right), front_(front), back_(back)
, res_x_(res_x), res_z_(res_z)
, stride_(res_x_)//+1)
//, h_(new Vector [(res_x+1)*(res_z+1)])
, h_(new Quad [res_x*res_z])
, width_ (right_-left_), depth_(back_-front_)
, voxelWidth_ (width_ / res_x_)
, voxelDepth_ (depth_ / res_z_)
, ires_x_(1/real(res_x_))
, ires_z_(1/real(res_z_))
, iwidth_(1/width_)
, idepth_(1/depth_)
{
        assert (res_x >= 2);
        assert (res_z >= 2);
        assert (left < right);
        assert (front < back);

        y_min = std::numeric_limits<real>::max();
        y_max = -y_min;
        //for (unsigned int uz=0; uz<=res_z; ++uz) {
        //        for (unsigned int ux=0; ux<=res_x; ++ux) {
        for (unsigned int uz=0; uz<res_z; ++uz) {
                for (unsigned int ux=0; ux<res_x; ++ux) {
                        const real u  = ux     / static_cast<real>(res_x);
                        const real u1 = (1+ux) / static_cast<real>(res_x);
                        const real v  = uz     / static_cast<real>(res_z);
                        const real v1 = (1+uz) / static_cast<real>(res_z);

                        const real x = (1-u)*left + u*right;
                        const real z = (1-v)*front + v*back;
                        const real x1 = (1-u1)*left + u1*right;
                        const real z1 = (1-v1)*front + v1*back;
                        const real y0 = fun(x,  z1);
                        const real y1 = fun(x1, z1);
                        const real y2 = fun(x1, z);
                        const real y3 = fun(x,  z);

                        if (y0 < y_min) y_min = y0;
                        if (y1 < y_min) y_min = y1;
                        if (y2 < y_min) y_min = y2;
                        if (y3 < y_min) y_min = y3;
                        if (y0 > y_max) y_max = y0;
                        if (y1 > y_max) y_max = y1;
                        if (y2 > y_max) y_max = y2;
                        if (y3 > y_max) y_max = y3;

                        //h(ux,uz) = y;
                        //h(ux,uz) = Vector(x,y,z);
                        h(ux,uz) = Quad(Quad::Rectangle(x,x1,z,z1),
                                        y0, y1, y2, y3);
                }
        }

        /*exactBB = BoundingBox (Point(left, y_min, front),
                               Point(right, y_max, back));*/

        material_.reset (new LambertMaterial(Color::FromRgb(
                                                     1.0,1.0,1.0)));
}

Patch::~Patch() {
        delete [] h_;
}

} } }
