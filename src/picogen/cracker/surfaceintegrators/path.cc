#include "path.h"

#include "color.h"
#include "ray.h"
#include "math3d.h"
#include "scene.h"
#include "random.h"
#include "material.h"

namespace picogen { namespace cracker {

Color PathIntegrator::operator() (Ray const &ray,
                                  Scene const &scene,
                                  Random &random) const
{
        const Intersection::Optional pi = scene(ray);
        if (!pi) return Color::Gray(0.4);

        const Intersection &i = pi.intersection();
        const Material &mat = i.material_ref();
        const BsdfSample sample = mat.sample(-ray.direction(), random);

        if (sample.pdf() == 0) return Color::Black();

        const DifferentialGeometry &dg = i.differentialGeometry();
        const Normal &u = dg.dpdu(),
                     &v = dg.dpdv(),
                     &w = i.normal();
        const Direction &inc = sample.incident().direction();
        const Vector &d = u * inc.x()
                        + v * inc.z()
                        + w * inc.y();

        //TODO -> need to transform ray into surface space
        //TODO -> need to transform sample-direction into world space
        return Color::FromRgb(0.5+d.x(),
                              0.5+d.y(),
                              0.5+d.z());
}

} }
