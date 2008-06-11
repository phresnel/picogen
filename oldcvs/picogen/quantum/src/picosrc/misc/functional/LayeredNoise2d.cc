/***************************************************************************
 *            LayeredNoise2d.cc
 *
 *  Mon Jun 09 13:50:00 2008
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



#include <string>
#include <sstream>


#include <picogen/misc/functional/LayeredNoise2d.h>


namespace picogen {
    namespace misc {
        namespace functional {



            LayeredNoise2d::LayeredNoise2d (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun)
                        : ufun(ufun), vfun(vfun), noiseEvalFun(0), persistenceFun(0)
                        , enableBilinearNoiseMapFilter(true)
                        , noiseDepth(4)
                        , frequency(2)
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
                    } else if (name == string("layercount")) {
                        istringstream hmmm (parameters[name]);
                        hmmm >> noiseDepth;
                    } else if (name == string("persistence")) {
                        persistenceFun = new Function_R1_R1 (parameters[name]); // Note that this line may throw.
                    } else if (name == string("noisemapfilter")) {
                        istringstream hmmm (parameters[name]);
                        string filterType;
                        hmmm >> filterType;
                        if (filterType == "bilinear") {
                            enableBilinearNoiseMapFilter = true;
                        } else if (filterType == "nearest") {
                            enableBilinearNoiseMapFilter = false;
                        } else {
                            throw functional_general_exeption ("unknown filter type for 'noisemapfilter': '" + filterType + "' (only 'bilinear' and 'nearest' are supported)");
                        }
                    } else if (name == string("levelEvaluationFunction")) {
                        noiseEvalFun = new Function_R1_R1 (parameters[name]); // Note that this line may throw.
                    } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                    }
                }
                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                    throw functional_general_exeption ("the following parameters do not exist for 'LayeredNoise': "+nonExistantParameterNames);
                }

                // Do we already have a noise evaluation function?
                if (0 == noiseEvalFun) {
                    // No, create one.
                    noiseEvalFun = new Function_R1_R1("x"); // This line may throw.
                }

                // Do we already have a persistence function?
                if (0 == persistenceFun) {
                    // No, create one.
                    persistenceFun = new Function_R1_R1("0.5");  // This line may throw.
                }


                //====---- - - -  -   -    -      -
                // Initialise Noise.
                //====---- - - -  -   -    -      -

                srand (seed);

                unsigned int offsetLutNumBits = 8;
                offsetLutMask = 0;
                for (unsigned int u=0; u<offsetLutNumBits; ++u) {
                    offsetLutMask |= 1 << u;
                }
                --offsetLutMask;
                offsetLutSize = 1+offsetLutMask;
                offsetLut = new unsigned int [offsetLutSize];
                rngLut    = new real [offsetLutSize];

                for (unsigned int u=0; u<offsetLutSize; ++u) {
                    offsetLut [u] = static_cast<unsigned int> ( ( static_cast<real> (rand()) / static_cast<real> (RAND_MAX-1) ) * static_cast<real> (offsetLutSize) );
                    rngLut    [u] = -0.5 + static_cast<real> (rand()) / static_cast<real> (RAND_MAX);
                }

            }



            LayeredNoise2d::~LayeredNoise2d() {
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
                if (0 != noiseEvalFun) {
                    delete noiseEvalFun;
                }
                if (0 != persistenceFun ) {
                    delete persistenceFun;
                }
            }


// const float *vg = lot_vals[ (i+lot_ofs[ (j+lot_ofs[k&0xFE]) &0xFE]) &0xFE ];
//#define LN2D_RNG( u, v, depth ) rngLut [ ((v) + offsetLut [((u) + offsetLut [(depth) &offsetLutMask]) &offsetLutMask]) &offsetLutMask]
#define LN2D_LUTINDEX(u, v, depth )  (((v) + offsetLut [((u) + offsetLut [(depth) %offsetLutSize]) %offsetLutSize]) %offsetLutSize)
#define LN2D_RNG(u, v, depth ) rngLut [ LN2D_LUTINDEX (u, v, depth) ]

            real_t LayeredNoise2d::operator () (const real_t * const parameters) const {

                using namespace std;

                const real_t x = (*ufun) (parameters);
                const real_t y = (*vfun) (parameters);
                real_t domainScale = static_cast<real> (frequency);
                real_t rangeScale = 1.0;

                if (enableBilinearNoiseMapFilter) {
                    real sum = 0;
                    for (unsigned int depth=0; depth<noiseDepth; ++depth) {
                        const real         u__  = x - ::floor (x); // > [0..1)
                        const real         u_   = u__ * (domainScale-1);
                        const unsigned int u    = static_cast<unsigned int>(u_);
                        const unsigned int u1   = static_cast<unsigned int>(1 + u_);

                        const real         v__  = y - ::floor (y); // > [0..1)
                        const real         v_   = v__ * (domainScale-1);
                        const unsigned int v    = static_cast<unsigned int>(v_);
                        const unsigned int v1   = static_cast<unsigned int>(1 + v_);

                        // 0...........|1...........|2
                        // ............|............|
                        // ............|............|
                        // ............|............|
                        const real pu = ( u_ - static_cast<real> (u) );
                        const real pv = ( v_ - static_cast<real> (v) );

                        const real A = LN2D_RNG( u  , v  , depth);
                        const real B = LN2D_RNG( u1 , v  , depth);
                        const real C = LN2D_RNG( u  , v1 , depth);
                        const real D = LN2D_RNG( u1 , v1 , depth);

                        const real P = A*(1.0-pu) + B*(pu);
                        const real Q = C*(1.0-pu) + D*(pu);

                        const real Z = P*(1.0-pv) + Q*(pv);

                        const real H = (*noiseEvalFun) (Z * rangeScale);

                        rangeScale   = rangeScale * (*persistenceFun) (H);
                        domainScale *= 2.0;

                        sum += H;
                    }
                    return sum;
                } else {
                    real sum = 0;
                    for (unsigned int depth=0; depth<noiseDepth; ++depth) {
                        const real         u__  = x - ::floor (x); // > [0..1)
                        const real         u_   = u__ * (domainScale);
                        const unsigned int u    = static_cast<unsigned int>(u_);

                        const real         v__  = y - ::floor (y); // > [0..1)
                        const real         v_   = v__ * (domainScale);
                        const unsigned int v    = static_cast<unsigned int>(v_);

                        const real Z = LN2D_RNG( u, v, depth);

                        const real H = (*noiseEvalFun) (Z * rangeScale);

                        rangeScale   = rangeScale * (*persistenceFun) (H);
                        domainScale *= 2.0;

                        sum += H;
                    }
                    return sum;
                }
            }


        } // namespace functional {
    } // namespace misc {
} // namespace picogen {
