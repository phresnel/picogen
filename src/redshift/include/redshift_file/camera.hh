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

#ifndef CAMERA_HH_20101013
#define CAMERA_HH_20101013

namespace redshift_file {
        struct Camera {

                enum Type {
                        pinhole,
                        cylindrical,
                        cubemap_left,
                        cubemap_right,
                        cubemap_front,
                        cubemap_back,
                        cubemap_bottom,
                        cubemap_top
                };
                static const actuarius::Enum<Type> Typenames;

                struct PinholeParams {
                        double front;
                        PinholeParams () : front(1) {}
                };
                struct CylindricalParams {
                        double front;
                        CylindricalParams () : front(1) {}
                };



                std::string title;
                TransformList transforms;
                Type type;
                PinholeParams pinholeParams;
                CylindricalParams cylindricalParams;


                Camera () : type(pinhole)
                {}



                shared_ptr<redshift::Camera> toRedshiftCamera (
                        unsigned int width, unsigned int height
                ) const {
                        using redshift::Camera;

                        switch (type) {
                        case pinhole:
                                return shared_ptr<redshift::Camera> (new redshift::camera::Pinhole(
                                        width, height, pinholeParams.front,
                                        transforms.toRedshiftTransform()));

                        case cylindrical:
                                return shared_ptr<redshift::Camera> (new redshift::camera::Cylindrical(
                                        width, height, cylindricalParams.front,
                                        transforms.toRedshiftTransform()));

                        case cubemap_left:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::left,
                                        transforms.toRedshiftTransform()));
                        case cubemap_right:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::right,
                                        transforms.toRedshiftTransform()));
                        case cubemap_bottom:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::bottom,
                                        transforms.toRedshiftTransform()));
                        case cubemap_top:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::top,
                                        transforms.toRedshiftTransform()));
                        case cubemap_front:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::front,
                                        transforms.toRedshiftTransform()));
                        case cubemap_back:
                                return shared_ptr<redshift::Camera> (new redshift::camera::CubeMapFace(
                                        width, height, redshift::camera::CubeMapFace::back,
                                        transforms.toRedshiftTransform()));

                        default:
                                throw std::runtime_error("only  pinhole supported");
                        };
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("transform", transforms);
                        arch & pack ("type", Typenames, type);

                        switch (type) {
                        case pinhole:
                                arch & pack ("front", pinholeParams.front);
                                break;
                        case cylindrical:
                                arch & pack ("front", cylindricalParams.front);
                                break;
                        case cubemap_front:
                        case cubemap_back:
                        case cubemap_left:
                        case cubemap_right:
                        case cubemap_bottom:
                        case cubemap_top:
                                break;
                        };
                }
        };
}

#endif // CAMERA_HH_20101013
