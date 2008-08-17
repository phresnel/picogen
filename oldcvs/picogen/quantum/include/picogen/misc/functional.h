/***************************************************************************
 *            functional.h
 *
 *  Sat May 25 15:00:00 2008
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


#ifndef _FUNCTIONAL_H
#define _FUNCTIONAL_H


// we are operating fully on floating point, no other datatypes, so true==1.0, false==0.0;
// this will then also allow for some "branch free" code :P
#ifdef FUNCTIONAL_BOOL_TO_REAL
#error "FUNCTIONAL_BOOL_TO_REAL has already been defined somwehere else..."
#endif
#define FUNCTIONAL_BOOL_TO_REAL(x)   ((x)?1.0:0.0)

// from above FUNCTIONAL_BOOL_TO_REAL we get 1.0==true, 0.0==false
// to respect inaccuracy, we will loose that condition to get
//    bool(x) ::= x>=0.5 ? true : false
// .
#ifdef FUNCTIONAL_REAL_TO_BOOL
#error "FUNCTIONAL_REAL_TO_BOOL has already been defined somwehere else..."
#endif
#define FUNCTIONAL_REAL_TO_BOOL(x)   ((x)>0.5?true:false)


/*#include <string>
#include <iostream>
#include <sstream>
#include <ctype.h>*/

namespace picogen {
    namespace misc {

        namespace functional {
            class functional_general_exeption {
                    const ::std::string message;
                public:
                    explicit functional_general_exeption (const ::std::string &message) : message (message) {};
                    const ::std::string &getMessage() const {
                        return message;
                    }
            };
            typedef ::picogen::real real_t;

#if 0
// below container is too slow to be used in practice for BasicFunction-calls
// but it still is usefull because of it's branch-free, yet safe opertator [],
// and the class is therefore kept for future use
            class Parameters {
                    real_t *parameters;
                    unsigned int indexMask;
                public:
                    Parameters (int numParameters) {
                        using namespace ::std;
                        int numBits = 1;
                        int maxPossibleIndex = 2; // don't forget: with one bit you can address two entities
                        this->indexMask = 1;
                        while (maxPossibleIndex < numParameters) {
                            maxPossibleIndex <<= 1;
                            ++numBits;
                            this->indexMask = (this->indexMask << 1) | 0x1;
                        }
                        if (0) {
                            cout << "requested number of parameters: " << numParameters << endl;
                            cout << "max possible index: " << maxPossibleIndex << endl;
                            cout << "number of bits: " << numBits << endl;
                            cout << "index mask: " << this->indexMask << endl;
                        }
                        this->parameters = new real_t[maxPossibleIndex];
                    }
                    virtual ~Parameters() {
                        indexMask = 0;
                        delete [] parameters;
                    }
                    real_t &operator [] (unsigned int index) {
                        return parameters[ index & indexMask ];
                    }
                    real_t operator [] (unsigned int index) const {
                        return parameters[ index & indexMask ];
                    }
            };
            template <int N> class FastParameters : public Parameters {
            };
#endif



            struct Setup {
                const unsigned int parameterCount;
                Setup (unsigned int parameterCount) : parameterCount (parameterCount) {}
            };



            class BasicFunction {
                public:
                    virtual real_t operator () (const real_t * const parameters) const = 0;
            };



            class Constant : public BasicFunction  {
                    real_t value;
                    explicit Constant (real_t value) : value (value) {}
                    friend BasicFunction * constant_ (real_t value);
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return value;
                    }
            };
            inline BasicFunction * constant_ (real_t value) {
                return new Constant (value);
            }



            class Parameter : public BasicFunction {
                    const unsigned int index;
                    explicit Parameter (unsigned int index) : index (index) {}
                    friend BasicFunction * parameter_ (const Setup &setup, unsigned int index);
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return parameters[index];
                    }
            };
            inline BasicFunction * parameter_ (const Setup &setup, unsigned int index) {
                if (index > setup.parameterCount) {
                    ::std::cerr << "index too large. max allowed index is " << setup.parameterCount << ::std::endl;
                }
                return new Parameter (index);
            }



#ifndef FUNCTIONAL_RR_IMPLEMENT_BINOP
#define FUNCTIONAL_RR_IMPLEMENT_BINOP( CLASSNAME, ALLOCNAME, OP, WRAP )             \
    class CLASSNAME;                                                                    \
    inline BasicFunction* ALLOCNAME( BasicFunction *lhs, BasicFunction *rhs );          \
    class CLASSNAME : public BasicFunction {                                            \
            BasicFunction * lhs, * rhs;                                                 \
        protected:                                                                      \
            explicit CLASSNAME( BasicFunction *lhs, BasicFunction *rhs )                \
                    : lhs(lhs), rhs(rhs) {}                                                     \
            friend BasicFunction* ALLOCNAME( BasicFunction *lhs, BasicFunction *rhs );  \
        public:                                                                         \
            virtual real_t operator () ( const real_t * const parameters ) const{       \
                return WRAP( (*lhs)( parameters ) OP (*rhs)( parameters ) );            \
            }                                                                           \
            virtual ~CLASSNAME() {                                                      \
                if( 0 != lhs ) {                                                     \
                    delete lhs;                                                         \
                }                                                                       \
                if( 0 != rhs ) {                                                     \
                    delete rhs;                                                         \
                }                                                                       \
            }                                                                           \
    };                                                                                  \
    inline BasicFunction* ALLOCNAME( BasicFunction *lhs, BasicFunction *rhs ){          \
        return new CLASSNAME( lhs, rhs );                                               \
    }
#else
#error "FUNCTIONAL_RR_IMPLEMENT_BINOP has already been defined somewhere else"
#endif


            FUNCTIONAL_RR_IMPLEMENT_BINOP (Add, add_, +,);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Sub, sub_, -,);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Mul, mul_, *,);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Div, div_, /,);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Less,         less_,          <,  FUNCTIONAL_BOOL_TO_REAL);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (LessEqual,    less_equal_,    <=, FUNCTIONAL_BOOL_TO_REAL);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Greater,      greater_,       >,  FUNCTIONAL_BOOL_TO_REAL);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (GreaterEqual, greater_equal_, >,  FUNCTIONAL_BOOL_TO_REAL);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (Equal,        equal_,         ==, FUNCTIONAL_BOOL_TO_REAL);
            FUNCTIONAL_RR_IMPLEMENT_BINOP (NotEqual,     not_equal_,     !=, FUNCTIONAL_BOOL_TO_REAL);

#undef FUNCTIONAL_RR_IMPLEMENT_BINOP

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN
#define FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( CLASSNAME, ALLOCNAME )                     \
    class CLASSNAME;                                                                    \
    inline BasicFunction* ALLOCNAME( BasicFunction *p1 );                               \
    class CLASSNAME : public BasicFunction {                                            \
            BasicFunction * p1;                                                         \
        protected:                                                                      \
            explicit CLASSNAME( BasicFunction *p1 )                                     \
                    : p1(p1) {}                                                                 \
            friend BasicFunction* ALLOCNAME( BasicFunction *p1 );                       \
        public:                                                                         \
            virtual ~CLASSNAME() {                                                      \
                if( 0 != p1 ){                                                       \
                    delete p1;                                                          \
                }                                                                       \
            }                                                                           \
        private:
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN has already beend defined somewhere else"
#endif

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_1_END
#define FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( CLASSNAME, ALLOCNAME )                   \
};                                                                                  \
inline BasicFunction* ALLOCNAME( BasicFunction *p1 ){                               \
    return new CLASSNAME( p1 );                                                     \
}
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_1_END has already been defined somewhere else"
#endif


            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Inv, inv_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return 1.0 / (*p1) (parameters);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Inv, inv_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Sin, sin_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return sin ( (*p1) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Sin, sin_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Cos, cos_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return sin ( (*p1) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Cos, cos_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Floor, floor_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return ::floor ( (*p1) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Floor, floor_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Abs, abs_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return ::fabs ( (*p1) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Abs, abs_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Trunc, trunc_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return static_cast<real_t> (static_cast<int> ( (*p1) (parameters)));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Trunc, trunc_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Frac, frac_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        real_t tmp = (*p1) (parameters);
                        return tmp - ::floor (tmp);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Frac, frac_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Neg, neg_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return - (*p1) (parameters);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Neg, neg_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Not, not_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const bool a = !FUNCTIONAL_REAL_TO_BOOL ( (*p1) (parameters));
                        return FUNCTIONAL_BOOL_TO_REAL (a);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Not, not_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN (Sqrt, sqrt_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return ::sqrt ( (*p1) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_1_END (Sqrt, sqrt_);

#undef FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN
#undef FUNCTIONAL_RR_IMPLEMENT_FUN_1_END

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN
#define FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN( CLASSNAME, ALLOCNAME )                     \
    class CLASSNAME;                                                                    \
    inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2 );            \
    class CLASSNAME : public BasicFunction {                                            \
            BasicFunction * p1, * p2;                                                   \
        protected:                                                                      \
            explicit CLASSNAME( BasicFunction *p1, BasicFunction *p2 )                  \
                    : p1(p1), p2(p2) {}                                                         \
            friend BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2 );    \
        public:                                                                         \
            virtual ~CLASSNAME() {                                                      \
                if( 0 != p1 ){                                                       \
                    delete p1;                                                          \
                }                                                                       \
                if( 0 != p2 ){                                                       \
                    delete p2;                                                          \
                }                                                                       \
            }                                                                           \
        private:
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN has already been defined somewhere else"
#endif

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_2_END
#define FUNCTIONAL_RR_IMPLEMENT_FUN_2_END( CLASSNAME, ALLOCNAME )                   \
};                                                                                  \
inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2 ){            \
    return new CLASSNAME( p1, p2 );                                                 \
}
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_2_END has already been defined somewhere else"
#endif

            FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN (Pow, pow_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return ::pow ( (*p1) (parameters), (*p2) (parameters));
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_2_END (Pow, pow_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN (And, and_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const bool a = FUNCTIONAL_REAL_TO_BOOL ( (*p1) (parameters));
                        const bool b = a && FUNCTIONAL_REAL_TO_BOOL ( (*p2) (parameters));
                        return  FUNCTIONAL_BOOL_TO_REAL (b);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_2_END (And, and_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN (Or, or_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const bool a = FUNCTIONAL_REAL_TO_BOOL ( (*p1) (parameters));
                        const bool b = a || FUNCTIONAL_REAL_TO_BOOL ( (*p2) (parameters));
                        return  FUNCTIONAL_BOOL_TO_REAL (b);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_2_END (Or, or_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN (Xor, xor_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const bool a = FUNCTIONAL_REAL_TO_BOOL ( (*p1) (parameters));
                        const bool b = FUNCTIONAL_REAL_TO_BOOL ( (*p2) (parameters));
                        const bool c = a != b;
                        return  FUNCTIONAL_BOOL_TO_REAL (c);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_2_END (Xor, xor_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN (Call, call_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const real_t parameters_ [] = {
                            (*p2) (parameters)
                        };
                        return (*p1) (parameters_);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_2_END (Call, call_);

#undef FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN
#undef FUNCTIONAL_RR_IMPLEMENT_FUN_2_END


#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN
#define FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN( CLASSNAME, ALLOCNAME )                 \
    class CLASSNAME;                                                                    \
    inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,              \
                                     BasicFunction *p3 );                                \
    class CLASSNAME : public BasicFunction {                                            \
            BasicFunction * p1, * p2, * p3;                                             \
        protected:                                                                      \
            explicit CLASSNAME( BasicFunction*p1, BasicFunction*p2, BasicFunction*p3 )  \
                    : p1(p1), p2(p2), p3(p3) {}                                                 \
            friend BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,      \
                                             BasicFunction *p3 );                                \
        public:                                                                         \
            virtual ~CLASSNAME() {                                                      \
                if( 0 != p1 ){                                                       \
                    delete p1;                                                          \
                }                                                                       \
                if( 0 != p2 ){                                                       \
                    delete p2;                                                          \
                }                                                                       \
                if( 0 != p3 ){                                                       \
                    delete p3;                                                          \
                }                                                                       \
            }                                                                           \
        private:
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN has already been defined somewhere else"
#endif

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_3_END
#define FUNCTIONAL_RR_IMPLEMENT_FUN_3_END( CLASSNAME, ALLOCNAME )                   \
};                                                                                  \
inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,              \
                                 BasicFunction *p3 ){                                                    \
    return new CLASSNAME( p1, p2, p3 );                                             \
}
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_3_END has already been defined somewhere else"
#endif

            FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN (IfElse, if_else_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        return FUNCTIONAL_REAL_TO_BOOL ( (*p1) (parameters))
                               ? (*p2) (parameters)
                               : (*p3) (parameters)
                               ;
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_3_END (IfElse, if_else_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN (Lerp, lerp_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const real_t a = (*p1) (parameters);
                        const real_t b = (*p2) (parameters);
                        const real_t f = (*p3) (parameters);
                        return a*(1-f) + b*f; // I find this most natural, since with increasing f one increases slowly from a to b.
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_3_END (Lerp, lerp_);



            FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN (Call2, call2_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const real_t parameters_ [] = {
                            (*p2) (parameters),
                            (*p3) (parameters)
                        };
                        return (*p1) (parameters_);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_3_END (Call2, call2_);



#undef FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN
#undef FUNCTIONAL_RR_IMPLEMENT_FUN_3_END


#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_4_BEGIN
#define FUNCTIONAL_RR_IMPLEMENT_FUN_4_BEGIN( CLASSNAME, ALLOCNAME )                 \
    class CLASSNAME;                                                                    \
    inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,              \
                                     BasicFunction *p3, BasicFunction *p4 );             \
    class CLASSNAME : public BasicFunction {                                            \
            BasicFunction * p1, * p2, * p3, * p4;                                       \
        protected:                                                                      \
            explicit CLASSNAME( BasicFunction*p1, BasicFunction*p2,                     \
                                BasicFunction*p3, BasicFunction*p4 )                                    \
                    : p1(p1), p2(p2), p3(p3), p4(p4) {}                                         \
            friend BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,      \
                                             BasicFunction *p3, BasicFunction *p4 );             \
        public:                                                                         \
            virtual ~CLASSNAME() {                                                      \
                if( 0 != p1 ){                                                       \
                    delete p1;                                                          \
                }                                                                       \
                if( 0 != p2 ){                                                       \
                    delete p2;                                                          \
                }                                                                       \
                if( 0 != p3 ){                                                       \
                    delete p3;                                                          \
                }                                                                       \
                if( 0 != p4 ){                                                       \
                    delete p4;                                                          \
                }                                                                       \
            }                                                                           \
        private:
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_4_BEGIN has already been defined somewhere else"
#endif

#ifndef FUNCTIONAL_RR_IMPLEMENT_FUN_4_END
#define FUNCTIONAL_RR_IMPLEMENT_FUN_4_END( CLASSNAME, ALLOCNAME )                   \
};                                                                                  \
inline BasicFunction* ALLOCNAME( BasicFunction *p1, BasicFunction *p2,              \
                                 BasicFunction *p3, BasicFunction *p4 ){                                 \
    return new CLASSNAME( p1, p2, p3, p4 );                                         \
}
#else
#error "FUNCTIONAL_RR_IMPLEMENT_FUN_4_END has already been defined somewhere else"
#endif


            FUNCTIONAL_RR_IMPLEMENT_FUN_4_BEGIN (Call3, call3_)
                public:
                    virtual real_t operator () (const real_t * const parameters) const {
                        const real_t parameters_ [] = {
                            (*p2) (parameters),
                            (*p3) (parameters),
                            (*p4) (parameters)
                        };
                        return (*p1) (parameters_);
                    }
            FUNCTIONAL_RR_IMPLEMENT_FUN_4_END (Call3, call3_);


#undef FUNCTIONAL_RR_IMPLEMENT_FUN_4_BEGIN
#undef FUNCTIONAL_RR_IMPLEMENT_FUN_4_END



            BasicFunction* heightslang_ (const Setup &setup, BasicFunction *root, const ::std::string &code);

            class Function_R1_R1 : public BasicFunction {
                private:
                    BasicFunction *fun;
                public:
                    virtual ~Function_R1_R1() {
                        if (0 != fun) {
                            delete fun;
                        }
                    }
                    explicit Function_R1_R1 (BasicFunction * fun) : fun (fun) {
                    }
                    explicit Function_R1_R1 (const ::std::string &str) : fun (heightslang_ (Setup (1),this,str)) {
                    }

                    real_t operator () (real_t x) const {
                        const real_t parameters[1] = {x};
                        return (*fun) (parameters);
                    }

                    virtual real_t operator () (const real_t * const parameters) const {
                        return (*fun) (parameters);
                    }
            };



            class Function_R2_R1 : public BasicFunction {
                private:
                    BasicFunction *fun;
                public:
                    virtual ~Function_R2_R1() {
                        if (0 != fun) {
                            delete fun;
                        }
                    }
                    explicit Function_R2_R1 (BasicFunction * fun) : fun (fun) {
                    }
                    explicit Function_R2_R1 (const ::std::string &str) : fun (heightslang_ (Setup (2),this,str)) {
                    }

                    real_t operator () (real_t x, real_t y) const {
                        const real_t parameters[] = {x,y};
                        return (*fun) (parameters);
                    }

                    virtual real_t operator () (const real_t * const parameters) const {
                        return (*fun) (parameters);
                    }
            };



            class Function_R3_R1 : public BasicFunction {
                private:
                    BasicFunction *fun;
                public:
                    virtual ~Function_R3_R1() {
                        if (0 != fun) {
                            delete fun;
                        }
                    }
                    explicit Function_R3_R1 (BasicFunction * fun) : fun (fun) {
                    }
                    explicit Function_R3_R1 (const ::std::string &str) : fun (heightslang_ (Setup (3),this,str)) {
                    }

                    real_t operator () (real_t x, real_t y, real_t z) const {
                        const real_t parameters[] = {x,y,z};
                        return (*fun) (parameters);
                    }

                    virtual real_t operator () (const real_t * const parameters) const {
                        return (*fun) (parameters);
                    }
            };


        } // namespace functional {
    } // namespace misc {
} // namespace picogen


#undef FUNCTIONAL_BOOL_TO_REAL
#undef FUNCTIONAL_REAL_TO_BOOL

//#include "functional_ex.h"

#endif // _FUNCTIONAL_H

