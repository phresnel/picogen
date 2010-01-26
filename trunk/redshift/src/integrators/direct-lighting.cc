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
        const Sample &sample,
        const bool doMirror
) const {
        const optional<Intersection> I (
                                scene.intersect (raydiff));
        if (I) {
                const DifferentialGeometry gd =
                        I->getDifferentialGeometry();


                const shared_ptr<Bsdf> bsdf = I->getPrimitive()->getBsdf (gd);
                const shared_ptr<Background> bg (scene.getBackground());
                const Normal normal = gd.getNormal();
                const Point poi = gd.getCenter()+
                        vector_cast<PointCompatibleVector>(normal*0.001f);

                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // crap begin
                Color sum = Color::fromRgb (0,0,0);
                Color spec = Color::fromRgb (0,0,0);
                int numSamples = 1;
                // diffuse
                if (bsdf->hasAny (Bsdf::reflection, Bsdf::diffuse)) {
                        Ray ray;
                        ray.position = poi;

                        // BETTER USE AMBIENT OCCLUSION FACTOR. THIS IS KINDA SLOW!
                        const tuple<Vector,Vector,Vector> cs = coordinateSystem (normal);
                        const Vector &X = get<0>(cs);
                        const Vector &Y = get<1>(cs);
                        const Vector &Z = get<2>(cs);
                        const int numDiffuseSamples = 20;
                        if (numDiffuseSamples>0) for (numSamples = 0; numSamples < numDiffuseSamples; ++numSamples) {
                                const tuple<real_t,real_t,real_t> sphere = diffuseRng.cosine_hemisphere();
                                const real_t &sx = get<0>(sphere);
                                const real_t &sy = get<1>(sphere);
                                const real_t &sz = get<2>(sphere);
                                optional<tuple<Color,Vector> > v_ = bsdf->sample_f (ray.direction, Bsdf::reflection, Bsdf::diffuse);
                                if (v_) {
                                        const tuple<Color,Vector> v = *v_;
                                        const Vector d = X * get<1>(v).x + Y * get<1>(v).y + Z * get<1>(v).z; // TODO: where to do this transform?
                                        ray.direction = d;
                                        if (d.y>0) {
                                                sum = sum + multiplyComponents(bg->query (ray), get<0>(v));
                                        }
                                }
                        }
                }
                // spec
                /*if (bsdf->hasAny (Bsdf::reflection, Bsdf::specular)) {
                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector> > v_ = bsdf->sample_f (
                                ray.direction, Bsdf::reflection, Bsdf::specular);
                        if (v_) {
                                const tuple<Color,Vector> v = *v_;
                                ray.direction = get<1>(v);
                                spec = spec + multiplyComponents(bg->query (ray), get<0>(v));
                        }
                }*/
                if (doMirror && bsdf->hasAny (Bsdf::reflection, Bsdf::specular)) {
                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector> > v_ = bsdf->sample_f (
                                ray.direction, Bsdf::reflection, Bsdf::specular);
                        if (v_) {
                                const tuple<Color,Vector> v = *v_;
                                ray.direction = get<1>(v);
                                spec = spec + get<1> (Li (scene, RayDifferential(ray), sample, false));
                        }
                }

                const Color surfaceSkyColor = spec + (
                        numSamples==0
                        ? Color(0.3,0.3,0.3)
                        : (sum * (1./numSamples)) * constants::pi
                ); // TODO: is this correct?
                // crap end
                //----------------------------------------------------------------------------

                Color ret = surfaceSkyColor;

                if (bg->hasSun()) {
                        const Vector sunDir = bg->getSunDirection();
                        const Ray ray (poi,sunDir);
                        const Color surfaceColor = bsdf->f(ray.direction, sunDir)/* * constants::pi*/; // TODO: is this correct?
                        //std::cout << "eh" << std::flush;
                        /*const Color skyColor = bg->hasFastDiffuseQuery()
                                        ? bg->diffuseQuery (poi, normal)
                                        : Color(5,0,0);*/

                        if (!scene.doesIntersect (ray)) {
                                const real_t d = max(
                                        0.f,
                                        dot(sunDir,vector_cast<Vector>(normal)));
                                ret = ret + multiplyComponents(surfaceColor,bg->querySun(ray))*d;
                        }
                }

                if (bg->hasAtmosphereShade())
                        ret = bg->atmosphereShade (ret, raydiff, gd.getDistance());
                return make_tuple(1.0f, ret);
        } else {
                return make_tuple (1.0,
                        scene.getBackground()->query(raydiff)); // TODO: atmosphere shade
        }
}



tuple<real_t,Color> DirectLighting::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample
) const {
        return Li(scene, raydiff, sample, true);
}

}
