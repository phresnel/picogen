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

#ifndef LIBNOISE_SUPPORT_HH_INCLUDED_20100427
#define LIBNOISE_SUPPORT_HH_INCLUDED_20100427

#include <boost/shared_array.hpp>
//#include <picogen/picogen.h>
#include "../../kallisto/common.hh"

#include "../quatsch.hh"
#include "../frontend/jux.hh"
#include "../backend/est/backend.hh"

#include <libnoise/noise.h>

#include <map>

namespace quatsch {  namespace configurable_functions {

template <typename FUNCTION, typename COMPILER>
class LibnoisePerlin : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Perlin perlin;

public:
        LibnoisePerlin (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; perlin.SetFrequency(tmp);
                        } else if (name == string("lacunarity")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; perlin.SetLacunarity(tmp);
                        } else if (name == string("octave-count")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; perlin.SetOctaveCount(tmp);
                        } else if (name == string("persistence")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; perlin.SetPersistence(tmp);
                        } else if (name == string("seed")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; perlin.SetSeed(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                perlin.SetNoiseQuality(noise::QUALITY_BEST);

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LayeredNoise': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoisePerlin() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoisePerlin");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return perlin.GetValue(u, v, 0.5);
        }

};






template <typename FUNCTION, typename COMPILER>
class LibnoiseBillow : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Billow mod;

public:
        LibnoiseBillow (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetFrequency(tmp);
                        } else if (name == string("lacunarity")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetLacunarity(tmp);
                        } else if (name == string("octave-count")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetOctaveCount(tmp);
                        } else if (name == string("persistence")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetPersistence(tmp);
                        } else if (name == string("seed")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; mod.SetSeed(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                mod.SetNoiseQuality(noise::QUALITY_BEST);

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseBillow': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseBillow() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseBillow");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return mod.GetValue(u, v, 0.5);
        }
};






template <typename FUNCTION, typename COMPILER>
class LibnoiseCylinders : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Cylinders mod;

public:
        LibnoiseCylinders (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetFrequency(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseCylinders': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseCylinders() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseCylinders");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return mod.GetValue(u, v, 0.5);
        }
};






template <typename FUNCTION, typename COMPILER>
class LibnoiseRidgedMulti : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::RidgedMulti mod;

public:
        LibnoiseRidgedMulti (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetFrequency(tmp);
                        } else if (name == string("lacunarity")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetLacunarity(tmp);
                        } else if (name == string("octave-count")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetOctaveCount(tmp);
                        } else if (name == string("seed")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; mod.SetSeed(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                mod.SetNoiseQuality(noise::QUALITY_BEST);

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseRidgedMulti': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseRidgedMulti() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseRidgedMulti");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return mod.GetValue(u, v, 0.5);
        }
};





template <typename FUNCTION, typename COMPILER>
class LibnoiseSpheres : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Spheres mod;

public:
        LibnoiseSpheres (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetFrequency(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }


                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseSpheres': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseSpheres() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseSpheres");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return mod.GetValue(u, v, 0.5);
        }
};







template <typename FUNCTION, typename COMPILER>
class LibnoiseVoronoi : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Voronoi voronoi;

public:
        LibnoiseVoronoi (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; voronoi.SetFrequency(tmp);
                        } else if (name == string("displacement")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; voronoi.SetDisplacement(tmp);
                        } else if (name == string("enable-distance")) {
                                istringstream hmmm (static_parameters[name]);
                                bool tmp; hmmm >> tmp; voronoi.EnableDistance(tmp);
                        } else if (name == string("seed")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; voronoi.SetSeed(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseVoroni': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseVoronoi() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseVoronoi");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return voronoi.GetValue(u, v, 0.5);
        }

};






template <typename FUNCTION, typename COMPILER>
class LibnoiseTurbulence : public FUNCTION {
private:
        typedef FUNCTION function_t;
        typedef typename function_t::FunctionPtr  FunctionPtr;
        typedef typename function_t::scalar_t     scalar_t;
        typedef typename function_t::parameters_t parameters_t;

        typedef COMPILER compiler_t;

        FunctionPtr ufun;
        FunctionPtr vfun;

        noise::module::Turbulence mod;

public:
        LibnoiseTurbulence (
                ::std::map<std::string,std::string>&static_parameters,
                ::std::vector <FunctionPtr> &runtime_parameters
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


                using namespace std;
                typedef map<string,string> Map;

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
                        if (name == string("frequency")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetFrequency(tmp);
                        } else if (name == string("power")) {
                                istringstream hmmm (static_parameters[name]);
                                double tmp; hmmm >> tmp; mod.SetPower(tmp);
                        } else if (name == string("roughness")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; mod.SetRoughness(tmp);
                        } else if (name == string("seed")) {
                                istringstream hmmm (static_parameters[name]);
                                int tmp; hmmm >> tmp; mod.SetSeed(tmp);
                        } else {
                                nonExistantParameterNames += (nonExistantParameterNames!=""?", ":"") + string("'") + it->first + string("'");
                        }
                }

                // Any parameters set that we don't know?
                if (nonExistantParameterNames != "") {
                        throw general_exception ("the following parameters do not exist for 'LibnoiseTurbulence': "+nonExistantParameterNames);
                }
        }

        virtual ~LibnoiseTurbulence() {}

        static ::std::string const & name () {
                static const ::std::string name ("LibnoiseTurbulence");
                return name;
        }

        static unsigned int parameterCount () {
                return 2;
        }

        virtual scalar_t operator () (const parameters_t &parameters) const {
                const scalar_t u = (*ufun) (parameters);
                const scalar_t v = (*vfun) (parameters);
                return mod.GetValue(u, v, 0.5);
        }

};
} }


#endif // LIBNOISE_SUPPORT_HH_INCLUDED_20100427
