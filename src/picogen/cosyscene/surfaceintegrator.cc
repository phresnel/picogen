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

#include "surfaceintegrator.hh"
#include "actuarius/bits/enum.hh"

namespace picogen { namespace cosyscene {

const actuarius::Enum<SurfaceIntegrator::Kind> SurfaceIntegrator::Typenames =
( actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::none, "none")
| actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::whitted, "whitted")
| actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::whitted_ambient, "whitted_ambient")
| actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::path, "path")
| actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::debug_distance, "debug_distance")
| actuarius::Nvp<SurfaceIntegrator::Kind>(SurfaceIntegrator::debug_normals, "debug_normals")
);



WhittedAmbientIntegrator::WhittedAmbientIntegrator(
        unsigned int numAmbientSamples
) : numAmbientSamples_(numAmbientSamples)
{
}
void WhittedAmbientIntegrator::setNumAmbientSamples (unsigned int val) {
        numAmbientSamples_ = val;
}
unsigned int WhittedAmbientIntegrator::numAmbientSamples() const {
        return numAmbientSamples_;
}
bool operator == (
        WhittedAmbientIntegrator const& lhs,
        WhittedAmbientIntegrator const& rhs
) {
        return lhs.numAmbientSamples() == rhs.numAmbientSamples();
}




SurfaceIntegrator::SurfaceIntegrator() {
        toWhittedIntegrator(WhittedIntegrator());
}

SurfaceIntegrator::Kind SurfaceIntegrator::kind() const {
        return kind_;
}

void SurfaceIntegrator::to (Kind kind) {
        switch (kind) {
        case SurfaceIntegrator::none:
                toNoneIntegrator();
                return;
        case SurfaceIntegrator::whitted:
                toWhittedIntegrator();
                return;
        case SurfaceIntegrator::whitted_ambient:
                toWhittedAmbientIntegrator();
                return;
        case SurfaceIntegrator::path:
                toPathIntegrator();
                return;
        case SurfaceIntegrator::debug_distance:
                toDebugDistanceIntegrator();
                return;
        case SurfaceIntegrator::debug_normals:
                toDebugNormalsIntegrator();
                return;
        }
        throw std::runtime_error(
                        "unsupported Kind in SurfaceIntegrator::to(Kind)");
}

void SurfaceIntegrator::toNoneIntegrator (NoneIntegrator const &val) {
        kind_ = none;
        none_ = val;
}
void SurfaceIntegrator::toWhittedIntegrator (WhittedIntegrator const &val) {
        kind_ = whitted;
        whitted_ = val;
}
void SurfaceIntegrator::toWhittedAmbientIntegrator(WhittedAmbientIntegrator const &val) {
        kind_ = whitted_ambient;
        whittedAmbient_ = val;
}
void SurfaceIntegrator::toPathIntegrator(PathIntegrator const &val) {
        kind_ = path;
        path_ = val;
}
void SurfaceIntegrator::toDebugDistanceIntegrator(DebugDistanceIntegrator const &val) {
        kind_ = debug_distance;
        debugDistance_ = val;
}
void SurfaceIntegrator::toDebugNormalsIntegrator(DebugNormalsIntegrator const &val) {
        kind_ = debug_normals;
        debugNormals_ = val;
}


NoneIntegrator SurfaceIntegrator::noneIntegrator () const {
        return none_;
}
WhittedIntegrator SurfaceIntegrator::whittedIntegrator () const {
        return whitted_;
}
WhittedAmbientIntegrator SurfaceIntegrator::whittedAmbientIntegrator() const {
        return whittedAmbient_;
}
PathIntegrator SurfaceIntegrator::pathIntegrator() const {
        return path_;
}
DebugDistanceIntegrator SurfaceIntegrator::debugDistanceIntegrator() const {
        return debugDistance_;
}
DebugNormalsIntegrator SurfaceIntegrator::debugNormalsIntegrator() const {
        return debugNormals_;
}




bool operator == (SurfaceIntegrator const &lhs, SurfaceIntegrator const &rhs) {
        if (lhs.kind() != rhs.kind())
                return false;
        switch (lhs.kind()) {
        case SurfaceIntegrator::none:
                return lhs.noneIntegrator() == rhs.noneIntegrator();
        case SurfaceIntegrator::whitted:
                return lhs.whittedIntegrator() == rhs.whittedIntegrator();
        case SurfaceIntegrator::whitted_ambient:
                return lhs.whittedAmbientIntegrator() == rhs.whittedAmbientIntegrator();
        case SurfaceIntegrator::path:
                return lhs.pathIntegrator() == rhs.pathIntegrator();
        case SurfaceIntegrator::debug_distance:
                return lhs.debugDistanceIntegrator() == rhs.debugDistanceIntegrator();
        case SurfaceIntegrator::debug_normals:
                return lhs.debugNormalsIntegrator() == rhs.debugNormalsIntegrator();
        }
        throw std::runtime_error("unsupported kind in bool operator == "
                "(SurfaceIntegrator const &lhs, SurfaceIntegrator const &rhs)");
}


} }
