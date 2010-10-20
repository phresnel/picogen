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

namespace quatsch { namespace backend { namespace est {



///////////////////////////////////////////////////////////////////////////////
// AccumulatingOpN<> : specialized on specific operand count
///////////////////////////////////////////////////////////////////////////////
template <typename TYPES, typename OP_T, unsigned int numOps, class PREFIX_T>
class AccumulatingOpN : public TYPES::Function, public OP_T, public PREFIX_T {

        enum { debug = 0 };

        typename TYPES::FunctionPtr operands [numOps];



        template <typename operands_vec_t>
        AccumulatingOpN (
                const operands_vec_t &operands
        ) {
                for (unsigned int i=0; i<numOps; ++i) {
                        this->operands [i] = operands [i];
                }
                if (debug) {
                        ::std::cout << "Instantiated a AccumulatingOpN <" 
                                    << numOps << ">" 
                                    << ::std::endl;
                }
        }
        
        

        AccumulatingOpN ();
        AccumulatingOpN (AccumulatingOpN &);

public:



        template <typename OPERAND_VECTOR_T>
        static typename TYPES::FunctionPtr
        create (
                const OPERAND_VECTOR_T &operands_
        ) {
                return typename TYPES::FunctionPtr (
                               new AccumulatingOpN <TYPES, 
                                                    OP_T,
                                                    numOps,
                                                    PREFIX_T> (operands_)
                       );
        }
        
        
        
        typename TYPES::scalar_t 
        operator () (
                const typename TYPES::parameters_t &parameters
        ) const {
                // TODO: implement a static assert for operandCount>=1
                typename TYPES::scalar_t sum = PREFIX_T::operator () (
                        (*operands [0]) (parameters)
                );
                
                for (unsigned int i=1; i<numOps; ++i) {
                        sum = PREFIX_T::operator () (
                              OP_T::operator () (
                                  sum, PREFIX_T::operator () (
                                      (*operands [i]) (parameters)
                                  )));
                }
                return sum;
        }
        
        
        
};






///////////////////////////////////////////////////////////////////////////////
// AccumulatingOpN<> : partially specialized on operand count of 1
///////////////////////////////////////////////////////////////////////////////
template <typename TYPES, typename OP_T, class PREFIX_T>
class AccumulatingOpN<
        TYPES, OP_T, 1, PREFIX_T
>
: public TYPES::Function
, public OP_T
, public PREFIX_T {

        enum { debug = 0 };

        typename TYPES::FunctionPtr operand;



        template <typename operands_vec_t>
        AccumulatingOpN (
                const operands_vec_t &operands
        ) {
                this->operand = operands [0];
                if (debug) {
                        ::std::cout << "Instantiated a AccumulatingOpN <1>" 
                                    << ::std::endl;
                }
        }
        
        

        AccumulatingOpN ();
        AccumulatingOpN (AccumulatingOpN &);

public:



        template <typename OPERAND_VECTOR_T>
        static typename TYPES::FunctionPtr
        create (
                const OPERAND_VECTOR_T &operands_
        ) {
                return typename TYPES::FunctionPtr (
                               new AccumulatingOpN <TYPES, 
                                                    OP_T,
                                                    1,
                                                    PREFIX_T> (operands_)
                       );
        }
        
        
        
        typename TYPES::scalar_t 
        operator () (
                const typename TYPES::parameters_t &parameters
        ) const {
                return PREFIX_T::operator () (
                        (*operand) (parameters)
                );
        }
        
        
        
};






///////////////////////////////////////////////////////////////////////////////
// AccumulatingOp : the general form
///////////////////////////////////////////////////////////////////////////////
template <
        typename TYPES, 
        typename OP_T,
        unsigned int MIN_OPERAND_COUNT=2,
        unsigned int MAX_FOLD=8,
        class PREFIX_T=typename TYPES::Prefixes::verbatim
>
class AccumulatingOp
: public TYPES::Function
, public OP_T
, public PREFIX_T {

        enum { debug = 0 };

        const unsigned int operandCount;
        const ::boost::shared_array <typename TYPES::FunctionPtr> operands;
        
        

        AccumulatingOp (
             const unsigned int operandCount,
             const ::boost::shared_array <typename TYPES::FunctionPtr> operands
        )
        : operandCount (operandCount), operands (operands) { }
                        
                        

        AccumulatingOp ();
        AccumulatingOp (AccumulatingOp &);
        
        

        template <
                typename OPERAND_VECTOR_T, 
                unsigned int OPERAND_COUNT, 
                bool DO_RECURSE
        > struct rcreate ;

        
                
        template <typename OPERAND_VECTOR_T, unsigned int OPERAND_COUNT>
        struct rcreate <OPERAND_VECTOR_T, OPERAND_COUNT, true>
        {
                static typename TYPES::FunctionPtr
                create (
                        const unsigned int operandCount, 
                        const OPERAND_VECTOR_T &operands_
                ) {
                        if (OPERAND_COUNT == operandCount) {
                                return AccumulatingOpN <
                                        TYPES, 
                                        OP_T,
                                        OPERAND_COUNT,
                                        PREFIX_T
                                > :: create (operands_);
                        } else {
                                return rcreate <
                                        OPERAND_VECTOR_T,
                                        OPERAND_COUNT-1,
                                        (OPERAND_COUNT-1 > MIN_OPERAND_COUNT)
                                > :: create (operandCount, operands_);
                        }
                }
        };



        template <typename OPERAND_VECTOR_T, unsigned int OPERAND_COUNT>
        struct rcreate <OPERAND_VECTOR_T, OPERAND_COUNT, false>
        {
                static typename TYPES::FunctionPtr
                create (
                        const unsigned int /*operandCount*/, 
                        const OPERAND_VECTOR_T &operands_
                ) {
                        return AccumulatingOpN<
                                TYPES, OP_T, MIN_OPERAND_COUNT, PREFIX_T
                        > :: create (operands_);
                }
        };
        
        

public:

        template <typename OPERAND_VECTOR_T>
        static typename TYPES::FunctionPtr create (
                const unsigned int operandCount,
                const OPERAND_VECTOR_T &operands_
        ) {
                if (MIN_OPERAND_COUNT > operandCount)
                        throw insufficient_number_of_operands_exception (
                                                            MIN_OPERAND_COUNT);

                if (MAX_FOLD >= operandCount) {
                        return rcreate <
                                OPERAND_VECTOR_T, MAX_FOLD,
                                (MIN_OPERAND_COUNT < MAX_FOLD)
                        > :: create (operandCount, operands_);
                } else {
                        // Convert from arbitrary ::std::vector type to what
                        // is needed by AccumulatingOp.
                        ::boost::shared_array <typename TYPES::FunctionPtr> 
                        operands (
                                new typename TYPES::FunctionPtr [operandCount]
                        );

                        for (unsigned int i=0; i<operandCount; ++i) {
                                operands [i] = operands_ [i];
                        }

                        return typename TYPES::FunctionPtr (
                                new AccumulatingOp <
                                        TYPES, 
                                        OP_T, MIN_OPERAND_COUNT, MAX_FOLD,
                                        PREFIX_T
                                > (operandCount, operands));
                }
        }
        
        

        virtual ~AccumulatingOp () {
        }
        


        typename TYPES::scalar_t operator () (
                const typename TYPES::parameters_t &parameters
        ) const {
                // TODO: implement a static assert for operandCount>=1
                typename TYPES::scalar_t sum = PREFIX_T::operator () (
                                                (*operands [0]) (parameters));

                for (unsigned int i=1; i<operandCount; ++i) {
                        sum = PREFIX_T::operator () (
                              OP_T::operator () (
                                    sum, PREFIX_T::operator () (
                                        (*operands [i]) (parameters))));
                }
                return sum;
        }
};

} } }

#endif // ESTNODES_ACCUMULATINGOP_20090124
