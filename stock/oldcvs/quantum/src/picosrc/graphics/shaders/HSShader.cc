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


#include <picogen/picogen.h>


namespace picogen {
    namespace graphics {
        namespace shaders {
            
            
            
            HeightSlangShader::~HeightSlangShader() {
                for (std::vector <IShader*>::const_iterator it=shaders.begin(); it != shaders.end(); ++it) {
                    if (0 != *it)
                        delete *it;
                }
            }
                
            
            
            HeightSlangShader::HeightSlangShader (const ::boost::intrusive_ptr <Function_R6_R1_Refcounted> hs, const std::vector <IShader*> &shaders)
                : hs (hs)
                , shaders (shaders)
                , numShaders (static_cast <real> (shaders.size()-1))
                //, size (static_cast <real> (shaders.size()))
                , sx (1), sy (1), sz (1)
            {
                /*min_h = picogen::constants::real_max;
                max_h = picogen::constants::real_min;
                for (int i=0; i<10000; ++i) {
                    Vector3d p(
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX),
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX),
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX)
                    );
                    Vector3d n(
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX),
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX),
                        static_cast <real> (rand()) / static_cast <real> (RAND_MAX)
                    );
                    const real h = (*hs) (u, v);
                }*/
            }
            
            
            
            void HeightSlangShader::shade (
                picogen::graphics::color::Color &color,
                const picogen::geometrics::Vector3d &normal,
                const picogen::geometrics::Vector3d &position
            ) const {
                if (shaders.size()==0) {
                    color = Color (1.0, 1.0, 1.0);
                    return;
                }
                if (shaders.size()==1) {
                    shaders [0]->shade (color, normal, position);
                }
                const real f_ = (*hs) (position [0]*sx, position [2]*sz, position [1]*sy, normal [0], normal [2], normal [1]) * numShaders;
                const int a = ::picogen::math::floor <int> (f_);
                if (a<0) {
                    shaders [0]->shade (color, normal, position);
                    return;
                }
                if (a>=static_cast<int> (shaders.size())-1) {
                    shaders [shaders.size()-1]->shade (color, normal, position);
                    return;
                }
                const int b = 1+a;
                const real f = f_ - static_cast <real> (a);

                Color A, B;
                shaders [a]->shade (A, normal, position);
                shaders [b]->shade (B, normal, position);
                color = A * (1-f) + B * f;
            }
            
            
            
        }
    }
}
