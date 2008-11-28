/***************************************************************************
 *            Layers.cc
 *
 *  2008-10-31
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


#include <picogen/picogen.h>
#include <picogen/misc/functional/Layers.h>


namespace picogen {
    namespace misc {
        namespace functional {



            Layers::Layers (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun)
                        : ufun(ufun), vfun(vfun)
                        , noiseEvalFun(0), persistenceFun(0), baseFun(0)
                        , frequency(2)
                        , depth (6)
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
                        hmmm >> depth;
                    } else if (name == string("persistence")) {
                        persistenceFun = new Function_R1_R1 (parameters[name]); // Note that this line may throw.
                    } else if (name == string("base")) {
                        baseFun = new Function_R2_R1 (parameters[name]); // Note that this line may throw.
                    } else if (name == string("levelEvaluationFunction")) {
                        noiseEvalFun = new Function_R1_R1 (parameters[name]); // Note that this line may throw.
                    } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                    }
                }
                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                    throw functional_general_exeption ("the following parameters do not exist for 'Layers': "+nonExistantParameterNames);
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

                // Do we already have a base function?
                if (0 == baseFun) {
                    // No, create one.
                    // So that this behaves like default LayeredNoise
                    baseFun = new Function_R2_R1("([2 Noise frequency(1)] xy)");  // This line may throw.
                }
            }



            Layers::~Layers() {
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
                if (0 != baseFun ) {
                    delete persistenceFun;
                }
            }

            real_t Layers::operator () (const real_t * const parameters) const {

                using namespace std;

                const real_t x = (*ufun) (parameters);
                const real_t y = (*vfun) (parameters);
                real_t domainScale = frequency;
                real_t rangeScale = 1.0;

                real sum = 0;
                for (unsigned int depth=0; depth<this->depth; ++depth) {
                    const real u  = x * domainScale;
                    const real v  = y * domainScale;

                    const real Z = (*baseFun) (u, v);

                    const real H = rangeScale * (*noiseEvalFun) (Z);

                    rangeScale  *= (*persistenceFun) (H);
                    domainScale *= 2.0;

                    sum += H;

                    //std::cout << H << ":" << sum << std::endl;
                }
                //exit (0);
                return sum;
            }


        } // namespace functional {
    } // namespace misc {
} // namespace picogen {
