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

#ifndef TO_REDSHIFT_HH_20101014
#define TO_REDSHIFT_HH_20101014

// redshift fwd+Co.
namespace redshift {
        class Sky;
        class Camera;
}


// redshift_file fwd+Co.
namespace redshift_file {
        class Background;
        class Camera;
}
#include "color_to_redshift.hh"

#include "redshift/include/smart_ptr.hh"

namespace redshift_file {
        redshift::shared_ptr<redshift::Sky>    toRedshift (Background const &);
        redshift::shared_ptr<redshift::Camera> toRedshift (Camera const &,
                                                           unsigned int width, 
                                                           unsigned int height);        
}

#endif // TO_REDSHIFT_HH_20101014
