//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2009  Sebastian Mach
//
//      email: a@b.c, with a=phresnel, b=gmail, c=com
//        www: http://phresnel.org
//             http://picogen.org
//
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the  Free  Software  Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed  in the hope that it will be useful, but
//    WITHOUT  ANY  WARRANTY;   without   even  the  implied  warranty  of
//    MERCHANTABILITY  or  FITNESS FOR A PARTICULAR  PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy  of  the  GNU General Public License
//    along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef LAYEREDNOISE2DDEF_HH_INCLUDED_20090813
#define LAYEREDNOISE2DDEF_HH_INCLUDED_20090813

#include <string>
#include <sstream>

#include "layerednoise2d.hh"

namespace quatsch {  namespace configurable_functions {


template <typename FUNCTION, typename COMPILER>
LayeredNoise2d <FUNCTION, COMPILER> :: LayeredNoise2d (
        ::std::map<std::string,std::string>& static_parameters,
        ::std::vector <FunctionPtr>& runtime_parameters
) {

        {
                using namespace quatsch::backend::est;
                if (runtime_parameters.size() < 2)
                        throw insufficient_number_of_operands_exception (2);
                if (runtime_parameters.size() > 2)
                        throw too_many_operands_exception (2);

                ufun = runtime_parameters[0];
                vfun = runtime_parameters[1];
        }
        /*::LayeredNoise2d (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun)
        : ufun(ufun), vfun(vfun), noiseEvalFun(0), persistenceFun(0)
        //, enableBilinearNoiseMapFilter(true)
        , filter (cosine)
        , noiseDepth(6)
        , frequency(2)*/

        using namespace std;
        typedef map<string,string> Map;

        unsigned int seed=42;

        //====---- - - -  -   -    -      -
        // Scan Parameters.
        //====---- - - -  -   -    -      -
        string nonExistantParameterNames (""); // To collect together parameter names that don't exist, for dumping errors in the end.

        for (Map::const_iterator it=static_parameters.begin();
             it!=static_parameters.end();
             ++it
        ) {
                const string name = it->first;
                /// \todo Add some shorter Mnenomics.
                if (name == string("seed")) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> seed;
                } else if (name == string("frequency") ) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> frequency;
                } else if (name == string("layercount")) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> noiseDepth;
                } else if (name == string("persistence")) {
                        //persistenceFun = new Function_R1_R1 (static_parameters[name]); // Note that this line may throw.
                        typename compiler_t::ConfigurableFunctionsMap addfuns;
                        persistenceFun = compiler_t::compile ("h", static_parameters[name], addfuns, std::cerr);
                } else if (name == string("filter")) {
                        istringstream hmmm (static_parameters[name]);
                        string filterType;
                        hmmm >> filterType;
                        if (filterType == "bilinear") {
                                filter = bilinear;
                        } else if (filterType == "nearest") {
                                filter = nearest;
                        } else if (filterType == "cosine") {
                                filter = cosine;
                        } else {
                                throw general_exception ("LayeredNoise2d: unknown filter type for 'filter': '" + filterType + "' (only 'bilinear','nearest','cosine' are supported)");
                        }
                } else if (name == string("levelEvaluationFunction")) {
                        typename compiler_t::ConfigurableFunctionsMap addfuns;
                        noiseEvalFun = compiler_t::compile ("h", static_parameters[name], addfuns, std::cerr);
                } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                }
        }

        // Any parameters set that we don't know?
        if (nonExistantParameterNames != "") {
                throw general_exception ("the following parameters do not exist for 'LayeredNoise': "+nonExistantParameterNames);
        }

        // Do we already have a noise evaluation function?
        if (0 == noiseEvalFun) {
                // No, create one.
                typename compiler_t::ConfigurableFunctionsMap addfuns;
                noiseEvalFun = compiler_t::compile ("h", "h", addfuns, std::cerr);
        }

        // Do we already have a persistence function?
        if (0 == persistenceFun) {
                // No, create one.
                typename compiler_t::ConfigurableFunctionsMap addfuns;
                persistenceFun = compiler_t::compile ("h", "0.5", addfuns, std::cerr);
        }


        //====---- - - -  -   -    -      -
        // Initialise Noise.
        //====---- - - -  -   -    -      -
        {
                using boost::shared_array;
                using namespace kallisto;
                random::MersenneTwister<scalar_t, 0, 1> mt (seed);

                const unsigned int offsetLutNumBits = 8;
                offsetLutMask = 0;
                for (unsigned int u=0; u<offsetLutNumBits; ++u) {
                        offsetLutMask |= 1 << u;
                }
                offsetLutSize = offsetLutMask+1;
                if (false) {
                        std::cout << "{{{{" << offsetLutMask << ":" << offsetLutSize << "}}}}";
                        for (unsigned int i=0; i<300; ++i) {
                                std::cout << i << ":" << (i&offsetLutMask) << std::endl;
                        }
                }
                offsetLut = boost::shared_array<unsigned int> (new unsigned int [offsetLutSize]);
                rngLut    = boost::shared_array<scalar_t> (new scalar_t [offsetLutSize]);

                for (unsigned int u=0; u<offsetLutSize; ++u) {
                        offsetLut [u] = static_cast<unsigned int> ((mt.rand()) * static_cast<scalar_t> (offsetLutSize));
                        rngLut    [u] = -0.5 + 1.0 * mt.rand();
                }
        }
}


template <typename FUNCTION, typename COMPILER>
LayeredNoise2d <FUNCTION, COMPILER> :: ~LayeredNoise2d() {
        /*
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
        }*/
}


// const scalar_t *vg = lot_vals[ (i+lot_ofs[ (j+lot_ofs[k&0xFE]) &0xFE]) &0xFE ];
//#define LN2D_LUTINDEX(u, v, depth )  (((v) + offsetLut [((u) + offsetLut [(depth) %offsetLutSize]) %offsetLutSize]) %offsetLutSize)
//#define LN2D_RNG(u, v, depth ) rngLut [ LN2D_LUTINDEX (u, v, depth) ]
//#define LN2D_RNG( u, v, depth ) rngLut [ ((v) + offsetLut [((u) + offsetLut [(depth) &offsetLutMask]) &offsetLutMask]) &offsetLutMask]

#ifdef LN2D_RNG
#error
#endif

#define LN2D_RNG( u, v, depth ) \
        rngLut [ \
                ((v) + offsetLut [  \
                        ((u) + offsetLut [  \
                                (depth)  \
                                & offsetLutMask  \
                        ])  \
                        & offsetLutMask  \
                ])  \
                & offsetLutMask  \
        ]


//scalar_t operator () (const parameters_t &) const;
//configurable-functions/layerednoise2ddef.hh:212: error: prototype for 'typename quatsch::configurable_functions::Noise2d<FUNCTION, COMPILER>::scalar_t quatsch::configurable_functions::LayeredNoise2d<FUNCTION, COMPILER>::operator()(const typename quatsch::configurable_functions::LayeredNoise2d<FUNCTION, COMPILER>::parameters_t&) const' does not match any in class 'quatsch::configurable_functions::LayeredNoise2d<FUNCTION, COMPILER>'
//configurable-functions/layerednoise2d.hh:101:    error: candidate is: virtual typename FUNCTION::scalar_t quatsch::configurable_functions::LayeredNoise2d<FUNCTION, COMPILER>::operator()(const typename FUNCTION::parameters_t&) const


template <typename FUNCTION, typename COMPILER>
typename LayeredNoise2d <FUNCTION, COMPILER>::scalar_t
LayeredNoise2d <FUNCTION, COMPILER>::operator () (
        const typename LayeredNoise2d <FUNCTION, COMPILER>::parameters_t& parameters
) const {

        using namespace std;

        const scalar_t x = (*ufun) (parameters);
        const scalar_t y = (*vfun) (parameters);
        scalar_t domainScale = frequency;
        scalar_t rangeScale = 1.0;

        switch (filter) {
        case bilinear: {
                scalar_t sum = 0;
                for (unsigned int depth=0; depth<noiseDepth; ++depth) {
                        const scalar_t u__  = x;
                        const scalar_t u_   = u__ * (domainScale);
                        const int u     = floor <int> (u_);
                        const int u1    = floor <int> (1 + u_);

                        const scalar_t v__  = y;
                        const scalar_t v_   = v__ * (domainScale);
                        const int  v    = floor <int> (v_);
                        const int  v1   = floor <int> (1 + v_);

                        const scalar_t pu = ( u_ - static_cast<scalar_t> (u) );
                        const scalar_t pv = ( v_ - static_cast<scalar_t> (v) );

                        const scalar_t A = LN2D_RNG( u  , v  , 0);
                        const scalar_t B = LN2D_RNG( u1 , v  , 0);
                        const scalar_t C = LN2D_RNG( u  , v1 , 0);
                        const scalar_t D = LN2D_RNG( u1 , v1 , 0);

                        const scalar_t P = A*(1.0-pu) + B*(pu);
                        const scalar_t Q = C*(1.0-pu) + D*(pu);

                        const scalar_t Z = P*(1.0-pv) + Q*(pv);

                        const scalar_t Z_ [] = { Z };
                        const scalar_t H = rangeScale * (*noiseEvalFun) (Z_);

                        const scalar_t H_ [] = { H };
                        rangeScale  *= (*persistenceFun) (H_);
                        domainScale *= 2.0;

                        sum += H;
                }
                return sum;
        }
        case cosine: { // from Hugo Elias (http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)
                using constants::pi;

                scalar_t sum = 0;
                for (unsigned int depth=0; depth<noiseDepth; ++depth) {
                        const scalar_t u__  = x;
                        const scalar_t u_   = u__ * (domainScale);
                        const int u     = floor <int> (u_);
                        const int u1    = floor <int> (1 + u_);

                        const scalar_t v__  = y;
                        const scalar_t v_   = v__ * (domainScale);
                        const int  v    = floor <int> (v_);
                        const int  v1   = floor <int> (1 + v_);

                        /*
                        function Cosine_Interpolate(a, b, x)
                            ft = x * 3.1415927
                            f = (1 - cos(ft)) * .5

                            return  a*(1-f) + b*f
                        end of function
                        */
                        const scalar_t pu_ = pi * (u_ - static_cast<scalar_t> (u));
                        const scalar_t pu  = (1.0 - cos (pu_)) * 0.5;
                        const scalar_t pv_ = pi * (v_ - static_cast<scalar_t> (v));
                        const scalar_t pv  = (1.0 - cos (pv_)) * 0.5;

                        const scalar_t A = LN2D_RNG( u  , v  , 0);
                        const scalar_t B = LN2D_RNG( u1 , v  , 0);
                        const scalar_t C = LN2D_RNG( u  , v1 , 0);
                        const scalar_t D = LN2D_RNG( u1 , v1 , 0);

                        const scalar_t P = A*(1.0-pu) + B*(pu);
                        const scalar_t Q = C*(1.0-pu) + D*(pu);

                        const scalar_t Z = P*(1.0-pv) + Q*(pv);

                        const scalar_t Z_ [] = { Z };
                        const scalar_t H = rangeScale * (*noiseEvalFun) (Z_);

                        const scalar_t H_ [] = { H };
                        rangeScale  *= (*persistenceFun) (H_);
                        domainScale *= 2.0;

                        sum += H;
                }
                return sum;
        }
        case nearest: {
                scalar_t sum = 0;
                for (unsigned int depth=0; depth<noiseDepth; ++depth) {
                        const scalar_t         u__  = x;// - ::floor (x); // > [0..1)
                        const scalar_t         u_   = u__ * (domainScale);
                        const unsigned int u    = static_cast<unsigned int>(u_);

                        const scalar_t         v__  = y;// - ::floor (y); // > [0..1)
                        const scalar_t         v_   = v__ * (domainScale);
                        const unsigned int v    = static_cast<unsigned int>(v_);

                        const scalar_t Z = LN2D_RNG( u, v, 0);
                        const scalar_t Z_ [] = { Z };

                        const scalar_t H = rangeScale * (*noiseEvalFun) (Z_);

                        const scalar_t H_ [] = { H };
                        rangeScale  *= (*persistenceFun) (H_);
                        domainScale *= 2.0;

                        sum += H;
                }
                return sum;
            }
        }
        return 0.0;
}

#undef LN2D_RNG

} }

#endif // LAYEREDNOISE2DDEF_HH_INCLUDED_20090813
