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

#ifndef ESTNODES_ROOTFUNCTION_20090124
#define ESTNODES_ROOTFUNCTION_20090124
namespace quatsch {  namespace backend {  namespace est {
    // Proxy to hold additional information, like valid functions (as Call does only store weak_ptr's to allow for direct/indirect self recursion).
    template <typename TYPES> 
    class RootFunction : public TYPES::Function {
            const typename TYPES::FunctionPtr start;
            // The following is the reason for the class RootFunction, because the Call-Node only holds 
            //  weak_ptr's, we have to remember the referencees somewhere.
            const ::boost::shared_array <typename TYPES::FunctionPtr> functions; 
            RootFunction (const typename TYPES::FunctionPtr &start, const ::boost::shared_array <typename TYPES::FunctionPtr> &functions) 
            : start (start), functions (functions) {}
            RootFunction ();
            RootFunction (const RootFunction &);
        public:            
            static typename TYPES::FunctionPtr create (typename TYPES::FunctionPtr const &start, 
                const ::boost::shared_array <typename TYPES::FunctionPtr> &functions
            ) {
                return typename TYPES::FunctionPtr (new RootFunction (start, functions));
            }
            virtual typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &p) const {
                return (*start) (p);
            }
    };
} } }
#endif // ESTNODES_ROOTFUNCTION_20090124
