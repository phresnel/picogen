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

#include <map>
#include <picogen/picogen.h>

namespace picogen {
    namespace misc {
        namespace functional {

            class Layers : public BasicFunction {

                private:
                    typedef ::picogen::real real;

                    BasicFunction *ufun, *vfun;
                    Function_R1_R1 *noiseEvalFun;
                    Function_R1_R1 *persistenceFun;
                    Function_R2_R1 *baseFun;
                    real_t frequency;
                    unsigned int depth;

                    real_t operator () (real_t x, real_t y, real_t domainScale, real_t rangeScale, unsigned int depth ) const;

                public:
                    Layers (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun);
                    virtual ~Layers();
                    virtual real_t operator () (const real_t * const parameters) const;

            };
        } // namespace functional {
    } // namespace misc {
} // namespace picogen
