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

#ifndef ACTSYNTAXERROR__20090123
#define ACTSYNTAXERROR__20090123
namespace quatsch {  namespace backend {  namespace est {  namespace parser_actions  {
    template <typename BACKEND> class SyntaxError {
            enum { debug=0 };
            BACKEND &backend;
            ::std::vector <SourcecodePosition> frames;
        public:
            class BeginFrame {
                    SyntaxError &syntaxError;
                public:
                    BeginFrame (SyntaxError &syntaxError) : syntaxError (syntaxError) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t& /*to*/) const {
                        if (debug) ::std::cout << "error.beginFrame" << ::std::endl;
                        syntaxError.frames.push_back (
                            SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file));
                    }
            };

            class EndFrame {
                    SyntaxError &syntaxError;
                public:
                    EndFrame (SyntaxError &syntaxError) : syntaxError (syntaxError) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& /*from*/, const code_iterator_t& /*to*/) const {
                        if (debug) ::std::cout << "error.endFrame" << ::std::endl;
                        syntaxError.frames.pop_back ();
                    }
            };
            
            class InvalidProgram {
                    BACKEND &backend;
                public:
                    InvalidProgram (BACKEND &backend) : backend (backend) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t& to) const {
                        using namespace std;
                        stringstream ss;
                        ss << "syntax error, compilation stopped at '" << *to << "':\n";                                        
                        
                        // Compose a fancy message like icc does (I think).
                        stringstream ss2;
                        code_iterator_t i = from;
                        while (i.get_position().line != to.get_position().line) ++i; // Fast forward to current line.
                        unsigned int len = 0;
                        ss2 << "    error-line: \"";
                        for (; i != to; ++i) {
                            ss2 << *i;
                            ++len;
                        }
                        ss2 << *i << "\"\n    where.....:  ";
                        for (unsigned int i=0; i<len; ++i)
                            ss2 << " ";
                        ss2 << "^ at that point.";
                        // Done, pump fancy message into error message.
                        ss << ss2.str();
                        backend.errorMessages.push_back (ErrorMessage (ErrorMessage::critical_error, ss.str(), 
                            SourcecodePosition (to.get_position().line, to.get_position().column, to.get_position().file)));
                    }
            };

            class Expected {
                    BACKEND &backend;
                public:

                    struct expected_t {
                        BACKEND &backend;
                        ::std::string what;
                        expected_t (BACKEND &backend, const ::std::string &what) : backend (backend), what (what) {}
                        
                        template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t& /*to*/) const {
                            backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ::std::string("expected \"") + what + "\".", 
                                SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file)));
                        }
                    };
                    
                    // TODO: should we store this in a std::map?
                    expected_t operator [] (const ::std::string &what) const {
                        return expected_t (backend, what);
                    }

                    Expected (BACKEND &backend) : backend (backend) {}
            };
            
            class Unexpected {
                    BACKEND &backend;
                public:

                    template <typename code_iterator_t> void operator () (const code_iterator_t& from, const code_iterator_t& /*to*/) const {
                        backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, ::std::string("unexpected."), 
                            SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file)));
                    }
                    
                    Unexpected (BACKEND &backend) : backend (backend) {}
            };

            class MissingClosing {
                    SyntaxError &syntaxError;
                    BACKEND &backend;
                public:

                    struct missingclosing_t {
                        SyntaxError &syntaxError;
                        BACKEND &backend;
                        ::std::string beginSymbol;
                        ::std::string endSymbol;
                        missingclosing_t (SyntaxError &syntaxError, const ::std::string &beginSymbol, const ::std::string &endSymbol)
                        : syntaxError (syntaxError), backend (syntaxError.backend), beginSymbol (beginSymbol), endSymbol (endSymbol) {}
                        
                        template <typename code_iterator_t> void operator () (const code_iterator_t& /*from*/, const code_iterator_t& /*to*/) const {
                            backend.errorMessages.push_back (ErrorMessage (ErrorMessage::error, 
                                ::std::string("for the opening '") + beginSymbol + "'-symbol, no corresponding '" + endSymbol + "'-symbol has been found.",
                                //SourcecodePosition (from.get_position().line, from.get_position().column, from.get_position().file)));
                                syntaxError.frames.back()));
                        }
                    };
                    
                    friend class missingclosing_t;

                    // TODO: should we store this in a std::map?
                    missingclosing_t operator () (const ::std::string &beginSymbol, const ::std::string &endSymbol) const {
                        return missingclosing_t (syntaxError, beginSymbol, endSymbol);
                    }

                    MissingClosing (SyntaxError &syntaxError) : syntaxError (syntaxError), backend (syntaxError.backend) {}
            };

            friend class InvalidProgram ;   InvalidProgram invalidProgram;
            friend class Expected       ;   Expected       expected;
            friend class Unexpected     ;   Unexpected     unexpected;
            friend class MissingClosing ;   MissingClosing missingClosing;
            friend class BeginFrame     ;   BeginFrame     beginFrame;
            friend class EndFrame       ;   EndFrame       endFrame;

            SyntaxError (BACKEND &backend) 
            : backend (backend), invalidProgram (backend)
            , expected (backend), unexpected (backend), missingClosing (*this)
            , beginFrame (*this), endFrame  (*this)
            {}
    };
} } } }
#endif // ACTSYNTAXERROR__20090123
