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

#ifndef ESTNODES_PARAMETER_20090124
#define ESTNODES_PARAMETER_20090124
namespace quatsch {  namespace backend {  namespace est {
    // To allow for accessing the parameters thrown at the vm.
    template <typename TYPES> class Parameter : public TYPES::Function {
            const unsigned int address;
            Parameter (const unsigned int address) : address (address) {}
            Parameter ();
            Parameter (Parameter &);                    
        public:
            static typename TYPES::FunctionPtr create (const unsigned int address) {
                return typename TYPES::FunctionPtr (new Parameter (address));
            }
            typename TYPES::scalar_t operator () (
                const typename TYPES::parameters_t &parameters) const {
                return parameters [address];
            }
    };
} } }
#endif // ESTNODES_PARAMETER_20090124
