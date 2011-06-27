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

#include <string>
#include "transform.hh"

namespace picogen { namespace redshift_file {
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


                // Serialization.
                template<typename Arch> void serialize (Arch &arch);
        };
} }

#endif // CAMERA_HH_20101013
