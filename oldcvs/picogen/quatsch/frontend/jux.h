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

#ifndef JUX_H__INCLUDED__20090107
#define JUX_H__INCLUDED__20090107

#include <string>
#include <map> 

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/iterator/position_iterator.hpp>
#include <boost/spirit/symbols/symbols.hpp>

#include "../quatsch.h"

namespace quatsch {    
    namespace frontend {
        namespace jux {
            
            template <typename BACKEND> class Compiler 
            : public boost::spirit::grammar <Compiler <BACKEND> > {
                
                public:
                    typedef typename BACKEND::scalar_t scalar_t;
                    typedef typename BACKEND::parameters_t parameters_t;
                    typedef typename BACKEND::Function Function;
                    typedef typename BACKEND::FunctionPtr FunctionPtr;
                    typedef typename quatsch::ICreateConfigurableFunction <Function> ICreateConfigurableFunction;
                    typedef typename ICreateConfigurableFunction::ConfigurableFunctionsMap ConfigurableFunctionsMap;
                
                    typedef ::boost::spirit::position_iterator<char const*> code_iterator_t;
                
                private:
                    const ConfigurableFunctionsMap &configurableFunctions;
                    BACKEND backend;

                    // Not that those can't be implemented, but paranoism first :D
                    Compiler ();
                    Compiler (const Compiler &);
                    Compiler (const code_iterator_t &begin, const code_iterator_t &end, const ::std::string & parameterNames, const ConfigurableFunctionsMap &configurableFunctions);
                    Compiler& operator = (const Compiler&) ;
                    
                    void setParameterNames (const ::std::string & parameterNames);
                
                    void dumpErrorMessages () const;
                
                public:
                    
                    typename BACKEND::Program &program;
                    typename BACKEND::FunctionDefinition &fundef;
                    typename BACKEND::CodeDefinition &codedef;
                    typename BACKEND::SyntaxError &syntaxError;
                
                    // Definition.
                    template <typename ScannerT>
                    struct definition {                    
                        // Those variable names might be confusing. Better look at the actual grammar in juxdef.h.
                        ::boost::spirit::rule <ScannerT> symbol;                    
                        ::boost::spirit::symbols <> operators;
                        ::boost::spirit::rule <ScannerT> function_definition;
                        ::boost::spirit::rule <ScannerT> function_name;
                        ::boost::spirit::rule <ScannerT> configurable_call;
                        ::boost::spirit::rule <ScannerT> operand;
                        ::boost::spirit::rule <ScannerT> call;
                        ::boost::spirit::rule <ScannerT> program_definition;
                        ::boost::spirit::rule <ScannerT> program_definition_no_action;
                        ::boost::spirit::rule <ScannerT> start_rule;
                        
                        definition(Compiler const& self);
                        ::boost::spirit::rule<ScannerT> const& start() const;
                    };
                
                    static typename ::quatsch::Function <scalar_t, parameters_t>::FunctionPtr compile (
                        const ::std::string &parameterNames,
                        const ::std::string &code,
                        const ConfigurableFunctionsMap &addfuns
                    );
            };
        }
    }
}
#endif // JUX_H__INCLUDED__20090107
