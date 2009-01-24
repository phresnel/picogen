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

#ifndef ACTFUNCTIONDEFINITION__20090123
#define ACTFUNCTIONDEFINITION__20090123
namespace quatsch {  namespace backend {  namespace est {  namespace parser_actions {
    template <typename BACKEND> class FunctionDefinition {
            enum { debug=0 };
            BACKEND &backend;
            SymbolTable <FunctionDescriptor<BACKEND> > &functions;
            ::std::string name;
            ::std::vector <SymbolTable <unsigned int> > &parameters;
            ::std::string code;
            void clear () {
                name = code = "";
            }
            
            FunctionDefinition ();
        
        public:
            class BeginDefinition {                    
                    BeginDefinition ();
                    FunctionDefinition &parent;
                    ::std::vector <SymbolTable <unsigned int> > &parameters;
                public:                    
                    BeginDefinition (FunctionDefinition &parent) : parent (parent), parameters (parent.parameters) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& /*from*/, const code_iterator_t& /*to*/) const {
                        parent.clear();
                        parameters.push_back (SymbolTable <unsigned int> ());
                        if (debug) { ::std::cout << "Found function definition." << ::std::endl; }
                    }
            };
            
            class SetName {
                    SetName ();
                    FunctionDefinition &parent;
                    BACKEND &backend;
                public:
                    SetName (FunctionDefinition &parent) : parent (parent), backend (parent.backend) {}
                
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {
                        parent.name = ::std::string (from, to);
                        if(node_types<BACKEND>::is_builtin (parent.name)) {
                            backend.errorMessages.push_back (ErrorMessage (ErrorMessage::warning, ::std::string ("definition of function \"" + parent.name + "\" shadows a builtin function."), 
                                SourcecodePosition (to.get_position().line, to.get_position().column, to.get_position().file)));
                        }
                        if (debug) { ::std::cout << "\tfunction-name:\"" << parent.name << "\"." << ::std::endl; }
                    }
            };
            
            class AddParameter {
                    AddParameter ();
                    FunctionDefinition &parent;
                    ::std::vector <SymbolTable <unsigned int> > &parameters;
                public:
                    AddParameter (FunctionDefinition &parent) : parent (parent), parameters (parent.backend.parameters) {}
                
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {                            
                        const unsigned int size = parameters.back().size();
                        parameters.back().addSymbol (::std::string (from, to), size);
                        if (debug) { ::std::cout << "\tnew-parameter:\"" << ::std::string (from, to) << "\"." << ::std::endl; }
                    }
            };
            
            class FinishPrototype {
                    FinishPrototype ();
                    FunctionDefinition &parent;
                    ::std::vector <SymbolTable <unsigned int> > &parameters;
                
                    BACKEND &backend;
                    SymbolTable <FunctionDescriptor<BACKEND> > &functions;                                    
                public:
                    FinishPrototype (FunctionDefinition &parent)
                    : parent (parent), parameters (parent.parameters), backend (parent.backend), functions (backend.functions) {}
                
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {                            
                        if (debug) { ::std::cout << "\tprototype:\"" << ::std::string (from, to) << "\"." << ::std::endl; }
                        if (!functions.hasSymbol (parent.name)) {
                            functions.addSymbol (parent.name, FunctionDescriptor<BACKEND>  (parent.name, parameters.back().size()));
                        } else {
                            // Function has been forward declared somehow, so check if the signature is allright.
                            FunctionDescriptor<BACKEND> & f = functions [parent.name];
                            if (f.getParameterCount() != parameters.back().size()) {
                                throw definition_does_not_match_forward_declaration_exception();
                            }
                        }
                    }
            };
            
            class SetCode {
                    SetCode ();
                    FunctionDefinition &parent;
                public:
                    SetCode (FunctionDefinition &parent) : parent (parent) {}
                
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {                            
                        parent.code = ::std::string (from, to);
                        if (debug) { ::std::cout << "\tcode:\"" << parent.code << "\"." << ::std::endl; }
                    }
            };
            
            class EndDefinition {                    
                    EndDefinition ();
                    FunctionDefinition &parent;
                    BACKEND &backend;
                    SymbolTable <FunctionDescriptor<BACKEND> > &functions;
                    ::std::vector <SymbolTable <unsigned int> > &parameters;
                public:                    
                    EndDefinition (FunctionDefinition &parent)
                    : parent (parent), backend (parent.backend), functions (backend.functions), parameters (parent.parameters)  {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t&to) const {
                        
                        typename BACKEND::FunctionPtr fun (backend.operands.back().back());
                        // TODO: below would prevent the weak_ptr's in Call become bogus. Got to write some Call-Center. D'Oh.
                        //typename Backend::FunctionPtr * fun (new typename Backend::FunctionPtr (backend.operands.back().back()));
                        backend.operands.back().pop_back();                            
                        
                        functions [parent.name].setFunction (fun);
                        
                        // Clean up.
                        parent.clear();
                        parameters.pop_back();
                        if (debug) { ::std::cout << "finished compiling \"" << ::std::string (from, to) << "\"." << ::std::endl; }
                    }
            };
            
            friend class BeginDefinition ;   BeginDefinition beginDefinition;
            friend class SetName         ;   SetName         setName;
            friend class AddParameter    ;   AddParameter    addParameter;
            friend class FinishPrototype ;   FinishPrototype finishPrototype;
            friend class SetCode         ;   SetCode         setCode;
            friend class EndDefinition   ;   EndDefinition   endDefinition;
            
            FunctionDefinition (BACKEND &backend) 
            : backend (backend)
            , functions (backend.functions)
            , parameters (backend.parameters)
            , beginDefinition (*this)
            , setName (*this)
            , addParameter (*this)
            , finishPrototype (*this)
            , setCode (*this)
            , endDefinition (*this)
            {}
    };
    
    
} } } }
#endif // ACTFUNCTIONDEFINITION__20090123
