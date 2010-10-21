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

#ifndef EST_BACKENDDEF__20090123
#define EST_BACKENDDEF__20090123

#include "backend.hh"

namespace quatsch { namespace backend { namespace est {

template <typename R, typename P>
void Backend<R,P> :: addParameter (const ::std::string &name)
{
        const size_t size = parameters.back().size();
        parameters.back().addSymbol (name, size);
}



template <typename R, typename P>
typename Backend<R,P>::FunctionPtr
Backend<R,P> :: getFunction ()
{
        ::boost::shared_array <FunctionPtr> functions (
                new FunctionPtr [this->functions.size()]
        );
        
        if (debug) {
                ::std::cout << "declared functions: " << ::std::endl;
        }

        int i = 0;
        for (typename SymbolTable<FunctionDescriptor<Backend> >::const_iterator
             it=this->functions.begin();
             it!=this->functions.end();
             ++it
            ) {
                functions [i++] = it->second.getFunction();
                if (debug) {
                        ::std::cout << "  " << it->first << ::std::endl;
                }
        }
        
        return node_types<Backend<R,P> >::RootFunction::create (
                operands.back().back(), functions
        );
}



template <typename R, typename P>
Backend<R,P> :: ~Backend ()
{
        typedef typename ::quatsch::
                SymbolTable<FunctionDescriptor<Backend> >::const_iterator
                symbol_table_const_iterator;

        /*if (0 < operands.size()) {
            ::std::vector <scalar_t> params;
            params.push_back (1);
            params.push_back (2);
            params.push_back (3);
            scalar_t r = (*operands.back().back()) (params);
            ::std::cout << "r:" << r << ::std::endl;
        }*/
        
        if (debug) {
                if (0 < functions.size()) {
                        ::std::cout << "User-Functions:\n";
                        for (symbol_table_const_iterator it=functions.begin();
                             it != functions.end();
                             ++it
                        ) {
                                ::std::cout << '\t'
                                            << it->second.getName()
                                            << ':' 
                                            << it->second.getParameterCount()
                                            << ':'
                                            << it->second.getFunction().get()
                                            << '\n';
                        }
                }
        }
}



template <typename R, typename P>
Backend<R,P> :: Backend (const ConfigurableFunctionsMap &configurableFunctions)
: configurableFunctions (configurableFunctions)
, program (*this)
, functionDefinition (*this)
, codeDefinition (*this)
, syntaxError (*this)
{
        parameters.push_back (SymbolTable <unsigned int> ());
        operands.push_back (::std::vector <FunctionPtr> ());
}



template <typename R, typename P>
void Backend<R, P> :: dumpErrorMessagesAndThrow (std::ostream &cerr) const
{
        typedef typename ::quatsch::
                SymbolTable<FunctionDescriptor<Backend> >::const_iterator
                symbol_table_const_iterator;
                
                
        using namespace std;
        bool valid = true;

        ::std::vector <ErrorMessage> errorMessages (this->errorMessages);

        // Check for outstanding function definitions:
        //  After we are done with parsing, we can check whether all forward 
        //  declared functions (or 'incomplete calls') have been resolved. 
        //  Luckily FunctionDescriptor saved all calls to a function until it 
        //  would be defined. We must simply call FunctionDescriptor::probe() 
        //  to provoke an exception.
        
        if (0 < functions.size()) {
                bool all_forward_declarations_resolved = true;
                
                for (symbol_table_const_iterator it=functions.begin();
                     it != functions.end();
                     ++it
                ) {
                        if (!it->second.isComplete()) {
                                const ::std::string message (
                                        "call to unknown function \"" + 
                                        it->second.getName() + "\".");
                                
                                ::std::vector <SourcecodePosition> pos;
                                
                                it->second.pushIncompleteCallPositions (pos);
                                
                                for (typename ::std::vector<SourcecodePosition>
                                        ::const_iterator pp=pos.begin();
                                     pp!=pos.end(); ++pp
                                ) {
                                        errorMessages.push_back (ErrorMessage (
                                                ErrorMessage::error, 
                                                message, 
                                                *pp
                                        ));
                                }
                                all_forward_declarations_resolved = false;
                        }
                }
                if (!all_forward_declarations_resolved) {
                        valid = false;
                }
        }
        

        // Output errors and warnings.
        //sort (errorMessages.begin(), errorMessages.end());
        for (typename ::std::vector<ErrorMessage>::const_iterator
                it=errorMessages.begin();
             it != errorMessages.end();
             ++it
        ) {
                if (it->isWarning()) {
                        cerr << it->getFilename() << ':'
                             << it->getLineNumber() << "(";
                        cerr.width (2);
                        cerr << right << it->getColumnNumber();
                        cerr.width (0);
                        cerr << "): warning: " << it->getMessage() << '\n';
                } else {
                        cerr << it->getFilename() << ':'
                             << it->getLineNumber() << "(";
                        cerr.width (2);
                        cerr << right << it->getColumnNumber();
                        cerr.width (0);
                        
                        if (it->isCritical()) {
                                cerr << "): critical error: "
                                     << it->getMessage() << '\n';
                        } else {
                                cerr << "): error: "
                                     << it->getMessage() << '\n';
                        }
                        
                        valid = false;
                }
        }

        // Banzai.
        if (!valid) {
                cerr <<"error: compilation failed. see above messages."<< endl;
                throw invalid_program_exception ();
        }
}

} } }
#endif // EST_BACKENDDEF__20090123
