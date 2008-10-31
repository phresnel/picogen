/***************************************************************************
 *            Noise2d.cc
 *
 *  2008-10-31
 *  Copyright 2008  Sebastian Mach
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

#include <string>
#include <sstream>


#include <picogen/picogen.h>
#include <picogen/misc/functional/Noise2d.h>


namespace picogen {
    namespace misc {
        namespace functional {



            Noise2d::Noise2d (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun)
                        : ufun(ufun), vfun(vfun)
                        , filter (cosine)
                        , frequency (8.0)
            {
                using namespace std;
                typedef map<string,string> Map;

                unsigned int seed=42;

                //====---- - - -  -   -    -      -
                // Scan Parameters.
                //====---- - - -  -   -    -      -
                string nonExistantParameterNames (""); // To collect together parameter names that don't exist, for dumping errors in the end.

                for ( Map::const_iterator it=parameters.begin(); it!=parameters.end(); ++it ) {
                    const string name = it->first;
                    /// \todo Add some shorter Mnenomics.
                    if (name == string("seed")) {
                        istringstream hmmm (parameters[name]);
                        hmmm >> seed;
                    } else if (name == string("frequency") ) {
                        istringstream hmmm (parameters[name]);
                        hmmm >> frequency;
                    } else if (name == string("filter")) {
                        istringstream hmmm (parameters[name]);
                        string filterType;
                        hmmm >> filterType;
                        if (filterType == "bilinear") {
                            filter = bilinear;
                        } else if (filterType == "nearest") {
                            filter = nearest;
                        } else if (filterType == "cosine") {
                            filter = cosine;
                        } else {
                            throw functional_general_exeption ("unknown filter type for 'noisemapfilter': '" + filterType + "' (only 'bilinear' and 'nearest' are supported)");
                        }
                    } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                    }
                }
                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                    throw functional_general_exeption ("the following parameters do not exist for 'LayeredNoise': "+nonExistantParameterNames);
                }

                //====---- - - -  -   -    -      -
                // Initialise Noise.
                //====---- - - -  -   -    -      -

                ::picogen::generators::rng::MersenneTwister twister (seed);

                const unsigned int offsetLutNumBits = 8;
                offsetLutMask = 0;
                for (unsigned int u=0; u<offsetLutNumBits; ++u) {
                    offsetLutMask |= 1 << u;
                }
                offsetLutSize = offsetLutMask+1;
                if (false) {
                    for (unsigned int i=0; i<300; ++i) {
                        std::cout << i << ":" << (i&offsetLutMask) << std::endl;
                    }
                }
                offsetLut = new unsigned int [offsetLutSize];
                rngLut    = new real [offsetLutSize];

                for (unsigned int u=0; u<offsetLutSize; ++u) {
                    offsetLut [u] = static_cast<unsigned int> ((twister.randf()) * static_cast<real> (offsetLutSize));
                    rngLut    [u] = -0.5 + 1.0 * twister.randf();
                }

            }



            Noise2d::~Noise2d() {
                if (0 != offsetLut) {
                    delete [] offsetLut;
                }
                if (0 != rngLut) {
                    delete [] rngLut;
                }
                if (0 != vfun) {
                    delete vfun;
                }
                if (0 != ufun) {
                    delete ufun;
                }
            }


// const float *vg = lot_vals[ (i+lot_ofs[ (j+lot_ofs[k&0xFE]) &0xFE]) &0xFE ];
//#define LN2D_LUTINDEX(u, v, depth )  (((v) + offsetLut [((u) + offsetLut [(depth) %offsetLutSize]) %offsetLutSize]) %offsetLutSize)
//#define LN2D_RNG(u, v, depth ) rngLut [ LN2D_LUTINDEX (u, v, depth) ]
#define LN2D_RNG( u, v, depth ) rngLut [ ((v) + offsetLut [((u) + offsetLut [(depth) &offsetLutMask]) &offsetLutMask]) &offsetLutMask]

            namespace {
                template <typename RT, typename T> RT floor (const T &v) {
                    assert (static_cast<int>(1.75) == 1);
                    assert (static_cast<int>(1.5) == 1);
                    assert (static_cast<int>(1.25) == 1);
                    assert (static_cast<int>(-0.75) == 0);
                    assert (static_cast<int>(-0.5) == 0);
                    assert (static_cast<int>(-0.25) == 0);
                    return (RT)(int)(v<0 ? v-1 : v);
                }
            }

            real_t Noise2d::operator () (const real_t * const parameters) const {

                using namespace std;

                const real_t x = (*ufun) (parameters);
                const real_t y = (*vfun) (parameters);
                const int depth = 0;

                switch (filter) {
                    case bilinear: {
                        const real u__  = x;
                        const real u_   = u__ * frequency;
                        const int u     = floor <int> (u_);
                        const int u1    = floor <int> (1 + u_);

                        const real v__  = y;
                        const real v_   = v__ * frequency;
                        const int  v    = floor <int> (v_);
                        const int  v1   = floor <int> (1 + v_);

                        const real pu = ( u_ - static_cast<real> (u) );
                        const real pv = ( v_ - static_cast<real> (v) );

                        const real A = LN2D_RNG( u  , v  , 0);
                        const real B = LN2D_RNG( u1 , v  , 0);
                        const real C = LN2D_RNG( u  , v1 , 0);
                        const real D = LN2D_RNG( u1 , v1 , 0);

                        const real P = A*(1.0-pu) + B*(pu);
                        const real Q = C*(1.0-pu) + D*(pu);

                        const real Z = P*(1.0-pv) + Q*(pv);
                        return Z;
                    }
                    case cosine: {
                        const real u__  = x;
                        const real u_   = u__ * frequency;
                        const int u     = floor <int> (u_);
                        const int u1    = floor <int> (1 + u_);

                        const real v__  = y;
                        const real v_   = v__ * frequency;
                        const int  v    = floor <int> (v_);
                        const int  v1   = floor <int> (1 + v_);

                        /*
                            :from Hugo Elias (http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)

                            function Cosine_Interpolate(a, b, x)
                                ft = x * 3.1415927
                                f = (1 - cos(ft)) * .5

                                return  a*(1-f) + b*f
                            end of function
                        */
                        const real pu_ = ::picogen::constants::pi * (u_ - static_cast<real> (u));
                        const real pu  = (1.0 - cos (pu_)) * 0.5;
                        const real pv_ = ::picogen::constants::pi * (v_ - static_cast<real> (v));
                        const real pv  = (1.0 - cos (pv_)) * 0.5;

                        const real A = LN2D_RNG( u  , v  , 0);
                        const real B = LN2D_RNG( u1 , v  , 0);
                        const real C = LN2D_RNG( u  , v1 , 0);
                        const real D = LN2D_RNG( u1 , v1 , 0);

                        const real P = A*(1.0-pu) + B*(pu);
                        const real Q = C*(1.0-pu) + D*(pu);

                        const real Z = P*(1.0-pv) + Q*(pv);
                        return Z;
                    }
                    case nearest: {
                        const real         u__  = x;// - ::floor (x); // > [0..1)
                        const real         u_   = u__ * frequency;
                        const unsigned int u    = static_cast<unsigned int>(u_);

                        const real         v__  = y;// - ::floor (y); // > [0..1)
                        const real         v_   = v__ * frequency;
                        const unsigned int v    = static_cast<unsigned int>(v_);

                        const real Z = LN2D_RNG( u, v, depth);
                        return Z;
                    }
                }
                return 0.0;
            }


        } // namespace functional {
    } // namespace misc {
} // namespace picogen {
