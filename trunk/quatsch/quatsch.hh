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



#ifndef SCRIPTING_H__INCLUDED__20090104
#define SCRIPTING_H__INCLUDED__20090104

#include <iostream>
#include <map>
#include <string>

#include "utility/symboltable.hh"

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

// Mother of all functions.
namespace quatsch {

        
        namespace util {

                template <typename scalar_t>
                inline bool scalar2bool (const scalar_t &r) {
                        return (r>0.5) ? true : false;
                }

                template <typename scalar_t>
                inline scalar_t bool2scalar (const bool &b) {
                        return b ? 1.0 : 0.0;
                }

                template<typename scalar_t>
                inline scalar_t scalar2bool2scalar (const scalar_t&r) {
                        return bool2scalar (scalar2bool (r));
                }

                template <typename scalar_t> inline 
                scalar_t floor (const scalar_t &v) {
                        // TODO: replace with static assert
                        assert (static_cast<int>(1.75) == 1);
                        assert (static_cast<int>(1.5) == 1);
                        assert (static_cast<int>(1.25) == 1);
                        assert (static_cast<int>(-0.75) == 0);
                        assert (static_cast<int>(-0.5) == 0);
                        assert (static_cast<int>(-0.25) == 0);
                        return static_cast <scalar_t> (
                                static_cast <int> (v<0 ? v-1 : v)
                        );
                }
        }
        
        
        
        namespace constants {
                const static double pi = 3.141592654;
        }
        
        
    
        
        
        
        template <
                typename T,
                typename PARAMETERS,
                bool debug=false
        > class Function;



        
        
        
        template <
                typename T, typename PARAMETERS
        > class Function <
                T, PARAMETERS, true
        > {
        public:
                Function () {
                        ::std::cout << "Constructed function " 
                                    << this << ::std::endl;
                }

                virtual ~Function () {
                        ::std::cout << "Destructed function " 
                                    << this << ::std::endl;
                }
            
                virtual T operator () (const PARAMETERS &) const = 0;
            
                typedef ::boost::shared_ptr <Function> FunctionPtr;
                typedef T scalar_t;
                typedef PARAMETERS parameters_t;
        };    
    
    
    
        
        
        
        template <
                typename T,
                typename PARAMETERS
        > class Function <
                T,
                PARAMETERS,
                false
        > {
        public:
                virtual ~Function () {}            
                virtual T operator () (const PARAMETERS &) const = 0;
                typedef ::boost::shared_ptr <Function> FunctionPtr;
                typedef T scalar_t;
                typedef PARAMETERS parameters_t;
        };


    
        
        
        
        template <typename FUNCTION>
        struct ICreateConfigurableFunction {        
        
                virtual typename FUNCTION::FunctionPtr create (
                  ::std::map<std::string,std::string> &static_parms,
                  ::std::vector <typename FUNCTION::FunctionPtr> &runtime_parms
                  // ?? This parameter enables us to avoid a costly Call,
                  // thus making the AST tighter.
                ) const = 0;
        
                virtual ~ICreateConfigurableFunction() {}
        
                virtual ::std::string const & name() = 0;
                virtual unsigned int parameterCount() = 0;
        
                typedef ::boost::shared_ptr <
                        ICreateConfigurableFunction 
                > ConfigurableFunctionDescriptionPtr;
        
                typedef SymbolTable <
                        ConfigurableFunctionDescriptionPtr
                > ConfigurableFunctionsMap;
        };

        
        
        
        
        
        template <typename T, typename FUNCTION>
        class CreateConfigurableFunction
        : public ICreateConfigurableFunction <FUNCTION> {
        
        public:
                typedef T type_t;
                typedef FUNCTION function_t;
            
                
                virtual ::std::string const & name() {
                        return T::name();
                }
                
                
                virtual unsigned int parameterCount() {
                        return T::parameterCount();
                }
                
                
                virtual ~CreateConfigurableFunction() {}
                
                
                virtual typename FUNCTION::FunctionPtr create (
                  ::std::map<std::string,std::string> &static_parms,
                  ::std::vector <typename FUNCTION::FunctionPtr> &runtime_parms
                ) const {
                        return typename FUNCTION::FunctionPtr (
                                new T (static_parms, runtime_parms)
                        );
                }
        };
        
        
        
        
        
        
        class general_exception : public std::exception {
                const ::std::string code;
                const ::std::string message;
        public:
                
                explicit general_exception (const ::std::string &message)
                : code ("<code unavailable>"), message (message)
                { }
                
            
                explicit general_exception (
                        const ::std::string &code,
                        const ::std::string &message
                )
                : code (code), message (message) { }
                
                
                virtual ~general_exception () throw () {}
            
                
                const ::std::string &getMessage() const {
                        return message;
                }
            
                
                const ::std::string &getCode() const {
                        return code;
                }
            
                
                // std::exception
                virtual const char* what() const throw() {
                        return message.c_str();
                }
        };
}


#endif // SCRIPTING_H__INCLUDED__20090104
