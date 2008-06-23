/***************************************************************************
 *            functional.cc
 *
 *  Sat May 17 19:31:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <ctype.h>

#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include <picogen.h>
#include <picogen/misc/functional/LayeredNoise2d.h>

namespace picogen {
    namespace misc {
        namespace functional {


            static inline BasicFunction* heightslang_ (
                const Setup &setup,                 ///< Stores information, for example how many different parameters are allowed to be used in the inlisp-code
                BasicFunction *root,                ///< The top node, usually one of Function_R1_R1, Function_R2_R1, or similar. Needed to allow recursion in the inlisp-code.
                std::string::const_iterator &it,    ///< Iterator to the current element of "code" being processed.
                const std::string &code             ///< The inlisp code.
            );


            static inline void skipWhitespace (::std::string::const_iterator &it, const ::std::string &code) {
                if (it != code.end()) {
                    //++it;
                    while (
                        (*it == ' '
                         || *it == '\t'
                         || *it == '\n'
                        ) && (
                            it != code.end()
                        )
                    ) {
                        ++it;
                    }
                }
            }

            /// \brief Gets the next token-char from code and iterates to the next one.
            ///
            /// If the 'end-of-file' (more specifically, the end of the string) has already been reached, then '\0' is returned;
            /// \param it Iterator pointing to the current element in "code" being processed.
            /// \param code Reference to the code being processed.
            static inline char nextToken (::std::string::const_iterator &it, const ::std::string &code) {
                skipWhitespace (it, code);
                if (it != code.end()) {
                    char ret = *it;
                    ++it;
                    return ret;
                }
                return '\0';
            }



            /// \brief Peeks to the next token in that it returns the next token, but without iterating to the next one.
            ///
            /// \param it Iterator pointing to the current element in "code" being processed.
            /// \param code Reference to the code being processed.
            static inline char peekNextToken (const ::std::string::const_iterator &it, const ::std::string &code) {
                ::std::string::const_iterator my_it = it;
                char ret = nextToken (my_it,code);
                return ret;
            }



            static inline BasicFunction* createHooked (
                const Setup &setup,                 ///< Stores information, for example how many different parameters are allowed to be used in the inlisp-code
                BasicFunction *root,                ///< The top node, usually one of Function_R1_R1, Function_R2_R1, or similar. Needed to allow recursion in the inlisp-code.
                std::string::const_iterator &it,    ///< Iterator to the current element of "code" being processed.
                const std::string &code             ///< The inlisp code.
            ) {
                using namespace std;
                char tok = nextToken (it, code);
                switch (tok) {
                    case '2':{
                        string hookName = "";
                        skipWhitespace (it, code);
                        while (isalnum (*it) && it!=code.end()) {
                            hookName += *it;
                            ++it;
                        }

                        map<string,string> parameters;
                        skipWhitespace (it, code);
                        while (*it != ']') {
                            // Scan parameter name.
                            skipWhitespace (it, code);
                            if (')' == *it || it == code.end()) {
                                throw functional_general_exeption ("missing ']'");
                            }

                            if (!isalnum (*it)) {
                                throw functional_general_exeption ("expected alphanumeric parameter name");
                            }
                            string parameterName = "";
                            while (isalnum (*it) && it!=code.end()) {
                                parameterName += *it;
                                ++it;
                            }

                            // Scan parameter value.
                            skipWhitespace (it, code);
                            if (it == code.end() || '(' != *it) {
                                throw functional_general_exeption ("missing value for parameter " + parameterName);
                            }

                            ++it; // eat '('
                            int balance = 1; // This will allow e.g. embedded inlisp within the function configuration.
                            string parameterValue = "";
                            while (it!=code.end()) {
                                if (*it==')') {
                                    balance--;
                                    if (balance == 0) {
                                        break;
                                    }
                                }
                                if (*it=='(' ) {
                                    balance++;
                                }
                                parameterValue += *it;
                                ++it;
                            }

                            if (*it != ')' || it == code.end()) {
                                throw functional_general_exeption ("missing ')' for parameter " + parameterName);
                            }

                            ++it; // eat ')'
                            skipWhitespace (it, code);

                            parameters[parameterName] = parameterValue;
                            if (0) {
                                cout << parameterName << "{" << parameters[parameterName] << "}" << endl;
                            }
                        }
                        ++it; // eat ']'
                        cout << endl;

                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                        BasicFunction *p2 = heightslang_ (setup,root,it,code);
                        if (nextToken (it,code) != ')') {
                            throw functional_general_exeption ("missing ')'");
                        }

                        if ("LayeredNoise" == hookName) {
                            return new LayeredNoise2d (parameters, p1, p2);
                        } else {
                            throw functional_general_exeption ("unknown configurable function: '" + hookName + "'");
                        }

                    }break;
                    default:
                        throw functional_general_exeption ("configurable functions are currently only supported with 2 parameters");
                }
                return NULL;
            }



            /// \brief Compiles a source-string in a kind of lisp-syntax into a BasicFunction
            ///
            /// "inlisp" means "inlisp is not lisp" (lisp itself meaning "list processor", which inlisp is definitely not)
            static inline BasicFunction* heightslang_ (
                const Setup &setup,                 ///< Stores information, for example how many different parameters are allowed to be used in the inlisp-code
                BasicFunction *root,                ///< The top node, usually one of Function_R1_R1, Function_R2_R1, or similar. Needed to allow recursion in the inlisp-code.
                std::string::const_iterator &it,    ///< Iterator to the current element of "code" being processed.
                const std::string &code             ///< The inlisp code.
            ) {
                using namespace std;
                for (; it!=code.end() && *it != '\0';) {
                    char tok = nextToken (it,code);
                    if (tok == '(') {
                        switch (tok=nextToken (it,code)) {

                            case '[':
                                return createHooked (setup, root, it, code);
                                break;

                                // +, -, *, /, ^, =
#ifndef FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP
#define FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP( OPERATOR, ALLOCATOR ) \
case OPERATOR:{                                                  \
    BasicFunction *p1 = heightslang_(setup,root,it,code);             \
    BasicFunction *p2 = heightslang_(setup,root,it,code);             \
    if( nextToken(it,code) != ')' ) {                            \
        throw functional_general_exeption( "missing ')'" );      \
    }                                                            \
    return ALLOCATOR( p1, p2 );                                  \
} break;
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('+', add_);
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('-', sub_);
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('*', mul_);
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('/', div_);
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('^', pow_);
                                FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP ('=', equal_);
#undef FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP
#else
#error "FUNCTIONAL_HEIGHTSLANG_IMPLEMENT_BINOP already defiend somewhere else"
#endif

                            // <, <=, <>, >, >=, ?
                            case '<': {
                                if ('=' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return less_equal_ (p1, p2);
                                } else if ('>' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return not_equal_ (p1, p2);
                                } else {
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return less_ (p1, p2);
                                }
                            }
                            break;

                            case '>': {
                                if ('=' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return greater_equal_ (p1, p2);
                                } else {
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return greater_ (p1, p2);
                                }
                            }
                            break;

                            // xor, abs, and, or
                            case 'x': {
                                if ('o' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('r' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return xor_ (p1, p2);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            case 'a': {
                                if ('n' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('d' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return and_ (p1, p2);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else if ('b' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('s' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return abs_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            case 'o': {
                                if ('r' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                    BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                    if (nextToken (it,code) != ')') {
                                        throw functional_general_exeption ("missing ')'");
                                    }
                                    return and_ (p1, p2);
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;


                            case '?': {
                                BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                BasicFunction *p3 = heightslang_ (setup,root,it,code);
                                if (nextToken (it,code) != ')') {
                                    throw functional_general_exeption ("missing ')'");
                                }
                                return if_else_ (p1, p2, p3);
                            }


                            // {neg, not} {inv} {sin,sqrt} {cos} {floor} {trunc}


                            // neg, not
                            case 'n': {
                                if ('e' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('g' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return neg_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else if ('o' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('t' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return not_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            // inv
                            case 'i': {
                                if ('n' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('v' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return inv_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            // sin, sqrt, self
                            case 's': {
                                if ('i' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('n' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return sin_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else if ('q' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('r' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        if ('t' == peekNextToken (it,code)) {
                                            nextToken (it,code);
                                            BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                            if (nextToken (it,code) != ')') {
                                                throw functional_general_exeption ("missing ')'");
                                            }
                                            return sqrt_ (p1);
                                        } else {
                                            throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                        }
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else if ('e' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('l' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        if ('f' == peekNextToken (it,code)) {
                                            nextToken (it,code);
                                            BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                            if (NULL == p1) {
                                                throw functional_general_exeption ("missing argument for 'self'-operation");
                                            }
                                            if (setup.parameterCount < 1) {
                                                throw functional_general_exeption ("it looks like the 'self'-operation is called with 1 argument (or more), but the surrounding function accepts only 0");
                                            }
                                            if (peekNextToken (it,code) == ')') {
                                                if (setup.parameterCount > 1) {
                                                    throw functional_general_exeption ("it looks like the 'self'-operation is called with 1 argument, but the surrounding function requires 2 or more");
                                                }
                                                nextToken (it,code);
                                                return call_ (root, p1);
                                            }
                                            BasicFunction *p2 = heightslang_ (setup,root,it,code);
                                            if (NULL == p2) {
                                                throw functional_general_exeption ("it looks like the 'self'-operation is called with 2 arguments, but the 2nd argument is missing");
                                            }
                                            if (setup.parameterCount < 2) {
                                                throw functional_general_exeption ("it looks like the 'self'-operation is called with 2 arguments (or more), but the surrounding function accepts only 1");
                                            }
                                            if (peekNextToken (it,code) == ')') {
                                                if (setup.parameterCount > 2) {
                                                    throw functional_general_exeption ("it looks like the 'self'-operation is called with 2 arguments, but the surrounding function requires 3 or more");
                                                }
                                                nextToken (it,code);
                                                return call2_ (root, p1, p2);
                                            }


                                            BasicFunction *p3 = heightslang_ (setup,root,it,code);
                                            if (NULL == p3) {
                                                throw functional_general_exeption ("it looks like the 'self'-operation is called with 3 arguments, but the 3rd argument is missing");
                                            }
                                            if (setup.parameterCount < 3) {
                                                throw functional_general_exeption ("it looks like the 'self'-operation is called with 3 arguments (or more), but the surrounding function accepts only 2");
                                            }
                                            if (peekNextToken (it,code) == ')') {
                                                if (setup.parameterCount > 3) {
                                                    throw functional_general_exeption ("it looks like the 'self'-operation is called with 3 argument, but the surrounding function requires 4 or more");
                                                }
                                                nextToken (it,code);
                                                return call3_ (root, p1, p2, p3);
                                            }
                                            throw functional_general_exeption ("unsupported parameter count for operation 'self'");
                                        } else {
                                            throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                        }
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            // cos
                            case 'c': {
                                if ('o' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('s' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                        if (nextToken (it,code) != ')') {
                                            throw functional_general_exeption ("missing ')'");
                                        }
                                        return cos_ (p1);
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;


                            // frac, floor
                            case 'f': {
                                if ('r' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('a' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        if ('c' == peekNextToken (it,code)) {
                                            nextToken (it,code);
                                            BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                            if (nextToken (it,code) != ')') {
                                                throw functional_general_exeption ("missing ')'");
                                            }
                                            return frac_ (p1);
                                        } else {
                                            throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                        }
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else if ('l' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('o' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        if ('o' == peekNextToken (it,code)) {
                                            nextToken (it,code);
                                            if ('r' == peekNextToken (it,code)) {
                                                nextToken (it,code);
                                                BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                                if (nextToken (it,code) != ')') {
                                                    throw functional_general_exeption ("missing ')'");
                                                }
                                                return floor_ (p1);
                                            } else {
                                                throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                            }
                                        } else {
                                            throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                        }
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;

                            // trunc
                            case 't': {
                                if ('r' == peekNextToken (it,code)) {
                                    nextToken (it,code);
                                    if ('u' == peekNextToken (it,code)) {
                                        nextToken (it,code);
                                        if ('n' == peekNextToken (it,code)) {
                                            nextToken (it,code);
                                            if ('c' == peekNextToken (it,code)) {
                                                nextToken (it,code);
                                                BasicFunction *p1 = heightslang_ (setup,root,it,code);
                                                if (nextToken (it,code) != ')') {
                                                    throw functional_general_exeption ("missing ')'");
                                                }
                                                return trunc_ (p1);
                                            } else {
                                                throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                            }
                                        } else {
                                            throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                        }
                                    } else {
                                        throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                    }
                                } else {
                                    throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                                }
                            }
                            break;


                            default: {
                                throw functional_general_exeption (std::string ("unknown operation: ") + tok + peekNextToken (it,code));
                            }
                        }
                    } else switch (tok) {
                            case 'x':
                                if (setup.parameterCount == 0) {
                                    throw functional_general_exeption ("'x' is only declared for functions with at least 1 parameter");
                                }
                                return parameter_ (setup, 0);
                                break;
                            case 'y':
                                if (setup.parameterCount <= 1) {
                                    throw functional_general_exeption ("'y' is only declared for functions with at least 2 parameters");
                                }
                                return parameter_ (setup, 1);
                                break;
                            case 'z':
                                if (setup.parameterCount <= 2) {
                                    throw functional_general_exeption ("'z' is only declared for functions with at least 3 parameters");
                                }
                                return parameter_ (setup, 2);
                                break;
                            case '0':
                            case '1':
                            case '2':
                            case '3':
                            case '4':
                            case '5':
                            case '6':
                            case '7':
                            case '8':
                            case '9':
                            case '-': {
                                int dotCount = 0;
                                real_t sign = 1.0;
                                if (tok == '-') {
                                    tok = nextToken (it,code);
                                    sign = -1;
                                }
                                std::string number ("");
                                while (isdigit (tok) || tok=='.') {
                                    if (tok=='.')
                                        ++dotCount;
                                    number = number + tok;
                                    if (it == code.end()) {
                                        tok = '\0'; // This will cause nothing but to break this loop.
                                    } else {
                                        tok = nextToken (it, code);//*it++; // Next token.
                                    }
                                }
                                --it; // This should fix the bug where the next character after the constant is ignored.
                                if (dotCount > 1) {
                                    throw functional_general_exeption (std::string ("too many dots in float-number: ") + number);
                                }
                                if (number.size()<=0) {
                                    throw functional_general_exeption (std::string ("invalid number (maybe you only wrote a prefix like '-' without the actual number?): '") + number + std::string ("'"));
                                }
                                real_t f;
                                stringstream ss;
                                ss << number;
                                ss >> f;
                                return constant_ (sign*f);
                            }
                            break;
                            default: {
                                throw functional_general_exeption (std::string ("token unknown: ") + tok);
                            }
                            break;
                        }
                }
                return NULL;
            }



/// \brief Compiles a source-string in a kind of lisp-syntax into a BasicFunction
///
/// "inlisp" means "inlisp is not lisp" (lisp itself meaning "list processor", which inlisp is definitely not).
/// This function actually wraps around "static inline BasicFunction* heightslang_( const Setup &setup, BasicFunction
/// *root, std::string::const_iterator &it, const std::string &code )".
            BasicFunction* heightslang_ (
                const Setup &setup,                 ///< Stores information, for example how many different parameters are allowed to be used in the inlisp-code
                BasicFunction *root,                ///< The top node, usually one of Function_R1_R1, Function_R2_R1, or similar. Needed to allow recursion in the inlisp-code.
                const std::string &code             ///< The inlisp code.
            ) {
                if (0) {
                    std::cout << "> " << code << std::endl;
                }
                std::string::const_iterator it = code.begin();
                return heightslang_ (setup, root, it, code);
            }


        }
    }
} // namespace picogen{ namespace misc{ namespace functional{
