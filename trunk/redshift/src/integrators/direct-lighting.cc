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

tuple<real_t,Color,real_t> DirectLighting::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample,
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
                        vector_cast<PointCompatibleVector>(normalG*0.001f);

                //-- skylight begin -------------------------------------------
                Color sum = Color::fromRgb (0,0,0);
                Color spec = Color::fromRgb (0,0,0);
                int numSamples = 1;
                // diffuse
                if (bsdf->is (Bsdf::reflection, Bsdf::diffuse)) {
                        Ray ray;
                        ray.position = poi;
                        const int numDiffuseSamples = 0;
                        if (numDiffuseSamples>0) for (numSamples = 0; numSamples < numDiffuseSamples; ++numSamples) {
                                optional<tuple<Color,Vector> > v_ = bsdf->sample_f (-ray.direction, Bsdf::reflection, Bsdf::diffuse);
                                if (v_) {
                                        ray.direction = get<1>(*v_);
                                        if (ray.direction.y>0) {
                                                sum = sum + multiplyComponents(bg->query (ray), get<0>(*v_));
                                        }
                                }
                        }
                }
                // spec
                if (doMirror && bsdf->is (Bsdf::reflection, Bsdf::specular)) {
                        Ray ray (poi, raydiff.direction);
                        const optional<tuple<Color,Vector> > v_ = bsdf->sample_f (
                                -ray.direction, Bsdf::reflection, Bsdf::specular);
                        if (v_) {
                                const tuple<Color,Vector> v = *v_;
                                ray.direction = get<1>(v);
                                Sample r = sample;
                                r.primaryRay = ray;
                                spec = spec + get<1> (scene.Li (r));
                        }
                }

                const Color surfaceSkyColor = spec + (
                        numSamples==0
                        ? Color(0.3,0.3,0.3)
                        : (sum * (1./numSamples)) * constants::pi
                ); // TODO: is this correct?
                //-------------------------------------------------------------

                Color ret = surfaceSkyColor;

                if (bg->hasSun()) {
                        const Vector sunDir = bg->getSunDirection();
                        const Ray ray (poi,sunDir);
                        const Color surfaceColor = bsdf->f(ray.direction, sunDir, Bsdf::reflection, Bsdf::diffuse)/* * constants::pi*/; // TODO: is this correct?
                        //std::cout << "eh" << std::flush;
                        /*const Color skyColor = bg->hasFastDiffuseQuery()
                                        ? bg->diffuseQuery (poi, normal)
                                        : Color(5,0,0);*/

                        if (!scene.doesIntersect (ray)) {
                                const real_t d = max(
                                        0.f,
                                        dot(sunDir,vector_cast<Vector>(normalS)));
                                ret = ret + multiplyComponents(surfaceColor,bg->querySun(ray))*d;
                        }
                }

                //-- atmospheric scattering -----------------------------------
                if (false) {
                        const Vector sunDir = bg->getSunDirection();
                        const real_t step = 10.f;
                        const Vector stepv = raydiff.direction * step;
                        Vector sv = vector_cast<Vector>(raydiff(0));
                        //const Color primaryColor = Color(0.5,0.5,0.5);//bg->query (raydiff);
                        for (real_t s=0; s<distance; (s+=step), (sv=sv+stepv)) {
                                Color C (Color::fromRgb(0,0,0));
                                /*const int num = 3;
                                for (int i=0; i<num; ++i) {
                                        const tuple<real_t,real_t,real_t> s
                                                = diffuseRng.uniform_sphere();
                                        const Vector dir (get<0>(s),fabs(get<1>(s)),get<2>(s));
                                        //C = C + bg->query (Ray (vector_cast<Point>(sv),dir));
                                }
                                C = C * (1.f / num);*/

                                const Ray sunRay = Ray (vector_cast<Point>(sv),sunDir);
                                if (!scene.doesIntersect (sunRay)) {
                                        C = C + bg->querySun (sunRay);
                                }

                                ret = (ret * 0.99f)
                                    + (C * 0.01f);
                        }
                }
                //-------------------------------------------------------------

                if (bg->hasAtmosphereShade())
                        ret = bg->atmosphereShade (ret, raydiff, gd.getDistance());
                return make_tuple(1.0f, ret, gd.getDistance());
        } else {
                return make_tuple (1.0,
                        scene.getBackground()->query(raydiff),
                        constants::infinity
                ); // TODO: atmosphere shade
        }
}



tuple<real_t,Color,real_t> DirectLighting::Li (
        const Scene &scene,
        const RayDifferential &raydiff,
        const Sample &sample
) const {
        return Li(scene, raydiff, sample, true);
}

}
