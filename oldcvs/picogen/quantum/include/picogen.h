/***************************************************************************
 *            picogen.h
 *
 *  Thu Oct 11 19:25:29 2007
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


#ifndef _PICOGEN_H
#define _PICOGEN_H


#define XRT_VERSION_MAJOR 0
#define XRT_VERSION_MINOR 0
#define XRT_VERSION_PATCH 0

#define XRT_INCLUDE_EXPERIMENTAL 1
//#define XRT_DEBUG





#include <math.h>
#include <float.h>
#include <iostream>
#include <string>
#include <sstream>
#include <list>



#ifdef XRT_DEBUG
namespace xrt {
namespace debug {
static int _call_stack_index = 0;
static std::string _call_stack[0xFF];
};
};
#define XRT_CALL_STACK_PUSH( name ) { \
		if( xrt::debug::_call_stack_index>=0xFF ){\
			fprintf( stderr, "probably unbalanced calls to CALL_STACK_(PUSH|POP) or maybe too deep recursion\n" );\
			throw;\
		} \
		xrt::debug::_call_stack[xrt::debug::_call_stack_index++] = name;\
	}
#define XRT_CALL_STACK_POP()        { xrt::debug::_call_stack_index--; \
		if(xrt::debug::_call_stack_index<0){ fprintf( stderr, "unbalanced calls to CALL_STACK_(PUSH|POP)\n" ); throw; } }
#define XRT_CALL_STACK_LENGTH()         xrt::debug::_call_stack_index
#define XRT_CALL_STACK_GET_STR( index ) xrt::debug::_call_stack[index]
#else
# define XRT_CALL_STACK_PUSH( name )
# define XRT_CALL_STACK_POP()
# define XRT_CALL_STACK_LENGTH()  0
# define XRT_CALL_STACK_GET_STR( index ) ""
#endif // ifdef XRT_DEBUG





#define param_out(type,name) type &name
#define param_in(type,name)  const type &name

namespace picogen {

namespace misc {
namespace string_helpers {
template <class T> inline std::string tostring( T val ) {
    XRT_CALL_STACK_PUSH( "template <class T> inline std::string tostring( T val )" );
    std::stringstream s;
    s << val;
    XRT_CALL_STACK_POP();
    return s.str();
}
};
#include <picogen/misc/exceptions.h>
namespace prim {
typedef double real;
};

namespace constants {
typedef enum side {
    inside = -1,
    noside = 0,
    outside = 1
};
static const prim::real epsilon = 0.0001;
static const prim::real real_max = FLT_MAX;
static const prim::real real_min = FLT_MIN;
static const prim::real pi = 3.14159265f;
};
#include <picogen/misc/geometrics.h>
#include <picogen/misc/functions.h> // << obsolete, take "functional.h"
#include <picogen/misc/functional.h>
#include <picogen/misc/templates.h>
};

namespace graphics {
#include <picogen/graphics/cameras.h>
#include <picogen/graphics/image.h>
#include <picogen/graphics/material.h>

namespace structs {
typedef struct _intersection_t {
    image::color::Color color;
    misc::prim::real t;
    misc::constants::side side;
    misc::geometrics::Vector3d normal;
    const material::abstract::IBRDF *brdf;
    misc::prim::real L_e;
} intersection_t;
};

#include <picogen/graphics/objects.h>
#include <picogen/graphics/samplers.h>
};

#include <picogen/misc/namespace_mnemonics.h>
};



#endif /* _PICOGEN_H */
