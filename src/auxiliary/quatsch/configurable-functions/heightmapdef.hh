//
//    quatsch - a tiny compiler framework for strict functional languages
//
//    Copyright (C) 2010  Sebastian Mach
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

#ifndef HEIGHTMAPDEF_HH_INCLUDED_20100424
#define HEIGHTMAPDEF_HH_INCLUDED_20100424

#include <string>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "heightmap.hh"

namespace quatsch {  namespace configurable_functions {

template <typename FUNCTION, typename COMPILER>
Heightmap <FUNCTION, COMPILER> :: Heightmap (
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
        /*::Heightmap (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun)
        : ufun(ufun), vfun(vfun), noiseEvalFun(0), persistenceFun(0)
        //, enableBilinearNoiseMapFilter(true)
        , filter (cosine)
        , noiseDepth(6)
        , frequency(2)*/

        using namespace std;
        typedef map<string,string> Map;


        std::string filename;

        //====---- - - -  -   -    -      -
        // Scan Parameters.
        //====---- - - -  -   -    -      -
        string nonExistantParameterNames (""); // To collect together parameter names that don't exist, for dumping errors in the end.

        width = height = depth = 1000;
        iwidth = 1/width;
        iheight = 1/height;
        idepth = 1/depth;
        filter = cubic;
        wrapMode = zero;

        picogen::redshift::aux::PixelToHeightMode p2hmode = picogen::redshift::aux::Average;

        for (Map::const_iterator it=static_parameters.begin();
             it!=static_parameters.end();
             ++it
        ) {
                const string name = it->first;
                /// \todo Add some shorter Mnenomics.
                if (name == string("pixel-to-height")) {
                        istringstream hmmm (static_parameters[name]);
                        string p2h;
                        hmmm >> p2h;
                        if (p2h == "average") {
                                p2hmode = picogen::redshift::aux::Average;
                        } else if (p2h == "luminance") {
                                p2hmode = picogen::redshift::aux::Luminance;
                        } else {
                                throw general_exception ("Heightmap: unknown pixel-to-height type for 'pixel-to-height': '" + p2h + "' (only 'average','luminance' are supported)");
                        }
                } else if (name == string("width")) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> width;
                        iwidth = 1 / width;
                } else if (name == string("height") ) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> height;
                        iheight = 1 / height;
                } else if (name == string("depth")) {
                        istringstream hmmm (static_parameters[name]);
                        hmmm >> depth;
                        idepth = 1 / depth;
                } else if (name == string("filename")) {
                        istringstream hmmm (static_parameters[name]);
                        filename = hmmm.str();
                } else if (name == string("filter")) {
                        istringstream hmmm (static_parameters[name]);
                        string filterType;
                        hmmm >> filterType;
                        if (filterType == "bilinear") {
                                filter = bilinear;
                        } else if (filterType == "nearest") {
                                filter = nearest;
                        } else if (filterType == "cubic") {
                                filter = cubic;
                        } else {
                                throw general_exception ("Heightmap: unknown filter type for 'filter': '" + filterType + "' (only 'bilinear','nearest','cubic' are supported)");
                        }
                } else if (name == string("wrap")) {
                        istringstream hmmm (static_parameters[name]);
                        string wrapType;
                        hmmm >> wrapType;
                        if (wrapType == "zero") {
                                wrapMode = zero;
                        } else if (wrapType == "wrap") {
                                wrapMode = wrap;
                        } else if (wrapType == "clamp") {
                                wrapMode = clamp;
                        } else if (wrapType == "mirror") {
                                wrapMode = mirror;
                        } else {
                                throw general_exception ("Heightmap: unknown wrap type for 'filter': '" +  wrapType + "' (only 'zero','wrap','clamp','mirror' are supported)");
                        }
                } else {
                        nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                }
        }

        // Any parameters set that we don't know?
        if (nonExistantParameterNames != "") {
                throw general_exception ("the following parameters do not exist for 'Heightmap': "+nonExistantParameterNames);
        }


        // TODO: Check filename ...
        {
                /*
                std::ifstream ifs (filename.c_str());
                if (!ifs) ... // <-- fails for directories
                */
                if (!boost::filesystem::exists(filename) || !boost::filesystem::is_regular_file(filename))
                        throw general_exception ("the file \"" + filename + "\" could not be opened for reading.");
        }

        if (!heightmap.load (filename, p2hmode))
                throw general_exception ("error while loading \"" + filename + "\" (possibly: out of memory, unsupported image format)");

        switch (wrapMode) {
        case zero: heightmap.setWrapMode(RedshiftHeightmap::Zero);
                   break;
        case wrap: heightmap.setWrapMode(RedshiftHeightmap::Wrap);
                   break;
        case clamp: heightmap.setWrapMode(RedshiftHeightmap::Clamp);
                   break;
        case mirror: heightmap.setWrapMode(RedshiftHeightmap::Mirror);
                   break;
        }
}


template <typename FUNCTION, typename COMPILER>
Heightmap <FUNCTION, COMPILER> :: ~Heightmap() {
}




template <typename FUNCTION, typename COMPILER>
typename Heightmap <FUNCTION, COMPILER>::scalar_t
Heightmap <FUNCTION, COMPILER>::operator () (
        const typename Heightmap <FUNCTION, COMPILER>::parameters_t& parameters
) const {
        const scalar_t u = ((*ufun) (parameters) * iwidth) + scalar_t(.5);
        const scalar_t v = ((*vfun) (parameters) * idepth) + scalar_t(.5);

        switch (filter) {
        case nearest: return height * heightmap.nearest(u, v);
        case bilinear: return height * heightmap.lerp(u, v);
        case cubic: return height * heightmap.cubic(u, v);
        }
        return 0.0;
}

} }

#endif // HEIGHTMAPDEF_HH_INCLUDED_20090813
