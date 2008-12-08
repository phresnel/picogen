/***************************************************************************
 *            HSShader.h
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


#ifndef _HSSHADER_H
#define _HSSHADER_H

namespace picogen {
    namespace graphics {
        namespace shaders {
            class HeightSlangShader : public picogen::graphics::material::abstract::IShader {
                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;

                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::objects::abstract::IScene IScene;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::color::Color Color;

                    typedef ::picogen::misc::functional::Function_R6_R1_Refcounted Function_R6_R1_Refcounted;
                    typedef ::picogen::misc::functional::Function_R6_R1 Function_R6_R1;
                    
                    ::boost::intrusive_ptr <Function_R6_R1_Refcounted> hs;
                    ::std::vector <IShader*> shaders;
                    const real numShaders;
                    real sx, sy, sz;

                    real min_h, max_h;

                public:
                    virtual ~HeightSlangShader();
                    HeightSlangShader (const ::boost::intrusive_ptr <Function_R6_R1_Refcounted> hs, const std::vector <IShader*> &shaders);
                    virtual void shade (
                        picogen::graphics::color::Color &color,
                        const picogen::geometrics::Vector3d &normal,
                        const picogen::geometrics::Vector3d &position
                    ) const ;
            };
        }
    }
}

#endif
