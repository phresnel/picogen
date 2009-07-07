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

#ifndef ESTNODES_LERP_HH_20090411
#define ESTNODES_LERP_HH_20090411
namespace quatsch { namespace backend { namespace est {


template <typename TYPES, unsigned int numOps>
class LerpN : public TYPES::Function
{

        enum { debug = 0 };

        typename TYPES::FunctionPtr operands [numOps];

        template <typename operands_vec_t> LerpN (const operands_vec_t &operands) {
                for (unsigned int i=0; i<numOps; ++i) {
                        this->operands [i] = operands [i];
                }
                if (debug) {
                        ::std::cout << "Instantiated a LerpN <" << numOps << ">" << ::std::endl;
                }
        }

        LerpN ();
        LerpN (LerpN &);

public:
        template <typename operand_vector_t>
        static typename TYPES::FunctionPtr create (const operand_vector_t &operands_) {
                return typename TYPES::FunctionPtr (
                               new LerpN <TYPES, numOps> (operands_)
                       );
        }
        typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters)
        const {
                const typename TYPES::scalar_t
                     indexF_ = (*operands [0])(parameters),
                     indexF  = indexF_ * static_cast<
                                                typename TYPES::scalar_t
                                         > (numOps-2),
                     fractional = indexF - util::floor (indexF)
                ;
                
                const int operandCount = static_cast<int>(numOps);
                                
                const int
                        index_ = 1 + static_cast<size_t> (
                                       util::floor (indexF)
                                 ),                        
                        
                        indexA = index_ < 1 ? 1
                               : index_ >= (operandCount-1) ? operandCount-1
                               : index_,
                               
                        indexB = index_ < 1 ? 1
                               : index_+1 >= (operandCount-1) ? operandCount-1
                               : index_+1                        
                ;
                
                /*
                std::cout << "LerpN<" << numOps << ">:" << std::endl;
                std::cout << "  indexF_=" << indexF_ << std::endl;
                std::cout << "  indexF=" << indexF << std::endl;
                std::cout << "  fractional=" << fractional << std::endl;
                std::cout << "  index_=" << index_ << std::endl;
                std::cout << "  indexA=" << indexA << std::endl;
                std::cout << "  indexB=" << indexB << std::endl;
                */               
                          
                return (*operands [indexA])(parameters) * (1 - fractional)
                     + (*operands [indexB])(parameters) * fractional;
        }
};



// I hereby request a better name. Thanks.
template <typename TYPES, unsigned int min_operand_count=3, unsigned int max_fold=8>
class Lerp : public TYPES::Function
{

        enum { debug = 0 };

        const unsigned int operandCount;
        const ::boost::shared_array <typename TYPES::FunctionPtr> operands;

        Lerp (const unsigned int operandCount,
              const ::boost::shared_array <typename TYPES::FunctionPtr> operands)
                        : operandCount (operandCount), operands (operands) { }

        Lerp ();
        Lerp (Lerp &);

        template <typename operand_vector_t, unsigned int operand_count, bool do_recurse> struct rcreate ;

        template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, true> {
                static typename TYPES::FunctionPtr create (const unsigned int operandCount, const operand_vector_t &operands_) {
                        if (operand_count == operandCount) {
                                return LerpN <TYPES, operand_count>::create (operands_);
                        } else {
                                return rcreate <operand_vector_t, operand_count-1, (operand_count-1 > min_operand_count)> :: create (operandCount, operands_);
                        }
                }
        };

        template <typename operand_vector_t, unsigned int operand_count> struct rcreate <operand_vector_t, operand_count, false> {
                static typename TYPES::FunctionPtr create (const unsigned int /*operandCount*/, const operand_vector_t &operands_) {
                        return LerpN<TYPES, min_operand_count>::create (operands_);
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
                        // Convert from arbitrary ::std::vector type to what Lerp needs.
                        ::boost::shared_array <typename TYPES::FunctionPtr> operands (
                                new typename TYPES::FunctionPtr [operandCount]
                        );
                        for (unsigned int i=0; i<operandCount; ++i) {
                                operands [i] = operands_ [i];
                        }
                        return typename TYPES::FunctionPtr (
                               new Lerp <TYPES, min_operand_count, max_fold> (operandCount, operands)
                        );
                }
        }

        virtual ~Lerp () {
        }

        typename TYPES::scalar_t operator () (const typename TYPES::parameters_t &parameters) const {
                const typename TYPES::scalar_t
                     indexF_ = (*operands [0])(parameters),
                     indexF  = indexF_ * static_cast<
                                                typename TYPES::scalar_t
                                         > (operandCount-2),
                     fractional = indexF - util::floor (indexF)
                ;
                
                const int operandCount = static_cast<int>(this->operandCount);
                                
                const int
                        index_ = 1 + static_cast<size_t> (
                                       util::floor (indexF)
                                 ),                        
                        
                        indexA = index_ < 1 ? 1
                               : index_ >= (operandCount-1) ? operandCount-1
                               : index_,
                               
                        indexB = index_ < 1 ? 1
                               : index_+1 >= (operandCount-1) ? operandCount-1
                               : index_+1                        
                ;
                
                /*
                std::cout << "LerpN<" << numOps << ">:" << std::endl;
                std::cout << "  indexF_=" << indexF_ << std::endl;
                std::cout << "  indexF=" << indexF << std::endl;
                std::cout << "  fractional=" << fractional << std::endl;
                std::cout << "  index_=" << index_ << std::endl;
                std::cout << "  indexA=" << indexA << std::endl;
                std::cout << "  indexB=" << indexB << std::endl;
                */               
                          
                return (*operands [indexA])(parameters) * (1 - fractional)
                     + (*operands [indexB])(parameters) * fractional;
        }
};

}
}
}
#endif // ESTNODES_LERP_HH_20090411
