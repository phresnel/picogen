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

#ifndef JUXDEF_H__INCLUDED__20090107
#define JUXDEF_H__INCLUDED__20090107

#include "jux.h"

namespace quatsch {    
    namespace frontend {
        namespace jux {
            
            template <class BACKEND> 
                template <typename ScannerT> 
                    Compiler <BACKEND> :: definition <ScannerT> :: definition (Compiler <BACKEND> const& self)
            {
                using namespace ::boost::spirit;
                using ::std::string;
                
                typename BACKEND::FunctionDefinition &fundef = self.fundef;
                typename BACKEND::CodeDefinition &codedef = self.codedef;
                typename BACKEND::Program &program = self.program;
                typename BACKEND::SyntaxError &syntaxError = self.syntaxError;
                
                symbol
                    =   lexeme_d
                            [
                                ( range_p ('a','z')
                                | range_p ('A','Z')
                                )
                            >>
                            *   ( range_p ('a','z')
                                | range_p ('A','Z')
                                | range_p ('0','9')
                                | "-" 
                                | "_"
                                )
                            ]
                    ;
                
                operators
                    =   "+"  , "-"  , "*"  ,  "/" , "^"  ,
                        "<"  , ">"  , "<=" , ">=" , "<>" ,
                        "!=" , "="  ,
                        "[]" , "[[" , "]]" , "]["
                    ;

                function_name
                        = operators 
                        | symbol
                    ;

                function_definition
                    = 
                        (       ch_p('(')
                            >>  eps_p [syntaxError.beginFrame]
                            >>  str_p("defun")
                            >>  ( function_name [fundef.beginDefinition] [fundef.setName] 
                                    >>  '('
                                    >>   *(symbol [fundef.addParameter])
                                    >>  ')'
                                ) [fundef.finishPrototype]
                            >>  operand [fundef.setCode]
                            >>  ( ')'
                                | eps_p [syntaxError.missingClosing("(",")")]
                                )
                        ) [fundef.endDefinition] >> eps_p [syntaxError.endFrame]
                    ;
                
                configurable_call
                    =   (ch_p ('[')
                            >> eps_p         [codedef.configurableFunctionCall. begin       ] 
                            >> function_name [codedef.configurableFunctionCall. functionName] 
                            >> ']'
                            >> eps_p         [codedef.configurableFunctionCall. end         ]
                        )
                    ;
                
                operand = call
                        | real_p [codedef.scalarOperand]
                        | symbol [codedef.symbolOperand]
                        ;// | eps_p [syntaxError.unexpected];

                call
                    = 
                        (ch_p('(') >> eps_p [codedef.push] >> eps_p [syntaxError.beginFrame]
                            // either an ordinary function, or a "configurable" one
                            >>
                                ( function_name [codedef.operatorName]
                                | configurable_call
                                )
                            >>
                            *operand
                            >>  
                                ( ')'
                                | eps_p [syntaxError.missingClosing("(",")")]
                                )
                        )
                        >> eps_p [codedef.pop] >> eps_p [syntaxError.endFrame]
                    ;

                program_definition
                    =   eps_p [program.begin]
                        >>
                        (*function_definition >> operand)
                        /*(   (*function_definition >> operand)
                        >>  eps_p [syntaxError.expected["end of file"]]
                        )*/
                        >> 
                        eps_p [program.end]
                    ;

                start_rule = program_definition;// >> (anychar_p >> eps_p [syntaxError.unexpected]); // expected end of file
            }



            template <typename BACKEND> 
                template <typename ScannerT>::boost::spirit::rule<ScannerT> 
                const& Compiler <BACKEND>::definition <ScannerT> :: start()
            const {
                return start_rule;
            }
                           
            
            
            template <typename BACKEND> Compiler<BACKEND> :: Compiler 
                ( const code_iterator_t &begin
                , const code_iterator_t &end 
                , const ::std::string & parameterNames
                , const ConfigurableFunctionsMap &configurableFunctions
                )
                : configurableFunctions (configurableFunctions)
                , backend(configurableFunctions)
                , program (backend.program)
                , fundef (backend.functionDefinition) 
                , codedef (backend.codeDefinition)
                , syntaxError (backend.syntaxError)
            {
                setParameterNames (parameterNames);
            }
                    
            
            
            template <typename BACKEND>
                void Compiler <BACKEND> :: setParameterNames (const ::std::string & parameterNames)
            {
                using ::std::string;
                ::std::string tmp;
                for (::std::string::const_iterator it=parameterNames.begin(); it != parameterNames.end(); ++it) {
                    if (*it == ';' && tmp.size()>0) {
                        backend.addParameter (tmp);
                        tmp = "";
                    } else {
                        tmp += *it;
                    }
                }
                if (tmp.size()>0) {
                    backend.addParameter (tmp);
                } 
            }



            template <typename BACKEND>
            void Compiler <BACKEND> :: dumpErrorMessages () const {
                backend.dumpErrorMessages ();
            }



            template <typename BACKEND>
                typename ::quatsch::Function <
                    typename Compiler <BACKEND> :: scalar_t, 
                    typename Compiler <BACKEND> :: parameters_t
                > :: FunctionPtr
                Compiler <BACKEND> :: compile (
                    const ::std::string &parameterNames, 
                    const ::std::string &code,
                    const ConfigurableFunctionsMap &addfuns
                )
            {
                using namespace ::boost::spirit;
                //::std::cout << "Compiling:\"" << code << "\"" << ::std::endl;
                
                code_iterator_t begin (code.c_str(), code.c_str() + strlen (code.c_str()), "");
                code_iterator_t end;
                begin.set_tabchars (1);
                /*char const* begin = code.c_str();
                char const* end = code.c_str() + strlen (code.c_str());*/
                
                Compiler compiler (begin, end, parameterNames, addfuns);

                parse_info<code_iterator_t> info = parse (begin, end, compiler, space_p | comment_p("/*", "*/") | comment_p("//"));
                if (!info.full) {
                    //::std::cerr << "The program \"" << code << "\" is not valid." << ::std::endl;
                    compiler.syntaxError.invalidProgram (begin, info.stop);
                    //throw;
                }
                compiler.dumpErrorMessages ();
                //throw;
                return compiler.backend.getFunction ();
            }
        }
    }
}

#endif // JUXDEF_H__INCLUDED__20090107
