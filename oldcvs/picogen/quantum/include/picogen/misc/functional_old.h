/***************************************************************************
 *            functional.h
 *
 *  Sat May 17 12:21:00 2008
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

// !!! NOTE
// this file has been tombstoned as soon as i began writing classes for 2d-domains,
// where it became obvious that operator () ( real, ... ) is shit

#if 0

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


namespace functional{
    typedef picogen::misc::prim::real real;

    namespace RR{

        class BasicFunction;
        class Function;
        class Constant;
        class Parameter;
        class Add;
        class Sub;
        class Mul;
        class Div;
        class Inv;
        class Floor;
        class Trunc;
        class Frac;
        class Neg;
        class Pow;
        class Sqrt;

        class Less;
        class LessEqual;
        class Greater;
        class GreaterEqual;
        class Equal;
        class NotEqual;

        class And;
        class Or;
        class Not;

        class IfElse;
        class Call;

        class Visitor{
            public:
                /*virtual void visit( BasicFunction* ) = 0;
                virtual void visit( Function* ) = 0;
                virtual void visit( Constant* ) = 0;
                virtual void visit( Parameter* ) = 0;
                virtual void visit( Add* ) = 0;
                virtual void visit( Sub* ) = 0;
                virtual void visit( Mul* ) = 0;
                virtual void visit( Div* ) = 0;
                virtual void visit( Inv* ) = 0;
                virtual void visit( Floor* ) = 0;
                virtual void visit( Trunc* ) = 0;
                virtual void visit( Frac* ) = 0;
                virtual void visit( Neg* ) = 0;
                virtual void visit( Pow* ) = 0;
                virtual void visit( Sqrt* ) = 0;

                virtual void visit( And* ) = 0;
                virtual void visit( Or* ) = 0;
                virtual void visit( Not* ) = 0;

                virtual void visit( IfElse* ) = 0;
                virtual void visit( Call* ) = 0;

                virtual void visit( Less* ) = 0;
                virtual void visit( LessEqual* ) = 0;
                virtual void visit( Greater* ) = 0;
                virtual void visit( GreaterEqual* ) = 0;
                virtual void visit( Equal* ) = 0;
                virtual void visit( NotEqual* ) = 0;

                virtual void visit_end( BasicFunction* ) = 0;
                virtual void visit_end( Function* ) = 0;
                virtual void visit_end( Constant* ) = 0;
                virtual void visit_end( Parameter* ) = 0;
                virtual void visit_end( Add* ) = 0;
                virtual void visit_end( Sub* ) = 0;
                virtual void visit_end( Mul* ) = 0;
                virtual void visit_end( Div* ) = 0;
                virtual void visit_end( Inv* ) = 0;
                virtual void visit_end( Floor* ) = 0;
                virtual void visit_end( Trunc* ) = 0;
                virtual void visit_end( Frac* ) = 0;
                virtual void visit_end( Neg* ) = 0;
                virtual void visit_end( Pow* ) = 0;
                virtual void visit_end( Sqrt* ) = 0;

                virtual void visit_end( And* ) = 0;
                virtual void visit_end( Or* ) = 0;
                virtual void visit_end( Not* ) = 0;

                virtual void visit_end( IfElse* ) = 0;
                virtual void visit_end( Call* ) = 0;

                virtual void visit_end( Less* ) = 0;
                virtual void visit_end( LessEqual* ) = 0;
                virtual void visit_end( Greater* ) = 0;
                virtual void visit_end( GreaterEqual* ) = 0;
                virtual void visit_end( Equal* ) = 0;
                virtual void visit_end( NotEqual* ) = 0;*/
        };


        class BasicFunction{
            public:
                virtual real operator () () const = 0;
                virtual void accept( Visitor * visitor ) = 0;
        };

        class Function : public BasicFunction {
                BasicFunction * fun;
                real x;
            public:
                Function( BasicFunction *fun ) : fun(fun) {}
                real &x_() { return x; }
                virtual real operator () ( real x ) const{
                    return (*fun)(x);
                }
                virtual void accept( Visitor * visitor ) {
                    visitor->visit( this );
                    fun->accept( visitor );
                    visitor->visit_end( this );
                }
        };

        class Constant;
        inline Constant* constant_( real value );
        class Constant : public BasicFunction {
                real value;
            protected:
                friend Constant* constant_( real value );
                explicit Constant( real value ) : value(value) {}
            public:
                virtual real operator () ( real x ) const {
                    return value;
                }
                virtual void accept( Visitor * visitor ) {
                    visitor->visit( this );
                    visitor->visit_end( this );
                }
        };
        inline Constant* constant_( real value ){
            return new Constant(value);
        }

        /*
        class Parameter;
        inline Parameter* x_();
        class Parameter : public BasicFunction {
            protected:
                friend Parameter* x_();
                explicit Parameter() {}
            public:
                virtual real operator () ( real x ) const {
                    return x;
                }
                virtual void accept( Visitor * visitor ) {
                    visitor->visit( this );
                    visitor->visit_end( this );
                }
        };
        inline Parameter* x_(){
            return new Parameter();
        }
        */
        class Parameter;
        //inline Parameter* parameter_( real &x );
        class Parameter : public BasicFunction {
            protected:
                friend Parameter* parameter_( real &param );
                real &param;
                explicit Parameter( real &param ) : param(param) {}
            public:
                virtual real operator () ( real x ) const {
                    return param;
                }
                virtual void accept( Visitor * visitor ) {
                    visitor->visit( this );
                    visitor->visit_end( this );
                }
        };
        inline Parameter* parameter_( real &param ){
            return new Parameter( param );
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
                    virtual real operator () ( real x ) const{                                  \
                        return WRAP( (*lhs)( x ) OP (*rhs)( x ) );                              \
                    }                                                                           \
                    virtual void accept( Visitor * visitor ) {                                  \
                        visitor->visit( this );                                                 \
                        lhs->accept( visitor );                                                 \
                        rhs->accept( visitor );                                                 \
                        visitor->visit_end( this );                                             \
                    }                                                                           \
            };                                                                                  \
            inline BasicFunction* ALLOCNAME( BasicFunction *lhs, BasicFunction *rhs ){          \
                return new CLASSNAME( lhs, rhs );                                               \
            }
        #else
        #error "FUNCTIONAL_RR_IMPLEMENT_BINOP has already been defined somewhere else"
        #endif


        FUNCTIONAL_RR_IMPLEMENT_BINOP( Add, add_, +, );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Sub, sub_, -, );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Mul, mul_, *, );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Div, div_, /, );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Less,         less_,          <,  FUNCTIONAL_BOOL_TO_REAL );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( LessEqual,    less_equal_,    <=, FUNCTIONAL_BOOL_TO_REAL );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Greater,      greater_,       >,  FUNCTIONAL_BOOL_TO_REAL );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( GreaterEqual, greater_equal_, >,  FUNCTIONAL_BOOL_TO_REAL );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( Equal,        equal_,         ==, FUNCTIONAL_BOOL_TO_REAL );
        FUNCTIONAL_RR_IMPLEMENT_BINOP( NotEqual,     not_equal_,     !=, FUNCTIONAL_BOOL_TO_REAL );

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
                    virtual void accept( Visitor * visitor ) {                                  \
                        visitor->visit( this );                                                 \
                        p1->accept( visitor );                                                  \
                        visitor->visit_end( this );                                                 \
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


        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Inv, inv_ )
            public:
                virtual real operator () ( real x ) const{
                    return 1.0 / (*p1)( x );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Inv, inv_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Floor, floor_ )
            public:
                virtual real operator () ( real x ) const{
                    return ::floor( (*p1)( x ) );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Floor, floor_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Trunc, trunc_ )
            public:
                virtual real operator () ( real x ) const{
                    return static_cast<real>(static_cast<int>( (*p1)( x ) ) );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Trunc, trunc_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Frac, frac_ )
            public:
                virtual real operator () ( real x ) const{
                    real tmp = (*p1)( x );
                    return tmp - ::floor( tmp );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Frac, frac_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Neg, neg_ )
            public:
                virtual real operator () ( real x ) const{
                    return -(*p1)( x );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Neg, neg_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Not, not_ )
            public:
                virtual real operator () ( real x ) const{
                    const bool a = !FUNCTIONAL_REAL_TO_BOOL((*p1)( x ));
                    return FUNCTIONAL_BOOL_TO_REAL(a);
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Not, not_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_1_BEGIN( Sqrt, sqrt_ )
            public:
                virtual real operator () ( real x ) const{
                    return ::sqrt((*p1)( x ));
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_1_END( Sqrt, sqrt_ );

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
                    virtual void accept( Visitor * visitor ) {                                  \
                        visitor->visit( this );                                                 \
                        /*if( dynamic_cast<Call*>(this) ){                                        */\
                        if( (void*)(Call*)(this) == (void*)this ){                                            \
                        }else{                                                                  \
                            p1->accept( visitor );                                              \
                        }                                                                       \
                        p2->accept( visitor );                                                  \
                        visitor->visit_end( this );                                             \
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

        FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN( Pow, pow_ )
            public:
                virtual real operator () ( real x ) const{
                    return ::pow( (*p1)( x ), (*p2)( x ) );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_2_END( Pow, pow_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN( And, and_ )
            public:
                virtual real operator () ( real x ) const{
                    const bool a = FUNCTIONAL_REAL_TO_BOOL((*p1)( x ));
                    const bool b = a && FUNCTIONAL_REAL_TO_BOOL((*p2)( x ));
                    return  FUNCTIONAL_BOOL_TO_REAL(b);
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_2_END( And, and_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN( Or, or_ )
            public:
                virtual real operator () ( real x ) const{
                    const bool a = FUNCTIONAL_REAL_TO_BOOL((*p1)( x ));
                    const bool b = a || FUNCTIONAL_REAL_TO_BOOL((*p2)( x ));
                    return  FUNCTIONAL_BOOL_TO_REAL(b);
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_2_END( Or, or_ );

        FUNCTIONAL_RR_IMPLEMENT_FUN_2_BEGIN( Call, call_ )
            public:
                virtual real operator () ( real x ) const{
                    return (*p1)( (*p2)( x ) );
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_2_END( Call, call_ );


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
                    virtual void accept( Visitor * visitor ) {                                  \
                        visitor->visit( this );                                                 \
                        p1->accept( visitor );                                                  \
                        p2->accept( visitor );                                                  \
                        p3->accept( visitor );                                                  \
                        visitor->visit_end( this );                                             \
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


        FUNCTIONAL_RR_IMPLEMENT_FUN_3_BEGIN( IfElse, if_else_ )
            public:
                virtual real operator () ( real x ) const{
                    return FUNCTIONAL_REAL_TO_BOOL( (*p1)( x ) )
                        ? (*p2)( x )
                        : (*p3)( x )
                    ;
                }
        FUNCTIONAL_RR_IMPLEMENT_FUN_3_END( IfElse, if_else_ );

        //BasicFunction *optimize( BasicFunction *f );
    };

    namespace R2R {

        class BasicFunction {
            public:
                virtual real operator () ( real x, real y ) const = 0;
        };

        class Function {
                RR::BasicFunction * fun;
                real x, y;
            public:
                real &x_() { return x; }
                real &y_() { return y; }
                explicit Function( RR::BasicFunction *fun ) : fun(fun) {
                }
                virtual real operator () ( real x, real y ) const {
                    // TODO: how about reentrance?
                    const_cast<real&>(this->x) = x;
                    const_cast<real&>(this->y) = y;
                    return (*fun)( 0.0 );
                }
        };

    };

    /*namespace R2R {
        class BasicFunction{
            public:
                virtual real operator () ( real x, real y ) const = 0;
        };

        class Function : public BasicFunction {
                BasicFunction * fun;

                class Param : public RR::BasicFunction{
                    public:
                        real val;
                        virtual real operator () ( real x ) const{
                            return val;
                        }
                        virtual void accept( RR::Visitor * visitor ){
                        }
                };
            public:

                //const RR::BasicFunction &x_() const { return paramX; }
                //const RR::BasicFunction &y_() const { return paramY; }
                Param paramX, paramY;

                Function( BasicFunction *fun ) : fun(fun) {}
                virtual real operator () ( real x, real y ) const{
                    const_cast<real&>(paramX.val) = x;
                    const_cast<real&>(paramY.val) = y;
                    return (*fun)(x,y);
                }
        };

        class ParameterX;
        class ParameterX : public BasicFunction {
                RR::BasicFunction * p1;
            protected:
                friend ParameterX* insert_x_( RR::BasicFunction * p1 );
                explicit ParameterX( RR::BasicFunction * p1 ) : p1(p1) {}
            public:
                virtual real operator () ( real x, real y ) const {
                    return (*p1)( x );
                }
        };
        inline ParameterX* insert_x_( RR::BasicFunction * p1 ){
            return new ParameterX( p1 );
        }

        class ParameterY;
        class ParameterY : public BasicFunction {
                RR::BasicFunction * p1;
            protected:
                friend ParameterY* insert_y_( RR::BasicFunction * p1 );
                explicit ParameterY( RR::BasicFunction * p1 ) : p1(p1) {}
            public:
                virtual real operator () ( real x, real y ) const {
                    return (*p1)( y );
                }
        };
        inline ParameterY* insert_y_( RR::BasicFunction * p1 ){
            return new ParameterY( p1 );
        }

    };*/

};


#undef FUNCTIONAL_BOOL_TO_REAL
#undef FUNCTIONAL_REAL_TO_BOOL


#endif // _FUNCTIONAL_H
#endif // #if 0