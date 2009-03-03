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

#ifndef EST_BACKEND__20090104
#define EST_BACKEND__20090104

#include <iostream>
#include <vector>
#include <sstream>

#include <boost/spirit/iterator/position_iterator.hpp>

#include "../../utility/symboltable.hh"
#include "../../utility/sourcecodeposition.hh"
#include "../../utility/errormessage.hh"

#include "exceptions.hh"
#include "node_types.hh"
#include "functiondescriptor.hh"

#include "actions/program.hh"
#include "actions/syntaxerror.hh"
#include "actions/functiondefinition.hh"
#include "actions/codedefinition.hh"

namespace quatsch {  namespace backend {  namespace est {

    // The 'virtual virtual machine' backend, as in "a virtual machine consisting of virtual function calls" :S
    template <typename SCALAR, typename PARAMETERS /*= ::std::vector <scalar_t>*/> class Backend {
            enum { debug=0 };
        public:
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Basic Types.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            typedef SCALAR     scalar_t;
            typedef PARAMETERS parameters_t;
            
            typedef ::boost :: spirit :: position_iterator<char const*>  code_iterator_t;

            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Function[Ptr] related.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            typedef typename ::quatsch :: Function<scalar_t, parameters_t>                Function;
            typedef typename ::quatsch :: Function<scalar_t, parameters_t> :: FunctionPtr FunctionPtr;
        
            typedef typename ::quatsch :: ICreateConfigurableFunction <Function>                             ICreateConfigurableFunction;
            typedef typename ::quatsch :: ICreateConfigurableFunction <Function> :: ConfigurableFunctionsMap ConfigurableFunctionsMap;
        
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Parser Actions.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
        
        public: 
            // > aliases:
            typedef        parser_actions :: Program           <Backend>  Program;
            typedef        parser_actions :: SyntaxError       <Backend>  SyntaxError;
            typedef        parser_actions :: FunctionDefinition<Backend>  FunctionDefinition;
            typedef        parser_actions :: CodeDefinition    <Backend>  CodeDefinition;            
        
        private: 
            // > mates:
            friend  class  parser_actions :: Program           <Backend>;
            friend  class  parser_actions :: SyntaxError       <Backend>;
            friend  class  parser_actions :: FunctionDefinition<Backend>;
            friend  class  parser_actions :: CodeDefinition    <Backend>;                        
        
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // More friends.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            
            friend  class  FunctionDescriptor<Backend>;

        private:           
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Private fields.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            
            // operators: To which operator the operands at this->operands.back() belong. 
            // Do not confuse with functions, in which user defined functions are stored!    
            ::std::vector < ::std::string> operators; 
            //::std::vector < ::boost::shared_ptr <ICreateConfigurableFunction> > configurableFunctionCreators;
            struct ConfigurableCallDescriptor {
                ::boost::shared_ptr <ICreateConfigurableFunction> creator;
                ::std::map <std::string, std::string> static_arguments;

                ConfigurableCallDescriptor (::boost::shared_ptr <ICreateConfigurableFunction> &creator, ::std::map <std::string, std::string> &static_arguments)
                : creator (creator), static_arguments (static_arguments)
                {}
            };
            ::std::vector <ConfigurableCallDescriptor> configurableCallDescriptors;
            ::std::vector < ::std::vector <FunctionPtr> > operands;

            // In this stack of SymbolTables, each table is distinct, and no table is derived from any other,
            // that is, parameters[x+1] does NOT include the symbols of parameters[x].
            ::std::vector <SymbolTable <unsigned int> > parameters;    
            
            SymbolTable <FunctionDescriptor<Backend> > functions;
            const ConfigurableFunctionsMap &configurableFunctions;    
        
            // To stack up error- and warning-messages.
            ::std::vector <ErrorMessage> errorMessages;
        
            Backend ();

        public:


            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Public Functors.
            ///////////////////////////////////////////////////////////////////////////////////////////////////

            // > members:
            Program             program;
            FunctionDefinition  functionDefinition;
            CodeDefinition      codeDefinition;
            SyntaxError         syntaxError;



            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Public Methods.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            void dumpErrorMessages () const;
            void addParameter (const ::std::string &name);
            FunctionPtr getFunction ();



            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Alpha + Omega.
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            Backend (const ConfigurableFunctionsMap &configurableFunctions);
            virtual ~Backend();
            
    };

} } }

#endif // EST_BACKEND__INCLUDED__20090104
