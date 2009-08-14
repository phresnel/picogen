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

#ifndef NOISE2DDEF_HH_INCLUDED_20090813
#define NOISE2DDEF_HH_INCLUDED_20090813

#include <string>
#include <sstream>


#include "noise2d.hh"


namespace quatsch {  namespace configurable_functions {

template <typename FUNCTION, typename COMPILER>
Noise2d <FUNCTION, COMPILER> :: Noise2d (
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

        typename compiler_t::ConfigurableFunctionsMap addfuns;
        using namespace std;
        typedef map<string,string> Map;

        uint32_t seed=42;
        //frequency = 

        //====---- - - -  -   -    -      -
        // Scan Parameters.
        //====---- - - -  -   -    -      -
        // To collect together parameter names that don't exist,
        // and for dumping errors in the end.
        string nonExistantParameterNames ("");

        for (Map::const_iterator it=static_parameters.begin() ;
             it!=static_parameters.end() ;
             ++it
        ) {
                const string name = it->first;
                if (name == string("seed")) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> seed;
                } else if (name == string("frequency") ) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> frequency;
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
                                throw general_exception (
                                "unknown filter type for 'noisemapfilter': '" +
                                filterType + "' (only 'bilinear' and 'nearest' "
                                "are supported)"
                                );
                        }
                } else {
                        nonExistantParameterNames +=
                                (nonExistantParameterNames!=""?", ":"") +
                                string("'") +
                                it->first +
                                string("'");
                }
        }
        // Any parameters set that we don't know?
        if (nonExistantParameterNames != "") {
                throw general_exception ("the following parameters do not "
                        "exist for 'Noise': " + nonExistantParameterNames);
        }

        //====---- - - -  -   -    -      -
        // Initialise Noise.
        //====---- - - -  -   -    -      -
        {
                using boost::shared_array;
                using namespace kallisto;
                random::MersenneTwister<float, 0, 1> mt (seed);

                const unsigned int offsetLutNumBits = 8;
                offsetLutMask = 0;
                for (unsigned int u=0; u<offsetLutNumBits; ++u) {
                        offsetLutMask |= 1 << u;
                }
                offsetLutSize = offsetLutMask+1;
                offsetLut = shared_array<unsigned int>(
                                new unsigned [offsetLutSize]);
                rngLut    = shared_array<scalar_t>(
                                new scalar_t [offsetLutSize]);

                for (unsigned int u=0; u<offsetLutSize; ++u) {
                        offsetLut [u] = static_cast<unsigned int> ((mt.rand()) *
                                        static_cast<scalar_t> (offsetLutSize));
                        rngLut    [u] = -0.5 + 1.0 * mt.rand();
                }
        }
}



template <typename FUNCTION, typename COMPILER>
Noise2d <FUNCTION, COMPILER> :: ~Noise2d() {
        /*if (0 != offsetLut) {
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
        }*/
}

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


template <typename FUNCTION, typename COMPILER>
typename Noise2d <FUNCTION, COMPILER>::scalar_t
Noise2d <FUNCTION, COMPILER>::operator () (
        const Noise2d <FUNCTION, COMPILER>::parameters_t& parameters
) const {

        using namespace std;

        const scalar_t x = (*ufun) (parameters);
        const scalar_t y = (*vfun) (parameters);
        const int depth = 0;

        switch (filter) {
        case bilinear: {
                const scalar_t u__  = x;
                const scalar_t u_   = u__ * frequency;
                const int u     = floor <int> (u_);
                const int u1    = floor <int> (1 + u_);

                const scalar_t v__  = y;
                const scalar_t v_   = v__ * frequency;
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
                return Z;
        } break;
        case cosine: {
                using constants::pi;

                const scalar_t u__  = x;
                const scalar_t u_   = u__ * frequency;
                const int u     = floor <int> (u_);
                const int u1    = floor <int> (1 + u_);

                const scalar_t v__  = y;
                const scalar_t v_   = v__ * frequency;
                const int  v    = floor <int> (v_);
                const int  v1   = floor <int> (1 + v_);

                /*
                    from Hugo Elias
                    http://freespace.virgin.net/hugo.elias/models/m_perlin.htm

                    --
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
                return Z;
        } break;
        case nearest: {
                const scalar_t         u__  = x;// - ::floor (x); // > [0..1)
                const scalar_t         u_   = u__ * frequency;
                const unsigned int u    = static_cast<unsigned int>(u_);

                const scalar_t         v__  = y;// - ::floor (y); // > [0..1)
                const scalar_t         v_   = v__ * frequency;
                const unsigned int v    = static_cast<unsigned int>(v_);

                const scalar_t Z = LN2D_RNG( u, v, depth);
                return Z;
        } break;
        }

        return 0.0;
}

#undef LN2D_RNG


} } // namespaces

#endif // NOISE2DDEF_HH_INCLUDED_20090813
