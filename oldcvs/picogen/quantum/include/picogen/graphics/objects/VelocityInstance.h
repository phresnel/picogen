/***************************************************************************
 *            VelocityInstance.h
 *
 *  Thu Nov  8 01:10:06 2007
 *  Copyright  2007  Sebastian Mach
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


#ifndef _VELOCITYINSTANCE_H
#define _VELOCITYINSTANCE_H

class VelocityInstance : public abstract::IIntersectable{
		abstract::IIntersectable *intersectable;
		misc::geometrics::Vector3d velocity, offset;
		misc::prim::real timeexp;
	public:
		VelocityInstance() : intersectable(NULL), timeexp(1.0) {}

		void SetTimeExponent( misc::prim::real t )
		{
			timeexp = t;
		}
		void SetVelocity( param_in( misc::geometrics::Vector3d, V ) )
		{
			velocity = V;
		}
		void SetOffset( param_in( misc::geometrics::Vector3d, o ) )
		{
			offset = o;
		}
		void SetIntersectable( const abstract::IIntersectable* I )
		{
			intersectable = const_cast<abstract::IIntersectable*>(I);
		}

		virtual bool Intersect(
			param_out(structs::intersection_t,intersection), param_in(misc::geometrics::Ray,ray)
		) const
		{
			using misc::geometrics::Ray;
			using misc::prim::real;

			if( NULL == intersectable )
				return false;
			Ray newRay;
			newRay.x() = ray.x() + offset + velocity * ( powf( real(rand())/real(RAND_MAX), timeexp ) );
			newRay.w() = ray.w();
			return intersectable->Intersect( intersection, newRay );
		}
};


#endif /* _VELOCITYINSTANCE_H */
