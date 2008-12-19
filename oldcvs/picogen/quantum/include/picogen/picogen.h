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
#define XRT_VERSION_MINOR 2
#define XRT_VERSION_PATCH 0 

#define XRT_INCLUDE_EXPERIMENTAL 1
//#define XRT_DEBUG




// Std.
#include <cmath>
#include <cfloat>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>

// #define NDEBUG
#include <cassert>


// Boost.
#include <boost/intrusive_ptr.hpp>



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
    namespace graphics {
        namespace structs {
        }
        namespace objects {
            namespace templates {
            }
            namespace abstract {
                class IIntersectable;
                class IScene;
                class ITriScene;
                class ISky;
            }
        }
        namespace image {
            namespace color {
            }
        }
        namespace material {
            namespace abstract {
            }
        }
    }
    namespace misc {
        namespace prim {
        }
        namespace constants {
        }
        namespace geometrics {
        }
    }
}


namespace picogen {

    namespace misc {
        namespace string_helpers {
            template <class T> inline std::string tostring (T val) {
                XRT_CALL_STACK_PUSH ("template <class T> inline std::string tostring( T val )");
                std::stringstream s;
                s << val;
                XRT_CALL_STACK_POP();
                return s.str();
            }
        } // namespace string_helpers {
    } // namespace misc
} // namespace picogen

#include <picogen/misc/exceptions.h>

namespace picogen {
    typedef double real;

    typedef uint32_t uint32;
    typedef int32_t int32;

    namespace constants {
        typedef enum {
            inside = -1,
            noside = 0,
            outside = 1
        } side;
        static const real epsilon = 0.0001;
        static const real real_max = FLT_MAX;
        static const real real_min = FLT_MIN;
        static const real pi = 3.14159265f;
        static const real e = 2.71828182845904523536;
    }
    
    namespace math {
        template <typename RT, typename T> RT floor (const T &v) {
            assert (static_cast<int>(1.75) == 1);
            assert (static_cast<int>(1.5) == 1);
            assert (static_cast<int>(1.25) == 1);
            assert (static_cast<int>(-0.75) == 0);
            assert (static_cast<int>(-0.5) == 0);
            assert (static_cast<int>(-0.25) == 0);
            return static_cast <RT> (static_cast <int> (v<0 ? v-1 : v));
        }
    }
} // namespace picogen {


#include <picogen/generators/generators.h>

#include <picogen/misc/geometrics.h>
#include <picogen/misc/templates.h>

#include <picogen/graphics/image.h>
#include <picogen/graphics/material.h>


namespace picogen {
    namespace graphics {
        namespace objects {
            class CloudAdapter;
        }
        namespace structs {
            class intersection_t {
                private:
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                public:
                    color::Color color;
                    ::picogen::real t;
                    ::picogen::constants::side side;
                    ::picogen::geometrics::Vector3d normal;
                    IBRDF const *brdf;
                    ::picogen::real L_e;
            };
            struct sample {
                const unsigned int x, y;
                const real fx, fy;
                const geometrics::Ray cameraRay;
                const ::picogen::graphics::objects::abstract::IIntersectable * const intersectable;
                const ::picogen::graphics::objects::abstract::ISky * const skyShader; /* for a phase of transition only! to dissappear! */
                //const ::picogen::graphics::objects::CloudAdapter * const cloudAdapter; /* for a phase of transition only! to dissappear! */

                intersection_t *primaryIntersection;
                color::Color color;

                sample (
                    unsigned int x, unsigned int y,
                    real fx, real fy,
                    geometrics::Ray cameraRay,
                    const ::picogen::graphics::objects::abstract::IIntersectable * const intersectable,
                    const ::picogen::graphics::objects::abstract::ISky * const skyShader
                )
                :   x (x), y (y),
                    fx (fx), fy (fy),
                    cameraRay (cameraRay),
                    intersectable (intersectable),
                    skyShader (skyShader),
                    //cloudAdapter (cloudAdapter),
                    primaryIntersection (0),
                    color()
                {}
            };
        }
    }
}

#include <picogen/misc/functions.h> // << obsolete, take "functional.h"
#include <picogen/misc/functional.h>

// TODO : inline the following header files (like with shaders/* below)
#include <picogen/graphics/cameras.h>
#include <picogen/graphics/film.h>
#include <picogen/graphics/objects.h>
#include <picogen/graphics/integrators.h>

#include <picogen/graphics/shaders/ConstantShader.h>
#include <picogen/graphics/shaders/HSShader.h>

#include <picogen/common.h>



#endif /* _PICOGEN_H */
