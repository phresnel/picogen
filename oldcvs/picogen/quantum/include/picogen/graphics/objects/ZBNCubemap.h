/***************************************************************************
 *            ZBNCubemap.h
 *
 *  Tue Apr 29 14:30:00 2008
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


#ifndef _ZBNCUBEMAP_H
#define _ZBNCUBEMAP_H

class ZBNCubemap : public picogen::graphics::objects::abstract::IIntersectable
{
	private:
		typedef misc::prim::real real;
		typedef misc::geometrics::Vector3d Vector3d;
		typedef misc::geometrics::Ray Ray;
		typedef misc::geometrics::BoundingBox BoundingBox;
		typedef material::abstract::IBRDF IBRDF;
		typedef structs::intersection_t intersection_t;
		typedef graphics::image::color::Color Color;
		typedef picogen::misc::functions::vector_to_scalar::PerlinNoise PerlinNoise;

	private:

	public:
		ZBNCubemap();
		virtual ~ZBNCubemap();
		virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const;

};


#endif /* _ZBNCUBEMAP_H */

