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

#ifndef ESTEXCEPTIONS__20090123
#define ESTEXCEPTIONS__20090123

#include <string>
#include <sstream>

#include "../../quatsch.hh"

namespace quatsch {  namespace backend {  namespace est {


        //++
        // Important Note.
        //
        // Most exceptions defined in this file are not escalated to clients
        // of an compiler instance, except for invalid_program_exception.
        //
        // Instead, most exception types will be caught by:
        //      - backend/est/actions/*, in case of the est-backend
        //      - <other places>, in case of another backend
        //           (but as of 20090410, there is only the est backend)
        //--


        template <typename T> std::string to_string (T const &val) {
                std::stringstream ss;
                ss << val;
                return ss.str();
        }
    
    
    
        class definition_does_not_match_forward_declaration_exception
        : public general_exception {
        public:
                definition_does_not_match_forward_declaration_exception ()
                : general_exception ("definition does not match "
                                     "forward declaration"
                  )
                {}
        };
    
    
    
        class unresolved_forward_declaration_exception
        : public general_exception {
        public:
                unresolved_forward_declaration_exception ()
                : general_exception ("unresolved forward declaration")
                {}
        };
    
    
    
        class invalid_program_exception
        : public general_exception {
        public:
                invalid_program_exception ()
                : general_exception ("invalid program")
                {}
                
                
                
                invalid_program_exception (std::string const &message)
                : general_exception (message)
                {}
        };
    
    
    
        class operation_not_allowed_exception
        : public general_exception {
        public:
                operation_not_allowed_exception ()
                : general_exception ("operation not allowed")
                {}
        };
        
    
    
        class insufficient_number_of_operands_exception
        : public general_exception {
        private:
                const unsigned int minimumCount;
        public:
        
                insufficient_number_of_operands_exception (
                        unsigned int minimumCount_
                )
                : general_exception (
                        std::string("Insufficient number of operands: ")
                        + "At least " + to_string (minimumCount_)
                        + " are required"
                  )
                , minimumCount (minimumCount_)
                {
                }
            
                unsigned int getMinimumCount () const { return minimumCount; }
        };
    
    
    
        class too_many_operands_exception
        : public general_exception {
        private:
                const unsigned int maximumCount;
        public:
        
                too_many_operands_exception (
                        unsigned int maximumCount_
                )
                : general_exception (
                        std::string("Passed too many arguments: ")
                        + "At maximum, " + to_string (maximumCount_)
                        + " are allowed"
                  )
                , maximumCount (maximumCount_)
                {
                }

                unsigned int getMaximumCount () const { return maximumCount; }
        };
    
    
    
        class invalid_number_of_operands_exception
        : public general_exception {
        private:
                const unsigned int count;
        public:
                
                invalid_number_of_operands_exception (unsigned int count_)                
                : general_exception (
                        std::string("Passed a wrong number of arguments: ")
                        + "Exactly " + to_string (count_)
                        + " are required"
                  )
                , count (count_)
                {
                }
                
                unsigned int getRequiredCount () const { return count; }
        };



        class unknown_operator_exception
        : public general_exception {
        public:
                unknown_operator_exception ()
                : general_exception ("unknown operator")
                {}
        };



        class null_operation_exception
        : public general_exception {
        public:
                null_operation_exception ()
                : general_exception ("nulloperation")
                {}
        };
    
} } }
#endif // ESTEXCEPTIONS__20090123
