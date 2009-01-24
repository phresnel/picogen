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

#include "../../quatsch.h"
#include <cmath>


namespace {
    template <typename scalar_t> inline bool scalar2bool (const scalar_t &r) { return (r>0.5) ? true : false; }
    template <typename scalar_t> inline scalar_t bool2scalar (const bool &b) { return b ? 1.0 : 0.0; }
    template <typename scalar_t> inline scalar_t scalar2bool2scalar (const scalar_t &r) { return bool2scalar (scalar2bool (r)); }
    template <typename scalar_t> inline scalar_t floor (const scalar_t &v) {
        // TODO: replace with static assert
        assert (static_cast<int>(1.75) == 1);
        assert (static_cast<int>(1.5) == 1);
        assert (static_cast<int>(1.25) == 1);
        assert (static_cast<int>(-0.75) == 0);
        assert (static_cast<int>(-0.5) == 0);
        assert (static_cast<int>(-0.25) == 0);
        return static_cast <scalar_t> (static_cast <int> (v<0 ? v-1 : v));
    }
}

#include "nodes/rootfunction.h"
#include "nodes/null.h"
#include "nodes/constant.h"
#include "nodes/parameter.h"
#include "nodes/call.h"
#include "nodes/accumulatingop.h"
#include "nodes/boolaccu.h"
#include "nodes/rangeop.h"

namespace quatsch {
    namespace backend {
        namespace est { // Executable Syntax Tree
            
            template <typename BACKEND_T> struct node_types {
                
                enum { debug = 0 };
                
                typedef BACKEND_T backend_t;
                typedef typename backend_t::scalar_t scalar_t;
                typedef typename backend_t::parameters_t parameters_t;
                typedef typename backend_t::Function Function;
                typedef typename backend_t::FunctionPtr FunctionPtr;

                // Operators for use in AccumulatingOp.
                // TODO: some operators rely on definitions of scalar2bool(), pow(), etc., so if we ever want too sse these lines ...
                struct Operators {
                    struct add { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs + rhs; } };
                    struct sub { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs - rhs; } };
                    struct mul { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs * rhs; } };
                    struct div { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs / rhs; } };
                    struct pow { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return ::pow (lhs, rhs); } };
                    struct min { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs < rhs ? lhs : rhs; } };
                    struct max { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return rhs < lhs ? lhs : rhs; } };
                    
                    struct and_ { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return bool2scalar<scalar_t> (scalar2bool (lhs) bitand scalar2bool (rhs)); } };
                    struct or_ { scalar_t operator () (const scalar_t &lhs, const scalar_t &rhs) const { return bool2scalar<scalar_t> (scalar2bool (lhs) bitor scalar2bool (rhs)); } };
                };
                
                struct Predicates {
                    struct less_than { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs < rhs; } };
                    struct greater_than { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs > rhs; } };
                    struct less_or_equal { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs <= rhs; } };
                    struct greater_or_equal { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs >= rhs; } };
                    struct equal { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs == rhs; } };
                    struct not_equal { bool operator () (const scalar_t &lhs, const scalar_t &rhs) const { return lhs != rhs; } };
                };
                
                struct Prefixes {
                    struct verbatim { scalar_t operator () (const scalar_t &r) const { return r; } };
                    //struct scalar2bool2scalar { scalar_t operator () (const scalar_t &r) const { return scalar2bool2scalar (r); } };
                };
                

                typedef est::RootFunction<node_types> RootFunction;
                typedef est::Null<node_types> Null;
                typedef est::Constant<node_types> Constant;
                typedef est::Parameter<node_types> Parameter;
                typedef est::Call<node_types> Call;

                struct p1 {
                    
                    static void probe (const unsigned int operandCount) {
                        if (operandCount != 1) throw invalid_number_of_operands_exception (1);
                    }
                    
                    struct Inv : public Function {
                            FunctionPtr operand;
                            Inv (); 
                            Inv (const Inv &);
                            Inv (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Inv (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return 1.0 / ((*operand ) (parameters));
                            }
                    };
                    
                    
                    
                    struct Sin : public Function {
                            FunctionPtr operand;
                            Sin (); 
                            Sin (const Sin &);
                            Sin (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Sin (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return ::sin ((*operand ) (parameters));
                            }
                    };
                    
                    
                    
                    struct Cos : public Function {
                            FunctionPtr operand;
                            Cos (); 
                            Cos (const Cos &);
                            Cos (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Cos (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return ::cos ((*operand ) (parameters));
                            }
                    };

                    
                    
                    struct Floor : public Function {
                            FunctionPtr operand;
                            Floor (); 
                            Floor (const Floor &);
                            Floor (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Floor (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return floor ((*operand ) (parameters));
                            }
                    };
                    
                    
                    
                    struct Abs : public Function {
                            FunctionPtr operand;
                            Abs (); 
                            Abs (const Abs &);
                            Abs (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Abs (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return ::fabs ((*operand ) (parameters));
                            }
                    };
                    
                    
                    
                    struct Trunc : public Function {
                            FunctionPtr operand;
                            Trunc (); 
                            Trunc (const Trunc &);
                            Trunc (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Trunc (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return static_cast<scalar_t> (static_cast<int> ((*operand ) (parameters)));
                            }
                    };
                    
                    
                    
                    struct Frac : public Function {
                            FunctionPtr operand;
                            Frac (); 
                            Frac (const Frac &);
                            Frac (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Frac (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                const scalar_t tmp = (*operand ) (parameters);
                                return tmp - floor (tmp);
                            }
                    };
                    
                    
                    
                    struct Neg : public Function {
                            FunctionPtr operand;
                            Neg (); 
                            Neg (const Neg &);
                            Neg (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Neg (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return -(*operand ) (parameters);
                            }
                    };

                    
                    
                    struct Not : public Function {
                            FunctionPtr operand;
                            Not (); 
                            Not (const Not &);
                            Not (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Not (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return bool2scalar<scalar_t> (!scalar2bool ((*operand ) (parameters)));
                            }
                    };
                    
                    
                    
                    struct Sqrt : public Function {
                            FunctionPtr operand;
                            Sqrt (); 
                            Sqrt (const Sqrt &);
                            Sqrt (FunctionPtr operand) : operand (operand) {}
                        public:
                            static FunctionPtr create (FunctionPtr operand) { return FunctionPtr (new Sqrt (operand)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return ::sqrt ((*operand ) (parameters));
                            }
                    };
                   
                }; // namespace p1
                
                
                
                
                struct p2 {
                    
                    static void probe (const unsigned int operandCount) {
                        if (operandCount != 2) throw invalid_number_of_operands_exception (2);
                    }
                    
                    struct Delta : public Function {
                            FunctionPtr lhs, rhs;
                            Delta (); 
                            Delta (const Delta &);
                            Delta (FunctionPtr lhs, FunctionPtr rhs) : lhs (lhs), rhs (rhs) {}
                        public:
                            static FunctionPtr create (FunctionPtr lhs, FunctionPtr rhs) { return FunctionPtr (new Delta (lhs, rhs)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                return ((*lhs) (parameters)) < ((*rhs) (parameters)) ? 0.0 : 1.0;
                            }
                    };
                    
                    
                    struct Xor : public Function {
                            FunctionPtr lhs;
                            FunctionPtr rhs;
                            Xor (); 
                            Xor (const Xor &);
                            Xor (FunctionPtr lhs, FunctionPtr rhs) : lhs (lhs), rhs (rhs) {}
                        public:
                            static FunctionPtr create (FunctionPtr lhs, FunctionPtr rhs) { return FunctionPtr (new Xor (lhs, rhs)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                const bool a = scalar2bool ((*lhs) (parameters));
                                const bool b = scalar2bool ((*rhs) (parameters));
                                return bool2scalar<scalar_t> (a != b);
                            }
                    };
                    
                }; // namespace p2
                
                
                struct p3 {
                    
                    static void probe (const unsigned int operandCount) {
                        if (operandCount != 3) throw invalid_number_of_operands_exception (3);
                    }
                    
                    // TODO: Write branchfree version for the case heuristics consider it faster.
                    struct IfThenElse : public Function {
                            FunctionPtr if_, then_, else_;
                            IfThenElse (); 
                            IfThenElse (const IfThenElse &);
                            IfThenElse (FunctionPtr if_, FunctionPtr then_, FunctionPtr else_) : if_ (if_), then_ (then_), else_ (else_) {}
                        public:
                            static FunctionPtr create (FunctionPtr if_, FunctionPtr then_, FunctionPtr else_) { return FunctionPtr (new IfThenElse (if_, then_, else_)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                if (scalar2bool ((*if_) (parameters))) {
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
                            Lerp (FunctionPtr alpha, FunctionPtr beta, FunctionPtr factor) : alpha (alpha), beta (beta), factor (factor) {}
                        public:
                            static FunctionPtr create (FunctionPtr alpha, FunctionPtr beta, FunctionPtr factor) { return FunctionPtr (new Lerp (alpha, beta, factor)); }
                            scalar_t operator () (const parameters_t &parameters) const {
                                const scalar_t factor = (*this->factor) (parameters);
                                return (*alpha) (parameters) * (1.0 - factor) + (*beta) (parameters) * factor;
                            }
                    };
                    
                }; // namespace p2
                
                
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
                    
                    
                    // p1
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
                    
                    // p2
                    if ("delta" == operator_) {
                        return true;
                    } else if ("xor" == operator_) {
                        return true;
                    } else

                    // p3
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

                template <typename operand_vector_t>
                static FunctionPtr create (const ::std::string &operator_, const unsigned int operandCount, const operand_vector_t &operands_) {
                    const int max_fold = 8;
                    if ("+" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::add, 2, max_fold>::create (operandCount, operands_);
                    } else if ("-" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::sub, 2, max_fold>::create (operandCount, operands_);
                    } else if ("*" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::mul, 2, max_fold>::create (operandCount, operands_);
                    } else if ("/" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::div, 2, max_fold>::create (operandCount, operands_);
                    } else if ("^" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::pow, 2, max_fold>::create (operandCount, operands_);
                    } else if ("min" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::min, 2, max_fold>::create (operandCount, operands_);
                    } else if ("max" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::max, 2, max_fold>::create (operandCount, operands_);
                    } else if ("and" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::and_, 2, max_fold>::create (operandCount, operands_);
                    } else if ("or" == operator_) {
                        return AccumulatingOp <node_types, typename Operators::or_, 2, max_fold>::create (operandCount, operands_);
                    } else 
                    
                    // predicates
                    if ("<" == operator_) {
                        return BoolAccu <node_types, typename Predicates::less_than, 2, max_fold>::create (operandCount, operands_);
                    } else if ("<=" == operator_) {
                        return BoolAccu <node_types, typename Predicates::less_or_equal, 2, max_fold>::create (operandCount, operands_);
                    } else if (">" == operator_) {
                        return BoolAccu <node_types, typename Predicates::greater_than, 2, max_fold>::create (operandCount, operands_);
                    } else if (">=" == operator_) {
                        return BoolAccu <node_types, typename Predicates::greater_or_equal, 2, max_fold>::create (operandCount, operands_);
                    } else if ("=" == operator_) {
                        return BoolAccu <node_types, typename Predicates::equal, 2, max_fold>::create (operandCount, operands_);
                    } else if ("!=" == operator_ || "<>" == operator_) {
                        return BoolAccu <node_types, typename Predicates::not_equal, 2, max_fold>::create (operandCount, operands_);
                    } else 

                    // ranges
                    if ("[]" == operator_) {
                        return RangeOp <node_types, typename Predicates::less_or_equal, typename Predicates::less_or_equal, 1, max_fold>::create (operandCount, operands_);
                    } else if ("]]" == operator_) {
                        return RangeOp <node_types, typename Predicates::less_than, typename Predicates::less_or_equal, 1, max_fold>::create (operandCount, operands_);
                    } else if ("][" == operator_) {
                        return RangeOp <node_types, typename Predicates::less_than, typename Predicates::less_than, 1, max_fold>::create (operandCount, operands_);
                    } else if ("[[" == operator_) {
                        return RangeOp <node_types, typename Predicates::less_or_equal, typename Predicates::less_than, 1, max_fold>::create (operandCount, operands_);
                    } else
                    
                    
                    // p1
                    if ("inv" == operator_) {
                        p1::probe (operandCount);  return p1::Inv::create (operands_ [0]);
                    } else if ("sin" == operator_) {
                        p1::probe (operandCount);  return p1::Sin::create (operands_ [0]);
                    } else if ("cos" == operator_) {
                        p1::probe (operandCount);  return p1::Cos::create (operands_ [0]);
                    } else if ("floor" == operator_) {
                        p1::probe (operandCount);  return p1::Floor::create (operands_ [0]);
                    } else if ("abs" == operator_) {
                        p1::probe (operandCount);  return p1::Abs::create (operands_ [0]);
                    } else if ("trunc" == operator_) {
                        p1::probe (operandCount);  return p1::Trunc::create (operands_ [0]);
                    } else if ("frac" == operator_) {
                        p1::probe (operandCount);  return p1::Frac::create (operands_ [0]);
                    } else if ("neg" == operator_) {
                        p1::probe (operandCount);  return p1::Neg::create (operands_ [0]);
                    } else if ("not" == operator_) {
                        p1::probe (operandCount);  return p1::Not::create (operands_ [0]);
                    } else if ("sqrt" == operator_) {
                        p1::probe (operandCount);  return p1::Sqrt::create (operands_ [0]);
                    } else
                    
                    // p2
                    if ("delta" == operator_) {
                        p2::probe (operandCount);  return p2::Delta::create (operands_ [0], operands_ [1]);
                    } else if ("xor" == operator_) {
                        p2::probe (operandCount);  return p2::Xor::create (operands_ [0], operands_ [1]);
                    } else

                    // p3
                    if ("?" == operator_ || "if" == operator_) {
                        p3::probe (operandCount);  return p3::IfThenElse::create (operands_ [0], operands_ [1], operands_ [2]);
                    } else if ("lerp" == operator_) {
                        p3::probe (operandCount);  return p3::Lerp::create (operands_ [0], operands_ [1], operands_ [2]);
                    } else 

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
