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

#ifndef VISUALISE_DIRECTION_H_INCLUDED_20100112
#define VISUALISE_DIRECTION_H_INCLUDED_20100112


namespace redshift { namespace backgrounds {
        DefineFinalizer(VisualiseDirection);
        class VisualiseDirection
        : public Background
        , DoFinalize (VisualiseDirection)
        {
        public:
                Color query (Ray const &ray) const {
                        return Color::FromRGB(
                                ray.direction.x+.5f,
                                ray.direction.y+.5f,
                                ray.direction.z+.5f
                        );
                }
        };
} }

#endif // VISUALISE_DIRECTION_H_INCLUDED_20100112
