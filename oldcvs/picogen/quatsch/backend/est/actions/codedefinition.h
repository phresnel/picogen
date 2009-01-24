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

#ifndef ACTCODEDEFINITION__20090123
#define ACTCODEDEFINITION__20090123
namespace quatsch {  namespace backend {  namespace est {  namespace parser_actions {
    template <typename BACKEND> class CodeDefinition {
            enum { debug=0 };
            int nestingDepth;
            CodeDefinition ();
            BACKEND &backend;
            ::std::vector <SymbolTable <unsigned int> > &parameters;
            ::std::vector < ::std::vector <typename BACKEND::FunctionPtr> > &operands;
            ::std::vector < ::std::string> &operators;
            ::std::vector <SourcecodePosition> operatorNamePositions;
        public:
            class Push {
                    Push ();
                    CodeDefinition &parent;
                public:
                    Push (CodeDefinition &parent) : parent (parent) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t&, const code_iterator_t&) const {
                        if (debug) {
                            for (int i=0; i<parent.nestingDepth; ++i) 
                                ::std::cout << "\t";
                            ::std::cout << "push." << ::std::endl;
                        }
                        ++parent.nestingDepth;
                        parent.operands.push_back (::std::vector <typename BACKEND::FunctionPtr> ());
                    }
            };
            
            class OperatorName {
                    OperatorName ();
                    CodeDefinition &parent;
                public:                    
                    OperatorName (CodeDefinition &parent) : parent (parent) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t&from, const code_iterator_t&to) const {
                        if (debug) {
                            for (int i=0; i<parent.nestingDepth; ++i) 
                                ::std::cout << "\t";                            
                        }
                        // TODO: ScopeGuard
                        parent.operators.push_back (::std::string (from, to));
                        parent.operatorNamePositions.push_back (SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file));
                        if (debug) { ::std::cout << "operator-name:\"" << parent.operators.back() << "\"." << ::std::endl; }
                    }
            };
            
            class ConfigurableFunctionCall {
                    ConfigurableFunctionCall ();
                    BACKEND &backend;
                    const typename BACKEND::ConfigurableFunctionsMap &configurableFunctions;
                
                    ::std::map<std::string, std::string> static_arguments; 
                    ::std::vector <typename BACKEND::FunctionPtr> runtime_arguments;
                    ::std::string name;
                
                    // ( [ <function-name> <parameter (<argument>)>* ] <runtime-arguments>*)
                public:
                    class Begin {
                            Begin();
                            ConfigurableFunctionCall &parent;
                        public:
                            Begin (ConfigurableFunctionCall &parent) : parent (parent) {}
                            template <typename code_iterator_t> void operator () (const code_iterator_t&, const code_iterator_t&) const {
                                parent.static_arguments.clear(); 
                                parent.runtime_arguments.clear();
                                parent.name = "";
                            }
                    };
                        
                    class FunctionName {
                            ConfigurableFunctionCall &parent;
                            BACKEND &backend;
                            FunctionName();
                        public:
                            FunctionName (ConfigurableFunctionCall &parent) : parent (parent), backend (parent.backend) {}
                            template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {
                                parent.name = ::std::string (from, to);
                                backend.operators.push_back (""); // Will say parent.pop() that it was a configurable call.
                            }
                    };
                    
                    class End {
                            End();
                            ConfigurableFunctionCall &parent;
                            BACKEND &backend;
                        public:
                            End (ConfigurableFunctionCall &parent) : parent (parent), backend (parent.backend) {}
                            template <typename code_iterator_t> void operator () (const code_iterator_t& /*from*/, const code_iterator_t& /*to*/) const {                                                
                                // If the function is not defined, next line will throw 'quatsch::SymbolTable<>::undefined_symbol_exception'.
                                ::boost::shared_ptr <typename BACKEND::ICreateConfigurableFunction> i (parent.configurableFunctions [parent.name]);

                                backend.configurableFunctionCreators.push_back (i);
                                // Create function from description.
                                //FunctionPtr p (i->create(parent.static_arguments, parent.runtime_arguments));
                            }
                    };
                    
                    friend class Begin        ;  Begin        begin;
                    friend class FunctionName ;  FunctionName functionName;
                    friend class End          ;  End          end;
                    
                    ConfigurableFunctionCall (CodeDefinition &parent) 
                    : backend (parent.backend), configurableFunctions (backend.configurableFunctions)
                    , begin (*this)
                    , functionName (*this)
                    , end (*this)
                    {}
            };
            
            class ScalarOperand {
                    ScalarOperand ();
                    CodeDefinition &parent;
                    BACKEND &backend;
                public:                    
                    ScalarOperand (CodeDefinition &parent) : parent (parent), backend (parent.backend) {}
                    void operator () (typename BACKEND::scalar_t scalar) const {
                        if (debug) {
                            for (int i=0; i<parent.nestingDepth; ++i) 
                                ::std::cout << "\t";
                            ::std::cout << "scalar-operand:\"" << scalar << "\"." << ::std::endl;
                        }
                        backend.operands.back().push_back (node_types<BACKEND>::Constant::create (scalar));
                    }
            };
            
            class SymbolOperand {
                    SymbolOperand ();
                    CodeDefinition &parent;
                    BACKEND &backend;
                    ::std::vector <SymbolTable <unsigned int> > &parameters;
                public:                    
                    SymbolOperand (CodeDefinition &parent) : parent (parent), backend (parent.backend), parameters (parent.parameters) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {
                        const ::std::string sym = ::std::string (from, to);
                        try {
                            parameters.back().probeSymbol (sym);
                            if (debug) {
                                for (int i=0; i<parent.nestingDepth; ++i) 
                                    ::std::cout << "\t";
                                ::std::cout << "symbol-operand:\"" << sym << "\":" << parameters.back().getItem (sym) << ::std::endl;
                            }
                            backend.operands.back().push_back (node_types<BACKEND>::Parameter::create (parameters.back().getItem (sym)));
                        } catch (SymbolTable <unsigned int>::undefined_symbol_exception &e) {
                            backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ::std::string("unknown symbol \"")+e.symbol+"\".", 
                                SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file)));
                            backend.operands.back().push_back (node_types<BACKEND>::Null::create ());
                        }                                        
                    }
            };
            
            class Pop {
                    Pop ();
                    CodeDefinition &parent;
                    BACKEND &backend;
                    SymbolTable <FunctionDescriptor<BACKEND> > &functions;
                    
                public:                    
                    Pop (CodeDefinition &parent) : parent (parent), backend (parent.backend), functions (backend.functions) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& /*from*/, const code_iterator_t& /*to*/) const {
                        using namespace std;

                        --parent.nestingDepth;
                        if (debug) {
                            for (int i=0; i<parent.nestingDepth; ++i) 
                                ::std::cout << "\t";
                            //::std::cout << "pop:\"" << ::std::string (from, to) << "\"." << ::std::endl;                            
                        }
                        
                        const ::std::string operator_ = parent.operators.back();
                        parent.operators.pop_back();
                        ::std::vector <typename BACKEND::FunctionPtr> operands;

                        for (typename ::std::vector <typename BACKEND::FunctionPtr>::const_iterator it=parent.operands.back().begin()
                            ; it!=parent.operands.back().end() ; ++it) {
                            operands.push_back (*it);
                        }
                        
                        // TODO: check param count of builtins
                        // TODO: check param count for zero-arg functions
                        // TODO: correct singular/plural
                        
                        // TODO: ScopeGuard
                        parent.operands.pop_back ();
                        const SourcecodePosition pos = parent.operatorNamePositions.back();
                        parent.operatorNamePositions.pop_back ();

                        if (functions.hasSymbol (operator_)) {
                            FunctionDescriptor<BACKEND>& f = functions [operator_];
                            if (debug) { ::std::cout << "Call to " << (f.isComplete() ? "complete":"incomplete") << " function \"" << f.getName() << "\"." << ::std::endl; }
                            
                            // Record whether the correct number of arguments is passed.
                            if (f.getParameterCount() != operands.size()) {                                                
                                stringstream ss;
                                ss << "passed " << operands.size() << ' ' << (1==operands.size()?"argument":"arguments") << " to function \"" << f.getName () << "\","
                                   << " but exactly " << f.getParameterCount() << ' ' << (1==f.getParameterCount()?"is":"are") << " required."
                                   << flush;
                                backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ss.str(), pos));
                            }
                            
                            // If not enough arguments are passed, we fill them up with Null()'s. This avoids more code mess.
                            for (unsigned int i=operands.size(); i<f.getParameterCount(); ++i) {
                                operands.push_back (node_types<BACKEND>::Null::create());
                            }
                            
                            if (f.isComplete()) {
                                parent.operands.back().push_back (node_types<BACKEND>::Call::create (
                                    f.getFunction(), f.getParameterCount(), operands));
                            } else {
                                typename BACKEND::FunctionPtr p (node_types<BACKEND>::Call::create_incomplete (f.getParameterCount(), operands));
                                f.addIncompleteCall (pos, p);
                                parent.operands.back().push_back (p);
                            }
                        } else if ("" == operator_) {
                            // This is a call to a configurable function.
                            if (debug) {::std::cout << "Creating call to configurable function '" << backend.configurableFunctionCreators.back()->name() << "'" << ::std::endl;}

                            // Record whether the correct number of arguments is passed.
                            if (backend.configurableFunctionCreators.back()->parameterCount() != operands.size()) {                                                
                                stringstream ss;
                                ss << "passed " << operands.size() << " arguments to function \"[" << backend.configurableFunctionCreators.back()->name() << "]\","
                                   << " but exactly " << backend.configurableFunctionCreators.back()->parameterCount() << " are required" 
                                   << flush;
                                backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ss.str(), pos));
                            }
                            
                            // If not enough arguments are passed, we fill them up with Null()'s. This avoids more code mess.
                            // TODO: It could be that it should actually be a value-function, for example the configurable-func. may pre-sample over some parameters.
                            //       * Maybe the configurable-func should just catch null-calls?
                            for (unsigned int i=operands.size(); i<backend.configurableFunctionCreators.back()->parameterCount(); ++i) {                                                
                                operands.push_back (node_types<BACKEND>::Null::create());
                            }
                            
                            static ::std::map<std::string, std::string> static_arguments; // TODO: not here
                            typename BACKEND::FunctionPtr p (backend.configurableFunctionCreators.back()->create(static_arguments, operands));
                            parent.operands.back().push_back (p);
                            
                            backend.configurableFunctionCreators.pop_back();
                        } else {
                            try {
                                parent.operands.back().push_back (node_types<BACKEND>::create (operator_, operands.size(), operands));
                            } catch (const unknown_operator_exception&) {
                                // Implicit forward declaration.
                                
                                // Store name:paramCount.
                                functions.addSymbol (operator_, FunctionDescriptor<BACKEND> (operator_, operands.size())); 
                                
                                // Emit call to incomplete function.
                                FunctionDescriptor<BACKEND> & f = functions [operator_];
                                typename BACKEND::FunctionPtr p (node_types<BACKEND>::Call::create_incomplete (f.getParameterCount(), operands));
                                f.addIncompleteCall (pos, p);
                                parent.operands.back().push_back (p);
                            } catch (const insufficient_number_of_operands_exception &e) {
                                
                                // Add an error message.
                                stringstream ss;
                                ss << "passed " << operands.size()
                                   << (operands.size()==1?" argument":" arguments") <<  " to function \"" << operator_ << "\", "
                                   << "but at least " << e.getMinimumCount() 
                                   << (e.getMinimumCount()==1?" is":" are") << " required." 
                                   << flush;
                                backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ss.str(), pos));
                                
                                // Fill up with nulls so we don't have to stop now.
                                while (operands.size()<e.getMinimumCount()) {
                                    operands.push_back (node_types<BACKEND>::Null::create());
                                }
                                // Do not catch this time, let possible exception propagate. Seems to be a serious problem.
                                parent.operands.back().push_back (node_types<BACKEND>::create (operator_, operands.size(), operands));
                            } catch (const too_many_operands_exception &e) {
                                
                                // Add an error message.
                                stringstream ss;
                                ss << "passed " << operands.size()
                                   << (operands.size()==1?" argument":" arguments") <<  " to function \"" << operator_ << "\", "
                                   << "but at most " << e.getMaximumCount() 
                                   << (e.getMaximumCount()==1?" is":" are") << " allowed." 
                                   << flush;
                                backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ss.str(), pos));
                                
                                // Kill operands behind the allowed limit.
                                while (operands.size() > e.getMaximumCount()) { 
                                    operands.pop_back ();
                                }
                                // Do not catch this time, let possible exception propagate. Seems to be a serious problem.
                                parent.operands.back().push_back (node_types<BACKEND>::create (operator_, operands.size(), operands));
                            } catch (const invalid_number_of_operands_exception &e) {
                                
                                // Add an error message.
                                stringstream ss;
                                ss << "passed " << operands.size()
                                   << (operands.size()==1?" argument":" arguments") <<  " to function \"" << operator_ << "\", "
                                   << "but exactly " << e.getRequiredCount() 
                                   << (e.getRequiredCount()==1?" is":" are") << " required." 
                                   << flush;
                                backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ss.str(), pos));
                                
                                // Kill operands behind the allowed limit.
                                if (operands.size() < e.getRequiredCount()) {
                                    while (operands.size() < e.getRequiredCount()) { 
                                        operands.push_back (node_types<BACKEND>::Null::create());
                                    }
                                } else {
                                    while (operands.size() > e.getRequiredCount()) { 
                                        operands.pop_back ();
                                    }
                                }
                                // Do not catch this time, let possible exception propagate. Seems to be a serious problem.
                                parent.operands.back().push_back (node_types<BACKEND>::create (operator_, operands.size(), operands));
                            }
                        }
                    }
            };
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Mates and members.
            friend class Push                     ;   Push                     push;
            friend class OperatorName             ;   OperatorName             operatorName;
            friend class ConfigurableFunctionCall ;   ConfigurableFunctionCall configurableFunctionCall;
            friend class ScalarOperand            ;   ScalarOperand            scalarOperand;
            friend class SymbolOperand            ;   SymbolOperand            symbolOperand;
            friend class Pop                      ;   Pop                      pop;
            
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Constructor.
            CodeDefinition (BACKEND &backend)
            : nestingDepth (0)
            , backend (backend)
            , parameters (backend.parameters)
            , operands (backend.operands)
            , operators (backend.operators)
            
            , push (*this)
            , operatorName (*this)
            , configurableFunctionCall (*this)
            , scalarOperand (*this)
            , symbolOperand (*this)
            , pop (*this)                            
            {}
    };
} } } }
#endif // ACTCODEDEFINITION__20090123
