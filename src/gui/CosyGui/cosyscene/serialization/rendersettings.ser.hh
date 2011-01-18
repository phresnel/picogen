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


#ifndef RENDERSETTINGS_SERIALIZATION_HH_20101019
#define RENDERSETTINGS_SERIALIZATION_HH_20101019

#include "../rendersettings.hh"
#include "cosyscene/serialization/stash.ser.hh"
#include "cosyscene/serialization/surfaceintegrator.ser.hh"

namespace cosyscene {

template<typename Arch>
inline void RenderSettings::serialize (Arch &arch) {
        using actuarius::pack;
        if (Arch::deserialize || !stash_.empty())
                arch & pack("stash", stash_);

        arch & pack("width", width_)
             & pack("height", height_)
             & pack("samples-per-pixel", samplesPerPixel_)
             & pack("random-seed", randomSeed_)
             & pack("max-lazy-quadtree-depth-override", maxLazyQuadtreeDepth_)
             & pack("surface-integrator", surfaceIntegrator_)
             ;
}

template<typename Arch>
inline void TwinRenderSettings::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack("preview", *preview_)
             & pack("production", *production_);
}


} // namespace cosyscene

#endif // RENDERSETTINGS_SERIALIZATION_HH_20101019
