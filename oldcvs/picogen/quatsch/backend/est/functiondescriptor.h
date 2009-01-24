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

#ifndef FUNCTIONDESCRIPTOR__20090123
#define FUNCTIONDESCRIPTOR__20090123

namespace quatsch { namespace backend { namespace est {
    
    template <typename BACKEND>
    class FunctionDescriptor {
            enum { debug = 0 };
            const ::std::string name;
            typename BACKEND::FunctionPtr function; // Needs to be mutable to allow for *easy* forward declaration.
            const unsigned int parameterCount;
            FunctionDescriptor ();
            struct IncompleteCall {
                SourcecodePosition position;
                typename BACKEND::FunctionPtr call;
                IncompleteCall (SourcecodePosition const& position_, typename BACKEND::FunctionPtr& call_) : position (position_), call (call_) {}
            };
            ::std::vector <IncompleteCall> incompleteCalls;
        public:
            FunctionDescriptor (const FunctionDescriptor& desc) 
            : name (desc.name), function (desc.function), parameterCount (desc.parameterCount) {}
            
            FunctionDescriptor (const ::std::string &name, const typename BACKEND::FunctionPtr function, const unsigned int parameterCount)
            : name (name), function (function), parameterCount (parameterCount) {}
            
            FunctionDescriptor (const ::std::string &name, const unsigned int parameterCount)
            : name (name), parameterCount (parameterCount) {}

            void setFunction (typename BACKEND::FunctionPtr function) {
                if (this->isComplete ()) {
                    throw operation_not_allowed_exception ();
                }
                this->function = function;
                for (typename ::std::vector <IncompleteCall>::iterator it=incompleteCalls.begin(); it!=incompleteCalls.end(); ++it) {                        
                    if (debug) { ::std::cout << it->call.get() << ".setFunction(" << function.get() << ") -> " << ::std::flush; }
                    ::boost::static_pointer_cast <
                        typename node_types<BACKEND>::Call, 
                        typename BACKEND::Function
                    > (it->call)->setFunction (function);
                }
                if (debug) { ::std::cout << "incompleteCalls.clear()" << ::std::endl; }
                incompleteCalls.clear();
            }
            
            void probe() const {
                if (!isComplete()) {
                    throw unresolved_forward_declaration_exception();
                }
            }
            
            bool isComplete () const {
                return 0 != function.get();
            }
            
            // To support forward declarations.
            void addIncompleteCall (SourcecodePosition const& position, typename BACKEND::FunctionPtr call) {
                // TODO: throw if we are actually complete
                if (debug) { ::std::cout << "addIncompleteCall (" << call.get() << ")" << ::std::endl; } 
                incompleteCalls.push_back (IncompleteCall (position, call));
            }
            
            void pushIncompleteCallPositions (::std::vector <SourcecodePosition> &out) const {
                for (typename ::std::vector <IncompleteCall>::const_iterator it=incompleteCalls.begin(); it!=incompleteCalls.end(); ++it) {
                    out.push_back (it->position);
                }
            }
            
            const ::std::string &getName () const { return name; }
            const typename BACKEND::FunctionPtr getFunction () const { return function; }
            unsigned int getParameterCount () const { return parameterCount; }
    };
} } }

#endif // FUNCTIONDESCRIPTOR__20090123
