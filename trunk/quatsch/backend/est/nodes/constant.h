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

#ifndef ESTNODES_CONSTANT_20090124
#define ESTNODES_CONSTANT_20090124
namespace quatsch {  namespace backend {  namespace est {
    // To allow for constant scalar numbers in the code.
    template <typename TYPES> class Constant : public TYPES::Function {
            const typename TYPES::scalar_t value;
            Constant (const typename TYPES::scalar_t value) : value (value) {}
            Constant ();
            Constant (Constant &);
        public:
            static typename TYPES::FunctionPtr create (const typename TYPES::scalar_t value) {
                return typename TYPES::FunctionPtr (new Constant (value));
            }
            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &) const {
                return value;
            }
    };
} } }
#endif // ESTNODES_CONSTANT_20090124
