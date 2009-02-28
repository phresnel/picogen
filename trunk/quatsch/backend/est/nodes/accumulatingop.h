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

#ifndef ESTNODES_ACCUMULATINGOP_20090124
#define ESTNODES_ACCUMULATINGOP_20090124
namespace quatsch {  namespace backend {  namespace est {
    // I hereby request a better name. Thanks.
    
    template <typename TYPES, typename op_t, unsigned int numOps, class prefix_t>
    class AccumulatingOpN : public TYPES::Function, public op_t, public prefix_t {
        
            enum { debug = 0 };
            
            typename TYPES::FunctionPtr operands [numOps];
        
            template <typename operands_vec_t> AccumulatingOpN (const operands_vec_t &operands) {
                for (unsigned int i=0; i<numOps; ++i) {
                    this->operands [i] = operands [i];
                }
                if (debug) {
                    ::std::cout << "Instantiated a AccumulatingOpN <" << numOps << ">" << ::std::endl;
                }
            }
            
            AccumulatingOpN ();
            AccumulatingOpN (AccumulatingOpN &);
            
        public:
            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (const operand_vector_t &operands_) {
                return typename TYPES::FunctionPtr (
                    new AccumulatingOpN <TYPES, op_t, numOps, prefix_t> (operands_)
                );
            }                
            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) 
            const {
                // TODO: implement a static assert for operandCount>=1
                typename TYPES::scalar_t sum = prefix_t::operator () ((*operands [0]) (parameters));
                for (unsigned int i=1; i<numOps; ++i) {
                    sum = prefix_t::operator () (op_t::operator () (sum, prefix_t::operator () ((*operands [i]) (parameters))));
                }
                return sum;
            }
    };
    
    
    
    // I hereby request a better name. Thanks.
    template <typename TYPES, typename op_t, unsigned int min_operand_count=2, unsigned int max_fold=8, class prefix_t=typename TYPES::Prefixes::verbatim>
    class AccumulatingOp : public TYPES::Function, public op_t, public prefix_t {
        
            enum { debug = 0 };
            
            const unsigned int operandCount;
            const ::boost::shared_array <typename TYPES::FunctionPtr> operands;
        
            AccumulatingOp (const unsigned int operandCount, 
                const ::boost::shared_array <typename TYPES::FunctionPtr> operands)
            : operandCount (operandCount), operands (operands)
            { }

            AccumulatingOp ();
            AccumulatingOp (AccumulatingOp &);

            template <typename operand_vector_t, unsigned int operand_count, bool do_recurse> struct rcreate ;
                
            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, true> {
                static typename TYPES::FunctionPtr create (const unsigned int operandCount, const operand_vector_t &operands_) {
                    if (operand_count == operandCount) {
                        return AccumulatingOpN <TYPES, op_t, operand_count, prefix_t>::create (operands_);
                    } else {
                        return rcreate <operand_vector_t, operand_count-1, (operand_count-1 > min_operand_count)> :: create (operandCount, operands_);
                    }
                }
            };
            
            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, false> {
                static typename TYPES::FunctionPtr create (const unsigned int /*operandCount*/, const operand_vector_t &operands_) {
                    return AccumulatingOpN<TYPES, op_t, min_operand_count, prefix_t>::create (operands_);
                }
            };
            
        public:
            
            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (const unsigned int operandCount, 
                const operand_vector_t &operands_
            ) {
                if (min_operand_count > operandCount) throw insufficient_number_of_operands_exception (min_operand_count);
                
                if (max_fold >= operandCount) {
                    return rcreate <operand_vector_t, max_fold, (min_operand_count < max_fold)> :: create (operandCount, operands_);
                } else {
                    // Convert from arbitrary ::std::vector type to what AccumulatingOp needs.
                    ::boost::shared_array <typename TYPES::FunctionPtr> operands (
                        new typename TYPES::FunctionPtr [operandCount]
                    );
                    for (unsigned int i=0; i<operandCount; ++i) {
                        operands [i] = operands_ [i];
                    }
                    return typename TYPES::FunctionPtr (
                        new AccumulatingOp <TYPES, op_t, min_operand_count, max_fold, prefix_t> (operandCount, operands)
                    );
                }
            }

            virtual ~AccumulatingOp () {
            }

            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                // TODO: implement a static assert for operandCount>=1
                typename TYPES::scalar_t sum = prefix_t::operator () ((*operands [0]) (parameters));
                for (unsigned int i=1; i<operandCount; ++i) {
                    sum = prefix_t::operator () (op_t::operator () (sum, prefix_t::operator () ((*operands [i]) (parameters))));
                }
                return sum;
            }
    };

} } }
#endif // ESTNODES_ACCUMULATINGOP_20090124
