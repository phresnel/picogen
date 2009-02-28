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

#ifndef SOURCECODEPOSITION__20090123
#define SOURCECODEPOSITION__20090123

#include <string>

namespace quatsch {
    class SourcecodePosition {
        public:
            unsigned int getLineNumber () const { return lineNumber; }
            unsigned int getColumnNumber () const { return columnNumber; }
            const ::std::string& getFilename () const { return filename; }
            SourcecodePosition (unsigned int lineNumber, unsigned int columnNumber, const ::std::string& filename = ::std::string (""))
            : lineNumber (lineNumber), columnNumber (columnNumber), filename (filename) {}
            bool operator < (const SourcecodePosition &rhs) const {
                if (filename == rhs.filename) {
                    if (lineNumber == rhs.lineNumber) {
                        return columnNumber < rhs.columnNumber;
                    } else {
                        return lineNumber < rhs.lineNumber;
                    }
                } else {
                    return filename < rhs.filename;
                }
            }
        private:
            unsigned int lineNumber;
            unsigned int columnNumber;
            ::std::string filename;
    };
}

#endif // SOURCECODEPOSITION__20090123
