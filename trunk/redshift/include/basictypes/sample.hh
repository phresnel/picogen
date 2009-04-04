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

#ifndef SAMPLE_HH_INCLUDED_20090310
#define SAMPLE_HH_INCLUDED_20090310

namespace redshift {
        
        class Sample {
        public:
                ImageCoordinates imageCoordinates;
                LensCoordinates lensCoordinates;
                RayDifferential primaryRay;
                shared_ptr<RenderTarget> renderTarget;

                Sample (ImageCoordinates const & imageCoordinates_,
                        LensCoordinates const & lensCoordinates_)
                : imageCoordinates(imageCoordinates_)
                , lensCoordinates(lensCoordinates_)
                {}
                
                Sample (ImageCoordinates const & imageCoordinates_,
                        LensCoordinates const & lensCoordinates_,
                        shared_ptr<RenderTarget> renderTarget_)
                : imageCoordinates (imageCoordinates_)
                , lensCoordinates (lensCoordinates_)
                , renderTarget (renderTarget_)
                {}
           
                Sample (ImageCoordinates const & imageCoordinates_,
                        LensCoordinates const & lensCoordinates_,
                        RayDifferential const & primaryRay_)
                : imageCoordinates(imageCoordinates_)
                , lensCoordinates(lensCoordinates_)
                , primaryRay (primaryRay_)
                {}

        };        
}

#endif // SAMPLE_HH_INCLUDED_20090310
