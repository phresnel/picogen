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

#include "../../include/coordinates/uvcoordinates.hh"
#include "../../include/coordinates/imagecoordinates.hh"
#include "../../include/coordinates/lenscoordinates.hh"

#include "../../include/basictypes/sample.hh"
#include "../../include/basictypes/rectangle.hh"

#include "../../include/samplers/sampler.hh"

namespace redshift {

Sampler::Sampler (Rectangle const &region_, int samplesPerPixel_) 
: region(region_), samplesPerPixel(samplesPerPixel_) 
{}

inline int Sampler::computeTotalSampleCount () const {
        return samplesPerPixel * area (region);
}

}