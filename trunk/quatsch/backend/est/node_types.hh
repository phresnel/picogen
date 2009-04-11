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

#ifndef NODETYPES_H__INCLUDED__20090104
#define NODETYPES_H__INCLUDED__20090104

#include "../../quatsch.hh"
#include <cmath>


#include "nodes/rootfunction.hh"
#include "nodes/null.hh"
#include "nodes/constant.hh"
#include "nodes/parameter.hh"
#include "nodes/call.hh"
#include "nodes/accumulatingop.hh"
#include "nodes/boolaccu.hh"
#include "nodes/rangeop.hh"
#include "nodes/lerp.hh"



// Executable Syntax Tree :/
namespace quatsch { namespace backend { namespace est { 



template <typename BACKEND_T> struct node_types {

        enum { debug = 0 };


        typedef BACKEND_T backend_t;
        typedef typename backend_t::scalar_t scalar_t;
        typedef typename backend_t::parameters_t parameters_t;
        typedef typename backend_t::Function Function;
        typedef typename backend_t::FunctionPtr FunctionPtr;


        // Operators for use in AccumulatingOp.
        // TODO: some operators rely on definitions of scalar2bool(), pow(), 
        // etc., so if we ever want too sse these lines ...
        struct Operators {        
                struct add {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs + rhs;
                        }
                };
                
                
                struct sub {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs - rhs;
                        }
                };
                
                
                struct mul {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs * rhs;
                        }
                };
                
                
                struct div {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs / rhs;
                        }
                };
                
                
                struct pow {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs 
                        ) const {
                                return ::pow (lhs, rhs);
                        }
                };
                
                
                struct min {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs < rhs ? lhs : rhs;
                        }                        
                };
                
                
                struct max {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return rhs < lhs ? lhs : rhs;
                        }
                };


                struct and_ {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return util::bool2scalar<scalar_t> (
                                     util::scalar2bool(lhs) 
                                     bitand util::scalar2bool(rhs)
                                );
                        }
                };
                
                
                struct or_ {
                        scalar_t operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return util::bool2scalar<scalar_t> (
                                     util::scalar2bool (lhs) 
                                     bitor util::scalar2bool (rhs)
                                );
                        }
                };
        };
        
                

        struct Predicates {              
                struct less_than {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs < rhs;
                        }
                };
                
                
                struct greater_than {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs > rhs;
                        }
                };
                
                
                struct less_or_equal {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs <= rhs;
                        }
                };
                
                
                struct greater_or_equal {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs >= rhs;
                        }
                };
                
                
                struct equal {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs == rhs;
                        }
                };
                
                
                struct not_equal {
                        bool operator () (
                                const scalar_t &lhs, const scalar_t &rhs
                        ) const {
                                return lhs != rhs;
                        }
                };
        };

        struct Prefixes {
                struct verbatim {
                        scalar_t operator () (const scalar_t &r) const {
                                return r;
                        }
                };
        };


        typedef est::RootFunction<node_types>  RootFunction;
        typedef est::Null        <node_types>  Null;
        typedef est::Constant    <node_types>  Constant;
        typedef est::Parameter   <node_types>  Parameter;
        typedef est::Call        <node_types>  Call;

        struct unary {

                static void probe (const unsigned int operandCount) {
                        if (operandCount != 1) 
                                throw invalid_number_of_operands_exception (1);
                }

                struct Inv : public Function {
                        FunctionPtr operand;
                        Inv ();
                        Inv (const Inv &);
                        Inv (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Inv (operand));
                        }                        
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return 1.0 / ((*operand ) (parameters));
                        }
                };



                struct Sin : public Function {
                        FunctionPtr operand;
                        Sin ();
                        Sin (const Sin &);
                        Sin (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Sin (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return ::sin ((*operand ) (parameters));
                        }
                };



                struct Cos : public Function {
                        FunctionPtr operand;
                        Cos ();
                        Cos (const Cos &);
                        Cos (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Cos (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return ::cos ((*operand ) (parameters));
                        }
                };



                struct Floor : public Function {
                        FunctionPtr operand;
                        Floor ();
                        Floor (const Floor &);
                        Floor (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Floor (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return util::floor ((*operand ) (parameters));
                        }
                };



                struct Abs : public Function {
                        FunctionPtr operand;
                        Abs ();
                        Abs (const Abs &);
                        Abs (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Abs (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return ::fabs ((*operand ) (parameters));
                        }
                };



                struct Trunc : public Function {
                        FunctionPtr operand;
                        Trunc ();
                        Trunc (const Trunc &);
                        Trunc (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Trunc (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return static_cast<scalar_t> (
                                        static_cast<int> (
                                                (*operand ) (parameters)
                                        ));
                        }
                };



                struct Frac : public Function {
                        FunctionPtr operand;
                        Frac ();
                        Frac (const Frac &);
                        Frac (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Frac (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                const scalar_t tmp = (*operand ) (parameters);
                                return tmp - util::floor (tmp);
                        }
                };



                struct Neg : public Function {
                        FunctionPtr operand;
                        Neg ();
                        Neg (const Neg &);
                        Neg (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Neg (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return -(*operand ) (parameters);
                        }
                };



                struct Not : public Function {
                        FunctionPtr operand;
                        Not ();
                        Not (const Not &);
                        Not (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Not (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return util::bool2scalar<scalar_t> (
                                  !util::scalar2bool ((*operand ) (parameters))
                                );
                        }
                };



                struct Sqrt : public Function {
                        FunctionPtr operand;
                        Sqrt ();
                        Sqrt (const Sqrt &);
                        Sqrt (FunctionPtr operand) : operand (operand) {}
                public:
                        static FunctionPtr create (FunctionPtr operand) {
                                return FunctionPtr (new Sqrt (operand));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return ::sqrt ((*operand ) (parameters));
                        }
                };

        }; // struct unary




        struct binary {

                static void probe (const unsigned int operandCount) {
                        if (operandCount != 2) 
                                throw invalid_number_of_operands_exception (2);
                }

                struct Delta : public Function {
                        FunctionPtr lhs, rhs;
                        Delta ();
                        Delta (const Delta &);
                        
                        Delta (FunctionPtr lhs, FunctionPtr rhs)
                        : lhs (lhs), rhs (rhs)
                        {}
                        
                public:
                        static FunctionPtr create (
                                FunctionPtr lhs, FunctionPtr rhs
                        ) {
                                return FunctionPtr (new Delta (lhs, rhs));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                return((*lhs)(parameters))<((*rhs)(parameters))
                                        ? 0.0
                                        : 1.0;
                        }
                };


                struct Xor : public Function {
                        FunctionPtr lhs;
                        FunctionPtr rhs;
                        Xor ();
                        Xor (const Xor &);
                        
                        Xor (FunctionPtr lhs, FunctionPtr rhs)
                        : lhs (lhs), rhs (rhs)
                        {}
                
                public:
                        static FunctionPtr create (
                                FunctionPtr lhs, FunctionPtr rhs
                        ) {
                                return FunctionPtr (new Xor (lhs, rhs));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                using util::scalar2bool;
                                using util::bool2scalar;
                                const bool a = scalar2bool((*lhs)(parameters));
                                const bool b = scalar2bool((*rhs)(parameters));
                                return bool2scalar<scalar_t> (a != b);
                        }
                };

        }; // struct binary


        struct tertiary {

                static void probe (const unsigned int operandCount) {
                        if (operandCount != 3)
                                throw invalid_number_of_operands_exception (3);
                }

                // TODO: Write branchfree version for the case some magic
                //       heuristics consider that faster.
                
                struct IfThenElse : public Function {
                        FunctionPtr if_, then_, else_;
                        IfThenElse ();
                        IfThenElse (const IfThenElse &);
                        
                        IfThenElse (
                                FunctionPtr if_, 
                                FunctionPtr then_, 
                                FunctionPtr else_)
                        : if_ (if_), then_ (then_), else_ (else_)
                        {}

                public:
                        static FunctionPtr create (
                                FunctionPtr if_, 
                                FunctionPtr then_, 
                                FunctionPtr else_
                        ) {
                                return FunctionPtr (new IfThenElse (
                                        if_, then_, else_
                                ));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                if (util::scalar2bool ((*if_) (parameters))) {
                                        return (*then_) (parameters);
                                } else {
                                        return (*else_) (parameters);
                                }
                        }
                };
                
                

                // Transform (a b f) -> (x_0 x_1 ... x_n fac)
                struct Lerp : public Function {
                        FunctionPtr alpha, beta, factor;
                        Lerp ();
                        Lerp (const Lerp &);
                        
                        Lerp (
                                FunctionPtr alpha, 
                                FunctionPtr beta, 
                                FunctionPtr factor
                        )
                        : alpha (alpha)
                        , beta (beta)
                        , factor (factor)
                        {}
                        
                public:
                        static FunctionPtr create (
                                FunctionPtr alpha, 
                                FunctionPtr beta, 
                                FunctionPtr factor
                        ) {
                                return FunctionPtr (new Lerp (
                                        alpha, beta, factor
                                ));
                        }
                        scalar_t operator () (
                                const parameters_t &parameters
                        ) const {
                                const scalar_t factor =
                                                  (*this->factor) (parameters);
                                return (*alpha) (parameters) * (1.0 - factor) +
                                        (*beta) (parameters) * factor;
                        }
                };

        }; // struct binary


        // TODO: this function must be outsourced
        // TODO: remove redundancy
        static bool is_builtin (const ::std::string &operator_) {
                if ("+" == operator_) {
                        return true;
                } else if ("-" == operator_) {
                        return true;
                } else if ("*" == operator_) {
                        return true;
                } else if ("/" == operator_) {
                        return true;
                } else if ("^" == operator_) {
                        return true;
                } else if ("min" == operator_) {
                        return true;
                } else if ("max" == operator_) {
                        return true;
                } else if ("and" == operator_) {
                        return true;
                } else if ("or" == operator_) {
                        return true;
                } else if ("lerp" == operator_) {
                        return true;
                } else

                // predicates
                if ("<" == operator_) {
                        return true;
                } else if ("<=" == operator_) {
                        return true;
                } else if (">" == operator_) {
                        return true;
                } else if (">=" == operator_) {
                        return true;
                } else if ("=" == operator_) {
                        return true;
                } else if ("!=" == operator_ || "<>" == operator_) {
                        return true;
                } else

                // ranges
                if ("[]" == operator_) {
                        return true;
                } else if ("]]" == operator_) {
                        return true;
                } else if ("][" == operator_) {
                        return true;
                } else if ("[[" == operator_) {
                        return true;
                } else


                // unary
                if ("inv" == operator_) {
                        return true;
                } else if ("sin" == operator_) {
                        return true;
                } else if ("cos" == operator_) {
                        return true;
                } else if ("floor" == operator_) {
                        return true;
                } else if ("abs" == operator_) {
                        return true;
                } else if ("trunc" == operator_) {
                        return true;
                } else if ("frac" == operator_) {
                        return true;
                } else if ("neg" == operator_) {
                        return true;
                } else if ("not" == operator_) {
                        return true;
                } else if ("sqrt" == operator_) {
                        return true;
                } else

                // binary
                if ("delta" == operator_) {
                        return true;
                } else if ("xor" == operator_) {
                        return true;
                } else

                // tertiary
                if ("?" == operator_ || "if" == operator_) {
                        return true;
                } else if ("lerp" == operator_) {
                        return true;
                } else

                // misc
                {
                        return false;
                }
        }
        
        struct hidden {
                template <typename T>
                struct accu
                : AccumulatingOp <node_types, T, 2, 8>
                {
                        static FunctionPtr create (FunctionPtr operand) {
                                return AccumulatingOp <node_types, T, 2, 8>::
                                        create (operand);
                        }
                };
        };

        template <typename operand_vector_t>
        static FunctionPtr create (
                const ::std::string &operator_, 
                const unsigned int operandCount, 
                const operand_vector_t &operands_
        ) {

                const int max_fold = 16;
                if ("+" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::add, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("-" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::sub, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("*" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::mul, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("/" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::div, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("^" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::pow, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("min" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::min, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("max" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::max, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("and" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::and_, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("or" == operator_) {
                        return AccumulatingOp <node_types, 
                                typename Operators::or_, 2, max_fold
                        >::create (operandCount, operands_);
                } else 
                
                
                if ("lerp" == operator_) {
                        return Lerp <
                                node_types, 2, max_fold
                        >::create (operandCount, operands_);
                } else

                // predicates
                if ("<" == operator_) {
                        return BoolAccu <node_types, 
                                typename Predicates::less_than, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("<=" == operator_) {
                        return BoolAccu <node_types, 
                                typename Predicates::less_or_equal, 2, max_fold
                        >::create (operandCount, operands_);
                } else if (">" == operator_) {
                        return BoolAccu <node_types, 
                                typename Predicates::greater_than, 2, max_fold
                        >::create (operandCount, operands_);
                } else if (">=" == operator_) {
                        return BoolAccu <node_types, 
                             typename Predicates::greater_or_equal, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("=" == operator_) {
                        return BoolAccu <node_types, 
                                typename Predicates::equal, 2, max_fold
                        >::create (operandCount, operands_);
                } else if ("!=" == operator_ || "<>" == operator_) {
                        return BoolAccu <node_types, 
                                typename Predicates::not_equal, 2, max_fold
                        >::create (operandCount, operands_);
                } else

                // ranges
                if ("[]" == operator_) {
                        return RangeOp <
                                node_types,
                                typename Predicates::less_or_equal, 
                                typename Predicates::less_or_equal,
                                1, max_fold
                        >::create (operandCount, operands_);
                } else if ("]]" == operator_) {
                        return RangeOp <
                                node_types,
                                typename Predicates::less_than, 
                                typename Predicates::less_or_equal,
                                1, max_fold
                        >::create (operandCount, operands_);
                } else if ("][" == operator_) {
                        return RangeOp <
                                node_types,
                                typename Predicates::less_than, 
                                typename Predicates::less_than,
                                1, max_fold
                        >::create (operandCount, operands_);
                } else if ("[[" == operator_) {
                        return RangeOp <
                                node_types,
                                typename Predicates::less_or_equal, 
                                typename Predicates::less_than,
                                1, max_fold
                        >::create (operandCount, operands_);
                } else


                // unary
                if ("inv" == operator_) {
                        unary::probe (operandCount);
                        return unary::Inv::create (operands_ [0]);
                } else if ("sin" == operator_) {
                        unary::probe (operandCount);
                        return unary::Sin::create (operands_ [0]);
                } else if ("cos" == operator_) {
                        unary::probe (operandCount);
                        return unary::Cos::create (operands_ [0]);
                } else if ("floor" == operator_) {
                        unary::probe (operandCount);
                        return unary::Floor::create (operands_ [0]);
                } else if ("abs" == operator_) {
                        unary::probe (operandCount);
                        return unary::Abs::create (operands_ [0]);
                } else if ("trunc" == operator_) {
                        unary::probe (operandCount);
                        return unary::Trunc::create (operands_ [0]);
                } else if ("frac" == operator_) {
                        unary::probe (operandCount);
                        return unary::Frac::create (operands_ [0]);
                } else if ("neg" == operator_) {
                        unary::probe (operandCount);
                        return unary::Neg::create (operands_ [0]);
                } else if ("not" == operator_) {
                        unary::probe (operandCount);
                        return unary::Not::create (operands_ [0]);
                } else if ("sqrt" == operator_) {
                        unary::probe (operandCount);
                        return unary::Sqrt::create (operands_ [0]);
                } else

                // binary
                if ("delta" == operator_) {
                        binary::probe (operandCount);
                        return binary::Delta::create(
                                operands_[0],operands_[1]
                        );
                } else if ("xor" == operator_) {
                        binary::probe (operandCount);
                        return binary::Xor::create(
                                operands_[0],operands_[1]
                        );
                } else

                // tertiary
                if ("?" == operator_ || "if" == operator_) {
                        tertiary::probe (operandCount);
                        return tertiary::IfThenElse::create (
                                operands_ [0], 
                                operands_ [1], 
                                operands_ [2]
                        );
                } else /* if ("lerp" == operator_) {
                        tertiary::probe (operandCount);
                        return tertiary::Lerp::create (
                                operands_ [0], 
                                operands_ [1], 
                                operands_ [2]
                        );
                } else*/

                // misc
                {
                        throw unknown_operator_exception ();
                }
        }
};
}
}
}
#endif // NODETYPES_H__INCLUDED__20090104
