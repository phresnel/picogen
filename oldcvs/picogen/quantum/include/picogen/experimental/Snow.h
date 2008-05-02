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


#ifndef _SNOW_H
#define _SNOW_H

namespace picogen{ namespace graphics{ namespace objects{

class Snow : public picogen::graphics::objects::abstract::IIntersectable
{
	private:
        typedef picogen::misc::prim::real real;
		typedef picogen::misc::geometrics::Vector3d Vector3d;
		typedef picogen::misc::geometrics::Ray Ray;
		typedef picogen::misc::geometrics::BoundingBox BoundingBox;
		typedef picogen::graphics::material::abstract::IBRDF IBRDF;
		typedef picogen::graphics::structs::intersection_t intersection_t;
		typedef picogen::graphics::image::color::Color Color;
		typedef picogen::misc::functions::vector_to_scalar::PerlinNoise PerlinNoise;

	private:

        typedef struct SnowSphere{
            real minDistance, maxDistance;
            PerlinNoise snowMap;
        };
        unsigned int numSnowSpheres;
        SnowSphere *snowSpheres;

        /*real timeexp;
        Vector3d velocity;*/

	public:
		Snow();
		virtual ~Snow();
		virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const;

		/*void SetTimeExponent( misc::prim::real t )
		{
			timeexp = t;
		}
		void SetVelocity( param_in( misc::geometrics::Vector3d, V ) )
		{
			velocity = V;
		}*/
};

} } } // namespace picogen{ namespace graphics{ namespace objects{

#endif /* _SNOW_H */

