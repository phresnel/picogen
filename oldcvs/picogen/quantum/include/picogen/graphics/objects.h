/***************************************************************************
 *            objects.h
 *
 *  Fri Oct 12 19:04:33 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
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


#ifndef _OBJECTS_H
#define _OBJECTS_H

namespace objects{

	namespace abstract{
		class IIntersectable{
				typedef misc::prim::real _real;
				typedef misc::geometrics::Vector3d _Vector3d;
				typedef misc::geometrics::Ray _Ray;
				typedef structs::intersection_t _intersection_t;
			public:
				virtual bool Intersect( param_out(_intersection_t,intersection), param_in(_Ray,ray) ) const = 0;
				virtual ~IIntersectable() {};
		};
		class IScene : public IIntersectable{
				typedef misc::prim::real real;
				typedef misc::geometrics::Vector3d Vector3d;
				typedef structs::intersection_t intersection_t;
			public:
				virtual void Insert( const IIntersectable*i ) = 0;
				virtual void Flush() = 0;
				virtual void Invalidate() = 0;
				virtual ~IScene() {}
		};

		//> similar to IScene, but constrained to naked triangles
		class ITriScene : public IIntersectable{
				typedef misc::prim::real real;
				typedef misc::geometrics::Vector3d Vector3d;
				typedef structs::intersection_t intersection_t;
			public:
				virtual void SetBRDF( const material::abstract::IBRDF *brdf ) = 0;
				virtual void Insert( param_in(Vector3d,A), param_in(Vector3d,B), param_in(Vector3d,C) ) = 0;
				virtual void Flush() = 0;
				virtual void Invalidate() = 0;
				virtual ~ITriScene() {}
		};

		class ISky{
				typedef misc::prim::real real;
				typedef misc::geometrics::Vector3d Vector3d;
				typedef misc::geometrics::Ray Ray;
				typedef structs::intersection_t intersection_t;
			public:
				virtual ~ISky() {};
				virtual void Shade( param_out(image::color::Color,color), param_in(Ray,ray) ) const = 0;
				virtual void SunShade( param_out(image::color::Color,color), param_in(Ray,ray) ) const = 0;
				virtual void SunSample( param_out(image::color::Color,color), param_out(Ray,ray), param_out(real,p), param_in(Vector3d,position) ) const = 0;
				virtual void AtmosphereShade( param_out(image::color::Color,color), param_in(image::color::Color,src_color), param_in(Ray,ray), real distance ) const = 0;
		};
	};

	#include "objects/LinearList.h"
	#include "objects/TriBIH.h"
	#include "objects/Preetham.h"
	#include "objects/SimpleHeightField.h"
	#include "objects/VelocityInstance.h"
	#include "objects/Sphere.h"
	#include "objects/Snow.h"
	#include "objects/Rain.h"
};
#endif /* _OBJECTS_H */
