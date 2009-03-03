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

#ifndef ESTNODES_BOOLACCU_20090124
#define ESTNODES_BOOLACCU_20090124
namespace quatsch {  namespace backend {  namespace est {
    
    // I hereby request a better name. Thanks.

    template <typename TYPES, typename predicate_t, unsigned int numOps> 
    class BoolAccuN : public TYPES::Function, public predicate_t {
        
            enum { debug = 0 };

            typename TYPES::FunctionPtr operands [numOps];
            template <typename operands_vec_t> BoolAccuN (const operands_vec_t &operands) {
                for (unsigned int i=0; i<numOps; ++i) {
                    this->operands [i] = operands [i];
                }
                if (debug) {
                    ::std::cout << "Instantiated a BoolAccuN <" << numOps << ">" << ::std::endl;
                }
            }
            BoolAccuN ();
            BoolAccuN (BoolAccuN &);
        public:
            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (const operand_vector_t &operands_) {
                return typename TYPES::FunctionPtr (
                    new BoolAccuN <TYPES, predicate_t, numOps> (operands_));
            }                
            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                // TODO: implement a static assert for operandCount>=1
                bool result = true;
                for (unsigned int i=0; i<numOps-1; ++i) {
                    result = result && (predicate_t::operator () ((*operands [0+i]) (parameters), (*operands [1+i]) (parameters)));
                }
                return bool2scalar<typename TYPES::scalar_t> (result);
            }
    };
    
    
    
    // I hereby request a better name. Thanks.
    template <typename TYPES, class predicate_t, unsigned int min_operand_count=2, unsigned int max_fold=8> 
    class BoolAccu : public TYPES::Function, public predicate_t {
        
            enum { debug = 0 };
            
            const unsigned int operandCount;
            const ::boost::shared_array <typename TYPES::FunctionPtr> operands;
        
            BoolAccu (const unsigned int operandCount, const ::boost::shared_array <typename TYPES::FunctionPtr> operands)
            : operandCount (operandCount), operands (operands) {}
            BoolAccu ();
            BoolAccu (const BoolAccu &);

            template <typename operand_vector_t, unsigned int operand_count, bool do_recurse> struct rcreate ;
                
            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, true> {
                static typename TYPES::FunctionPtr create (const unsigned int operandCount, const operand_vector_t &operands_) {
                    if (operand_count == operandCount) {
                        return BoolAccuN <TYPES, predicate_t, operand_count>::create (operands_);
                    } else {
                        return rcreate <operand_vector_t, operand_count-1, (operand_count-1 > min_operand_count)> :: create (operandCount, operands_);
                    }
                }
            };

            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, false> {
                static typename TYPES::FunctionPtr create (const unsigned int /*operandCount*/, const operand_vector_t &operands_) {
                    return BoolAccuN <TYPES, predicate_t, min_operand_count>::create (operands_);
                }
            };
            
        public:
            
            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr 
            create (const unsigned int operandCount, const operand_vector_t &operands_) {
                
                if (min_operand_count > operandCount)
                    throw insufficient_number_of_operands_exception (min_operand_count);
                
                if (max_fold >= operandCount) {
                    return rcreate<operand_vector_t, max_fold, (min_operand_count < max_fold)>::
                        create (operandCount, operands_);
                } else {
                    // Convert from arbitray ::std::vector type to what AccumulatingOp needs.
                    ::boost::shared_array <typename TYPES::FunctionPtr> operands (
                        new typename TYPES::FunctionPtr [operandCount]
                    );
                    for (unsigned int i=0; i<operandCount; ++i) {
                        operands [i] = operands_ [i];
                    }
                    return typename TYPES::FunctionPtr (
                        new BoolAccu <TYPES, predicate_t, min_operand_count, max_fold> (operandCount, operands)
                    );
                }
            }

            virtual ~BoolAccu () {
            }

            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                // TODO: implement a static assert for operandCount>=1
                bool result = true;
                for (unsigned int i=0; i<operandCount-1; ++i) {
                    result = result && (predicate_t::operator () ((*operands [0+i]) (parameters), (*operands [1+i]) (parameters)));
                }
                return bool2scalar<typename TYPES::scalar_t> (result);
            }
    };
} } }
#endif // ESTNODES_BOOLACCU_20090124
