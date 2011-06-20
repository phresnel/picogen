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

#ifndef RENDERSETTINGS_HH_20101013
#define RENDERSETTINGS_HH_20101013

#include <string>
#include "surfaceintegrator.hh"
#include "volumeintegrator.hh"

namespace redshift_file {
        struct RenderSettings {
                unsigned int width, height, samplesPerPixel;
                unsigned int min_y, max_y;
                unsigned int userSeed;
                std::string title;
                SurfaceIntegrator surfaceIntegrator;
                VolumeIntegrator volumeIntegrator;

                RenderSettings ()
                : width(800), height(600), samplesPerPixel(1)
                , min_y(0), max_y(0), userSeed(0)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);
        };
}

#endif // RENDERSETTINGS_HH_20101013
