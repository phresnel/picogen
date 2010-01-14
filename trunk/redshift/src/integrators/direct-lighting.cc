//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include "../../include/setup.hh"

#include "../../include/basictypes/background.hh"
#include "../../include/backgrounds/visualise-direction.hh"

#include "../../include/basictypes/differentialgeometry.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"

#include "../../include/rendertargets/rendertargetlock.hh"
#include "../../include/rendertargets/rendertarget.hh"

#include "../../include/basictypes/sample.hh"

#include "../../include/cameras/camera.hh"

#include "../../include/primitives/primitive.hh"

#include "../../include/interaction/progressreporter.hh"
#include "../../include/interaction/usercommandprocessor.hh"

#include "../../include/basictypes/scene.hh"

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
                const Vector sunDir = normalize(Vector(1,1,0));
                const Point poi = gd.getCenter();
                const Normal normal = gd.getNormal();
                const Ray ray (
                        poi+vector_cast<PointCompatibleVector>(
                                                normal*0.1f),
                        sunDir
                );
                //std::cout << "eh" << std::flush;
                if (scene.doesIntersect (ray)) {
                        return make_tuple (
                                1.0, 
                                Color(0.1,0.1,0.1));
                }
                
                const real_t d = max(
                      0.f,
                      dot(sunDir,vector_cast<Vector>(normal)));
                
                Color skyCol (10,10,10);

                return make_tuple (
                        1.0,
                        skyCol
                        + Color(
                                I->getNormal().x+0.5,
                                I->getNormal().y+0.5,
                                I->getNormal().z+0.5
                        ) * d
                );
        } else {
                return make_tuple (1.0,
                        scene.getBackground()->query(raydiff));
        }
}

}