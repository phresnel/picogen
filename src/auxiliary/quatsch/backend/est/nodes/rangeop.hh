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

#ifndef ESTNODES_RANGEOP_20090124
#define ESTNODES_RANGEOP_20090124
namespace quatsch {  namespace backend {  namespace est {

    // I hereby request a better name. Thanks.

    template <typename TYPES, typename min_predicate_t, typename max_predicate_t, unsigned int numOps>
    class RangeOpN : public TYPES::Function {

            enum { debug = 0 };

            const typename TYPES::FunctionPtr funMin;
            const typename TYPES::FunctionPtr funMax;
            typename TYPES::FunctionPtr operands [numOps];

            // We don't derive from the following, because then we would have to write a class
            //  template specialisation for the case that min_predicate_t==max_predicate_t.
            min_predicate_t minPred;
            max_predicate_t maxPred;

            template <typename operands_vec_t> RangeOpN (
                const typename TYPES::FunctionPtr &funMin,
                const typename TYPES::FunctionPtr &funMax,
                const operands_vec_t &operands
            ) : funMin (funMin), funMax (funMax)
            {
                for (unsigned int i=0; i<numOps; ++i) {
                    this->operands [i] = operands [i];
                }
                if (debug) {
                    ::std::cout << "Instantiated a RangeOpN <" << numOps << ">" << ::std::endl;
                }
            }
            RangeOpN ();
            RangeOpN (RangeOpN &);

        public:

            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (
                const typename TYPES::FunctionPtr &funMin,
                const typename TYPES::FunctionPtr &funMax,
                const operand_vector_t &operands_
            ) {
                return typename TYPES::FunctionPtr (
                    new RangeOpN <TYPES, min_predicate_t, max_predicate_t, numOps> (funMin, funMax, operands_));
            }

            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                // TODO: implement a static assert for operandCount>=1
                bool result = true;
                const typename TYPES::scalar_t min = (*funMin) (parameters);
                const typename TYPES::scalar_t max = (*funMax) (parameters);
                for (unsigned int i=0; i<numOps; ++i) {
                    const typename TYPES::scalar_t val = (*operands [i]) (parameters);
                    //std::cout << "val=" << val << ", min=" << min << "==" << minPred (min, val) << ", max=" << max << "==" << maxPred (val, max) << ::std::endl;
                    result = result && minPred (min, val);
                    result = result && maxPred (val, max);
                }
                return util::bool2scalar<typename TYPES::scalar_t> (result);
            }
    };



    template <typename TYPES, typename min_predicate_t, typename max_predicate_t, unsigned int min_operand_count=1, unsigned int max_fold=8>
    class RangeOp : public TYPES::Function {

            enum { debug = 0 };

            const unsigned int operandCount;
            const typename TYPES::FunctionPtr funMin;
            const typename TYPES::FunctionPtr funMax;
            const ::boost::shared_array <typename TYPES::FunctionPtr> operands;

            // We don't derive from the following, because then we would have to write a class
            //  template specialisation for the case that min_predicate_t==max_predicate_t.
            min_predicate_t minPred;
            max_predicate_t maxPred;

            RangeOp (
                const unsigned int operandCount,
                const typename TYPES::FunctionPtr funMin,
                const typename TYPES::FunctionPtr funMax,
                const ::boost::shared_array <typename TYPES::FunctionPtr> operands
            ) : operandCount (operandCount), funMin (funMin), funMax(funMax), operands (operands)
            { }

            RangeOp ();
            RangeOp (const RangeOp &);

            template <typename operand_vector_t, unsigned int operand_count, bool do_recurse> struct rcreate ;

            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, true> {
                static typename TYPES::FunctionPtr create (
                    const unsigned int operandCount,
                    const typename TYPES::FunctionPtr &funMin,
                    const typename TYPES::FunctionPtr &funMax,
                    const operand_vector_t &operands_
                ) {
                    if (operand_count == operandCount) {
                        return RangeOpN<TYPES, min_predicate_t, max_predicate_t, operand_count>::
                            create (funMin, funMax, operands_);
                    } else {
                        return rcreate<operand_vector_t, operand_count-1, (operand_count-1 > min_operand_count)>::
                            create (operandCount, funMin, funMax, operands_);
                    }
                }
            };

            template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, false> {
                static typename TYPES::FunctionPtr create (
                    const unsigned int /*operandCount*/,
                    const typename TYPES::FunctionPtr &funMin,
                    const typename TYPES::FunctionPtr &funMax,
                    const operand_vector_t &operands_
                ) {
                    return RangeOpN<TYPES, min_predicate_t, max_predicate_t, min_operand_count>::
                        create (funMin, funMax, operands_);
                }
            };

        public:

            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (
                const unsigned int operandCount,
                const typename TYPES::FunctionPtr &funMin,
                const typename TYPES::FunctionPtr &funMax,
                const operand_vector_t &operands_
            ) {
                if (min_operand_count > operandCount)
                    throw insufficient_number_of_operands_exception (min_operand_count+2);

                if (max_fold >= operandCount) {
                    return rcreate<operand_vector_t, max_fold, (min_operand_count < max_fold)>::
                        create (operandCount, funMin, funMax, operands_);
                } else {
                    // Convert from arbitray ::std::vector type to what AccumulatingOp needs.
                    ::boost::shared_array <typename TYPES::FunctionPtr> operands (
                        new typename TYPES::FunctionPtr [operandCount]
                    );
                    for (unsigned int i=0; i<operandCount; ++i) {
                        operands [i] = operands_ [i];
                    }
                    return typename TYPES::FunctionPtr (
                        new RangeOp
                            <TYPES, min_predicate_t, max_predicate_t, min_operand_count, max_fold>
                            (operandCount, funMin, funMax, operands));
                }
            }

            template <typename operand_vector_t>
            static typename TYPES::FunctionPtr create (
                const unsigned int operandCount_,
                const operand_vector_t &operands_
            ) {
                const unsigned int operandCount = operandCount_ - 2;

                if (min_operand_count > operandCount)
                    throw insufficient_number_of_operands_exception (min_operand_count+2);

                // TODO: make this independent of operand_vector_t
                operand_vector_t copy (operands_);

                const typename TYPES::FunctionPtr funMin = copy [0];
                const typename TYPES::FunctionPtr funMax = copy [1];

                copy.erase (copy.begin());
                copy.erase (copy.begin());

                if (max_fold >= operandCount) {
                    return rcreate<operand_vector_t, max_fold, (min_operand_count < max_fold)>::
                        create (operandCount, funMin, funMax, copy);
                } else {
                    // Convert from arbitray ::std::vector type to what AccumulatingOp needs.
                    ::boost::shared_array <typename TYPES::FunctionPtr> operands (
                        new typename TYPES::FunctionPtr [operandCount]
                    );

                    for (unsigned int i=0; i<operandCount; ++i) {
                        operands [i] = copy [i];
                    }

                    return typename TYPES::FunctionPtr(
                        new RangeOp
                            <TYPES, min_predicate_t, max_predicate_t, min_operand_count, max_fold>
                            (operandCount, funMin, funMax, operands));
                }
            }

            virtual ~RangeOp () {
            }

            typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                // TODO: implement a static assert for operandCount>=1
                bool result = true;
                const typename TYPES::scalar_t min = (*funMin) (parameters);
                const typename TYPES::scalar_t max = (*funMax) (parameters);
                for (unsigned int i=0; i<operandCount; ++i) {
                    const typename TYPES::scalar_t val = (*operands [i]) (parameters);
                    result = result && minPred(min, val);
                    result = result && maxPred(val, max);
                }
                return util::bool2scalar<typename TYPES::scalar_t> (result);
            }
    };
} } }
#endif // ESTNODES_RANGEOP_20090124
