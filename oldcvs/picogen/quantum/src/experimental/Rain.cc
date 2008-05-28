/***************************************************************************
 *            Snow.h
 *
 *  Tue Apr 18 18:40:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <picogen/picogen.h>
#include <picogen/experimental/Rain.h>

using picogen::misc::prim::real;
using picogen::misc::geometrics::Vector3d;
using picogen::misc::geometrics::Ray;
using picogen::misc::geometrics::BoundingBox;
using picogen::graphics::material::abstract::IBRDF;
using picogen::graphics::structs::intersection_t;
using picogen::graphics::image::color::Color;


namespace picogen {
namespace graphics {
namespace objects {

Rain::Rain() {
}

Rain::~Rain() {
}

bool Rain::Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const {
    return false;
}

};
};
};
