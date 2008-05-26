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


#include <picogen.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctype.h>


namespace picogen{ namespace misc{ namespace functional{


/*RR::BasicFunction *optimize( RR::BasicFunction *f ){
    return f;
}*/



static inline char nextToken( std::string::const_iterator &it, const std::string &code ){
    if( it != code.end() ) {
        //++it;
        while(
            (    *it == ' '
              || *it == '\t'
              || *it == '\n'
            ) && (
              it != code.end()
            )
        ){
             ++it;
        }
    }
    if( it != code.end() ){
        char ret = *it;
        ++it;
        return ret;
    }
    return '\0';
}


static inline char peekNextToken( std::string::const_iterator &it, const std::string &code ){
    std::string::const_iterator prev = it;
    char ret = nextToken(it,code);
    it = prev;
    return ret;
}


static inline BasicFunction* inlisp_(
    const Setup &setup,
    BasicFunction *root,
    std::string::const_iterator &it,
    const std::string &code
) {
    using namespace std;
    for( ; it!=code.end() && *it != '\0';  ){
        char tok = nextToken(it,code);
        if( tok == '(' ){
            switch( tok=nextToken(it,code) ){

                // +, -, *, /, ^, =
                #ifndef FUNCTIONAL_INLISP_IMPLEMENT_BINOP
                #define FUNCTIONAL_INLISP_IMPLEMENT_BINOP( OPERATOR, ALLOCATOR ) \
                case OPERATOR:{                                                  \
                    BasicFunction *p1 = inlisp_(setup,root,it,code);             \
                    BasicFunction *p2 = inlisp_(setup,root,it,code);             \
                    if( nextToken(it,code) != ')' ) {                            \
                        throw functional_general_exeption( "missing ')'" );      \
                    }                                                            \
                    return ALLOCATOR( p1, p2 );                                  \
                } break;
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '+', add_ );
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '-', sub_ );
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '*', mul_ );
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '/', div_ );
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '^', pow_ );
                FUNCTIONAL_INLISP_IMPLEMENT_BINOP( '=', equal_ );
                #undef FUNCTIONAL_INLISP_IMPLEMENT_BINOP
                #else
                #error "FUNCTIONAL_INLISP_IMPLEMENT_BINOP already defiend somewhere else"
                #endif

                // <, <=, <>, >, >=, ?
                case '<':{
                    if( '=' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return less_equal_( p1, p2 );
                    }else if( '>' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return not_equal_( p1, p2 );
                    }else{
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return less_( p1, p2 );
                    }
                } break;

                case '>':{
                    if( '=' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return greater_equal_( p1, p2 );
                    }else{
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return greater_( p1, p2 );
                    }
                } break;

                // xor, and, or
                case 'x':{
                    if( 'o' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'r' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            BasicFunction *p2 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return xor_( p1, p2 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                case 'a':{
                    if( 'n' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'd' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            BasicFunction *p2 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return and_( p1, p2 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                case 'o':{
                    if( 'r' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        BasicFunction *p1 = inlisp_(setup,root,it,code);
                        BasicFunction *p2 = inlisp_(setup,root,it,code);
                        if( nextToken(it,code) != ')' ) {
                            throw functional_general_exeption( "missing ')'" );
                        }
                        return and_( p1, p2 );
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;


                case '?':{
                    BasicFunction *p1 = inlisp_(setup,root,it,code);
                    BasicFunction *p2 = inlisp_(setup,root,it,code);
                    BasicFunction *p3 = inlisp_(setup,root,it,code);
                    if( nextToken(it,code) != ')' ) {
                        throw functional_general_exeption( "missing ')'" );
                    }
                    return if_else_( p1, p2, p3 );
                }


                // {neg, not} {inv} {sin,sqrt} {cos} {floor} {trunc}

                // neg, not
                case 'n':{
                    if( 'e' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'g' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return neg_( p1 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else if( 'o' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 't' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return not_( p1 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                // inv
                case 'i':{
                    if( 'n' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'v' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return inv_( p1 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                // sin, sqrt, self
                case 's':{
                    if( 'i' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'n' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return sin_( p1 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else if( 'q' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'r' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            if( 't' == peekNextToken(it,code) ){
                                nextToken(it,code);
                                BasicFunction *p1 = inlisp_(setup,root,it,code);
                                if( nextToken(it,code) != ')' ) {
                                    throw functional_general_exeption( "missing ')'" );
                                }
                                return sqrt_( p1 );
                            }else{
                                throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                            }
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else if( 'e' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'l' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            if( 'f' == peekNextToken(it,code) ){
                                nextToken(it,code);
                                BasicFunction *p1 = inlisp_(setup,root,it,code);
                                if( NULL == p1 ) {
                                    throw functional_general_exeption( "missing argument for 'self'-operation" );
                                }
                                if( setup.parameterCount < 1 ){
                                    throw functional_general_exeption( "it looks like the 'self'-operation is called with 1 argument (or more), but the surrounding function accepts only 0" );
                                }
                                if( peekNextToken(it,code) == ')' ){
                                    if( setup.parameterCount > 1 ){
                                        throw functional_general_exeption( "it looks like the 'self'-operation is called with 1 argument, but the surrounding function requires 2 or more" );
                                    }
                                    nextToken(it,code);
                                    return call_( root, p1 );
                                }
                                BasicFunction *p2 = inlisp_(setup,root,it,code);
                                if( NULL == p2 ) {
                                    throw functional_general_exeption( "it looks like the 'self'-operation is called with 2 arguments, but the 2nd argument is missing" );
                                }
                                if( setup.parameterCount < 2 ){
                                    throw functional_general_exeption( "it looks like the 'self'-operation is called with 2 arguments (or more), but the surrounding function accepts only 1" );
                                }
                                if( peekNextToken(it,code) == ')' ){
                                    if( setup.parameterCount > 2 ){
                                        throw functional_general_exeption( "it looks like the 'self'-operation is called with 2 arguments, but the surrounding function requires 3 or more" );
                                    }
                                    nextToken(it,code);
                                    return call2_( root, p1, p2 );
                                }


                                BasicFunction *p3 = inlisp_(setup,root,it,code);
                                if( NULL == p3 ) {
                                    throw functional_general_exeption( "it looks like the 'self'-operation is called with 3 arguments, but the 3rd argument is missing" );
                                }
                                if( setup.parameterCount < 3 ){
                                    throw functional_general_exeption(  "it looks like the 'self'-operation is called with 3 arguments (or more), but the surrounding function accepts only 2" );
                                }
                                if( peekNextToken(it,code) == ')' ){
                                    if( setup.parameterCount > 3 ){
                                        throw functional_general_exeption( "it looks like the 'self'-operation is called with 3 argument, but the surrounding function requires 4 or more" );
                                    }
                                    nextToken(it,code);
                                    return call3_( root, p1, p2, p3 );
                                }
                                throw functional_general_exeption( "unsupported parameter count for operation 'self'" );
                            }else{
                                throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                            }
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                // cos
                case 'c':{
                    if( 'o' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 's' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            BasicFunction *p1 = inlisp_(setup,root,it,code);
                            if( nextToken(it,code) != ')' ) {
                                throw functional_general_exeption( "missing ')'" );
                            }
                            return cos_( p1 );
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;


                // frac, floor
                case 'f':{
                    if( 'r' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'a' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            if( 'c' == peekNextToken(it,code) ){
                                nextToken(it,code);
                                BasicFunction *p1 = inlisp_(setup,root,it,code);
                                if( nextToken(it,code) != ')' ) {
                                    throw functional_general_exeption( "missing ')'" );
                                }
                                return frac_( p1 );
                            }else{
                                throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                            }
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else if( 'l' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'o' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            if( 'o' == peekNextToken(it,code) ){
                                nextToken(it,code);
                                if( 'r' == peekNextToken(it,code) ){
                                    nextToken(it,code);
                                    BasicFunction *p1 = inlisp_(setup,root,it,code);
                                    if( nextToken(it,code) != ')' ) {
                                        throw functional_general_exeption( "missing ')'" );
                                    }
                                    return floor_( p1 );
                                }else{
                                    throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                                }
                            }else{
                                throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                            }
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;

                // trunc
                case 't':{
                    if( 'r' == peekNextToken(it,code) ){
                        nextToken(it,code);
                        if( 'u' == peekNextToken(it,code) ){
                            nextToken(it,code);
                            if( 'n' == peekNextToken(it,code) ){
                                nextToken(it,code);
                                if( 'c' == peekNextToken(it,code) ){
                                    nextToken(it,code);
                                    BasicFunction *p1 = inlisp_(setup,root,it,code);
                                    if( nextToken(it,code) != ')' ) {
                                        throw functional_general_exeption( "missing ')'" );
                                    }
                                    return trunc_( p1 );
                                }else{
                                    throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                                }
                            }else{
                                throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                            }
                        }else{
                            throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                        }
                    }else{
                        throw functional_general_exeption( std::string("unknown operation: ") + tok + peekNextToken(it,code) );
                    }
                } break;


                default:{
                    throw functional_general_exeption( std::string("unknown operation: ") );
                }
            }
        }else switch( tok ){
            case 'x':
                if( setup.parameterCount == 0 ){
                    throw functional_general_exeption( "'x' is only declared for functions with at least 1 parameter" );
                }
                return parameter_(0);
                break;
            case 'y':
                if( setup.parameterCount <= 1 ){
                    throw functional_general_exeption( "'y' is only declared for functions with at least 2 parameters" );
                }
                return parameter_(1);
                break;
            case 'z':
                if( setup.parameterCount <= 2 ){
                    throw functional_general_exeption( "'z' is only declared for functions with at least 3 parameters" );
                }
                return parameter_(2);
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '-': {
                int dotCount = 0;
                real_t sign = 1.0;
                if( tok == '-' ){
                    tok = nextToken(it,code);
                    sign = -1;
                }
                std::string number( "" );
                while( (isdigit( tok ) || tok=='.') && it != code.end() ){
                    if( tok=='.' )
                        ++dotCount;
                    number = number + tok;
                    tok = *it++; // look ahead
                }
                --it;
                if( dotCount > 1 ){
                    throw functional_general_exeption( std::string("too many dots in float-number: ") + number );
                }
                real_t f;
                stringstream ss;
                ss << number;
                ss >> f;
                return constant_( sign*f );
            } break;
            default:{
                throw functional_general_exeption( std::string("token unknown: ") + tok );
            } break;
        }
    }
    return NULL;
}
BasicFunction* inlisp_( const Setup &setup, BasicFunction *root, const std::string &code ) {
    std::cout << "> " << code << std::endl;
    std::string::const_iterator it = code.begin();
    return inlisp_( setup, root, it, code );
}


} } } // namespace picogen{ namespace misc{ namespace functional{
