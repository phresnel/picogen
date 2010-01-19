//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "../../include/integrators/direct-lighting.hh"

namespace redshift {

tuple<real_t,Color> DirectLighting::Li (
        const Scene &scene, 
        const RayDifferential &raydiff,
        const Sample &sample
) const {
        const optional<Intersection> I (
                                scene.intersect (raydiff));
        if (I) {
                const DifferentialGeometry gd =
                        I->getDifferentialGeometry();
                const Vector sunDir = normalize(Vector(9,1,3));
                const Normal normal = gd.getNormal();
                const Point poi = gd.getCenter()+
                        vector_cast<PointCompatibleVector>(normal*0.1f);
                const Ray ray (poi,sunDir);
                const shared_ptr<Background> bg (scene.getBackground());
                //std::cout << "eh" << std::flush;
                /*const Color skyColor = bg->hasFastDiffuseQuery()
                                ? bg->diffuseQuery (poi, normal)
                                : Color(5,0,0);*/
                
                const shared_ptr<Bsdf> bsdf = I->getPrimitive()->getBsdf (gd);

                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // crap begin
                Color sum = Color::fromRgb (0,0,0);
                int numSamples = 1;
                if (1) {
                        Ray ray;
                        ray.position = poi;

                        // BETTER USE AMBIENT OCCLUSION FACTOR. THIS IS KINDA SLOW!
                        const tuple<Vector,Vector,Vector> cs = coordinateSystem (normal);
                        const Vector &X = get<0>(cs);
                        const Vector &Y = get<1>(cs);
                        const Vector &Z = get<2>(cs);
                        const int maxNumSamples = 2;
                        for (numSamples = 0; numSamples < maxNumSamples; ++numSamples) {
                                const tuple<real_t,real_t,real_t> sphere = diffuseRng.cosine_hemisphere();
                                const real_t &sx = get<0>(sphere);
                                const real_t &sy = get<1>(sphere);
                                const real_t &sz = get<2>(sphere);
                                tuple<Color,Vector> rrr = make_tuple(Color(1,1,1), Vector(sx,sy,sz));

                                optional<tuple<Color,Vector> > v_ = bsdf->sample_f (ray.direction, Bsdf::reflection, Bsdf::diffuse);
                                if (v_) {
                                        tuple<Color,Vector> v = *v_;
                                        const Vector d = X * get<1>(v).x + Y * get<1>(v).y + Z * get<1>(v).z; // TODO: where to do this transform?
                                        ray.direction = d;
                                        if (d.y>0) {// && !scene.doesIntersect(ray)) {
                                                sum = sum + bg->query (ray);
                                        }
                                }
                        }
                }
                const Color surfaceSkyColor = (sum * (1./numSamples));
                // crap end
                //----------------------------------------------------------------------------

                const Color surfaceColor = bsdf->f(ray.direction, sunDir);

                Color ret = surfaceSkyColor;

                if (scene.doesIntersect (ray)) {
                } else {
                        const real_t d = max(
                                0.f,
                                dot(sunDir,vector_cast<Vector>(normal)));
                        ret = ret + surfaceColor*d;
                }
                
                if (bg->hasAtmosphereShade())
                        ret = bg->atmosphereShade (ret, ray, gd.getDistance());
                return make_tuple(1.0f, ret);
        } else {
                return make_tuple (1.0,
                        scene.getBackground()->query(raydiff));
        }
}

}
