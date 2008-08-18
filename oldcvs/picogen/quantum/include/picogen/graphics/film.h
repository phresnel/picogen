/***************************************************************************
 *            film.h
 *
 *  2008-08-17 12:09:00 2008
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

#ifndef FILM_H__INCLUDED
#define FILM_H__INCLUDED

namespace picogen {
    namespace graphics {
        namespace film {
            namespace abstract {
                class IFilm {
                    public:
                        virtual void addSample (param_in (::picogen::graphics::structs::sample, sample)) = 0;
                        virtual ::picogen::graphics::color::Color operator () (unsigned int x, unsigned int y) const = 0;
                        virtual unsigned int getWidth() const = 0;
                        virtual unsigned int getHeight() const = 0;
                };
            } // namespace abstract {
        } // namespace film {
    } // namespace graphics {
} // namespace picogen {

#include <picogen/graphics/film/SimpleColorFilm.h>
#include <picogen/graphics/film/DistanceVis.h>
#include <picogen/graphics/film/NormalVis.h>
#include <picogen/graphics/film/RayDirVis.h>


#endif // FILM_H__INCLUDED
