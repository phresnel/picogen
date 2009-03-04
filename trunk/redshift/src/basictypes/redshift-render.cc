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

#ifdef AMALGAM
#include "../include/setup.hh"
#include "../include/basictypes/scene.hh"
#include "../include/basictypes/redshift-render.hh"
#else
#include "../../include/setup.hh"
#include "../../include/basictypes/scene.hh"
#include "../../include/basictypes/redshift-render.hh"
#endif

namespace redshift {

        Redshift::Redshift ()
        : scene () {
                if (ALLOW_LOGSPAM) {
                        std::cout << "Redshift::Redshift() (" << this << ")\n";
                }
        }
        
        Redshift::~Redshift () {
                if (ALLOW_LOGSPAM) {
                        std::cout << "Redshift::~Redshift().\n";
                }
        }
        
        
}