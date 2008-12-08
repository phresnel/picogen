/***************************************************************************
 *            ConstanstShader.cc
 *
 *  2008-12-08
 *  Copyright  2008  Sebastian Mach
 *  phresnel@gmail.com
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

namespace picogen {
    namespace graphics {
        namespace shaders {
            void ConstantShader::shade (
                picogen::graphics::color::Color &color,
                const picogen::geometrics::Vector3d &normal,
                const picogen::geometrics::Vector3d &position
            ) const {
                color = this->color;
            }
        }
    }
}
