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



DirectLighting::DirectLighting (unsigned int numAmbientSamples)
: numAmbientSamples(numAmbientSamples)
{
}



tuple<real_t,Color> DirectLighting::Li_VolumeOnly (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        Random &rand
) const {
        return make_tuple (
                1.0, scene.getBackground()->query(raydiff)
        );
}



tuple<real_t,Color,real_t> DirectLighting::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
        Random &rand,
        const bool doMirror
) const {
        const optional<Intersection> I (scene.intersect (raydiff));
        if (I) {
                const DifferentialGeometry gd = I->getDifferentialGeometry();
                const shared_ptr<Bsdf> bsdf = I->getPrimitive()->getBsdf (gd);
                const real_t       distance = I->getDistance();
                const shared_ptr<Background> bg (scene.getBackground());
                const Normal normalG = gd.getGeometricNormal();
                const Normal normalS = gd.getShadingNormal();
                const Point poi = gd.getCenter()+
                        vector_cast<PointCompatibleVector>(normalG*real_t(0.001));

                //-- skylight begin -------------------------------------------
                Color sum = Color(0);
                Color spec = Color(0);
                int numSamples = 1;
                // diffuse
                if (0) if (bsdf->is (Bsdf::reflection, Bsdf::diffuse)) {
                        RayDifferential ray;
                        ray.position = poi;
                        if (numAmbientSamples>0)
                        for (numSamples = 0; numSamples < numAmbientSamples; ++numSamples) {
                                const optional<tuple<Color,Vector> > v_ =
                                        bsdf->sample_f (
                                                -ray.direction,
                                                Bsdf::reflection, Bsdf::diffuse,
                                                rand);
                                if (v_) {
                                        ray.direction = get<1>(*v_);
                                        Sample s = sample;
                                        s.primaryRay = ray;
                                        const tuple<real_t,Color> L = scene.Li_VolumeOnly(s, rand);
                                        /*if (ray.direction.y>0)*/ {
                                                sum = sum +
                                                        //bg->query (ray)  *  get<0>(*v_);
                                                        get<1>(L)  *  get<0>(*v_);
                                        }
                                }
                        }
                }
                // spec
                if (doMirror && bsdf->is (Bsdf::reflection, Bsdf::specular)) {
                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector> > v_ = bsdf->sample_f (
                                -ray.direction,
                                Bsdf::reflection, Bsdf::specular,
                                rand);
                        if (v_) {
                                const tuple<Color,Vector> v = *v_;
                                ray.direction = get<1>(v);
                                Sample r = sample;
                                r.primaryRay = ray;
                                spec = spec + get<1>(scene.Li (r, rand)) * get<0>(v);
                        }
                }

                const Color surfaceSkyColor = spec + (
                        numSamples==0
                        ? Color(0.3)
                        : Color((sum / Color::real_t(numSamples)) * Color::real_t(constants::pi))
                ); // TODO: is this correct?
                //-------------------------------------------------------------

                Color ret = surfaceSkyColor;

                if (bg->hasSun()) {
                        const Vector sunDir = bg->getSunDirection();
                        const Ray ray (poi,sunDir);
                        const Color surfaceColor = bsdf->f(
                                ray.direction,
                                sunDir,
                                Bsdf::reflection, Bsdf::diffuse,
                                rand)/* * constants::pi*/; // TODO: is this correct?

                        if (!scene.doesIntersect (ray)) {
                                const real_t d = max(
                                        real_t(0),
                                        dot(sunDir,vector_cast<Vector>(normalS)));
                                ret = ret + (surfaceColor * bg->querySun(ray))*d;
                        }
                }


                if (0) if (bg->hasAtmosphereShade()) {
                        ret = bg->atmosphereShade (ret, raydiff, gd.getDistance());
                }
                return make_tuple(1.0f, ret, gd.getDistance());
        } else {
                return make_tuple (1.0,
                        scene.getBackground()->query(raydiff),
                        constants::infinity
                );
        }
}



tuple<real_t,Color,real_t> DirectLighting::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample, Random &rand
) const {
        return Li(scene, raydiff, sample, rand, true);
}

}
