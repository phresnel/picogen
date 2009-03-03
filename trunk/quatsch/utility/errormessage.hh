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

#ifndef ERRORMESSAGE__20090123
#define ERRORMESSAGE__20090123

#include <string>
#include "sourcecodeposition.hh"

namespace quatsch {
    class ErrorMessage {
            public:
                enum error_t {
                    critical_error,
                    error, 
                    warning
                };
                ErrorMessage (error_t error_, const ::std::string& message, const SourcecodePosition errorPosition)
                : error_(error_), message(message), errorPosition (errorPosition) {}
                ErrorMessage (const ErrorMessage &e)  : error_(e.error_), message(e.message), errorPosition (e.errorPosition) {}
                ErrorMessage &operator = (const ErrorMessage &e) {
                    error_ = e.error_;
                    message = e.message;
                    errorPosition = e.errorPosition;
                    return *this;
                }
                ::std::string const& getMessage () const { return message; }
                bool isWarning () const { return error_ == warning; }
                bool isCritical () const { return error_ == critical_error; }
                unsigned int getLineNumber () const { return errorPosition.getLineNumber(); }
                unsigned int getColumnNumber () const { return errorPosition.getColumnNumber(); }
                const ::std::string& getFilename () const { return errorPosition.getFilename(); }
                
                bool operator < (ErrorMessage const &rhs) const {
                    return errorPosition < rhs.errorPosition;
                }
            private:
                ErrorMessage ();
                error_t error_;
                ::std::string message;
                SourcecodePosition errorPosition;
    };
}

#endif // ERRORMESSAGE__20090123
