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

#ifndef SURFACEINTEGRATOR_SERIALIZATION_HH_20101215
#define SURFACEINTEGRATOR_SERIALIZATION_HH_20101215

#include "../surfaceintegrator.hh"

namespace picogen { namespace cosyscene {

template<typename Arch>
inline void WhittedAmbientIntegrator::serialize (Arch &arch) {
        arch & actuarius::pack ("num-ambient-samples", numAmbientSamples_);
}

template<typename Arch>
inline void SurfaceIntegrator::serialize (Arch &arch) {
        using actuarius::pack;

        using actuarius::pack;
        arch & pack ("type", kind_, Typenames);

        switch (kind_) {
        case none:
                arch & pack ("parameters", none_); break;
                break;
        case whitted:
                arch & pack ("parameters", whitted_); break;
                break;
        case whitted_ambient:
                arch & pack ("parameters", whittedAmbient_); break;
                break;
        case path:
                arch & pack ("parameters", path_); break;
                break;
        case debug_distance:
                arch & pack ("parameters", debugDistance_); break;
                break;
        case debug_normals:
                arch & pack ("parameters", debugNormals_); break;
                break;
        }
}


} }

#endif // SURFACEINTEGRATOR_SERIALIZATION_HH_20101215
