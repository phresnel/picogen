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
namespace quatsch {  namespace backend {  namespace est {
    class definition_does_not_match_forward_declaration_exception {};
    class unresolved_forward_declaration_exception {};
    class invalid_program_exception {};
    class operation_not_allowed_exception {};
        
    class insufficient_number_of_operands_exception {
        private:
            const unsigned int minimumCount;
        public:
            insufficient_number_of_operands_exception (unsigned int minimumCount) : minimumCount (minimumCount) {}
            unsigned int getMinimumCount () const { return minimumCount; }
    };
    class too_many_operands_exception {
        private:
            const unsigned int maximumCount;
        public:
            too_many_operands_exception (unsigned int maximumCount) : maximumCount (maximumCount) {}
            unsigned int getMaximumCount () const { return maximumCount; }
    };
    class invalid_number_of_operands_exception {
        private:
            const unsigned int count;
        public:
            invalid_number_of_operands_exception (unsigned int count) : count (count) {}
            unsigned int getRequiredCount () const { return count; }
    };
    class unknown_operator_exception {};
    class null_operation_exception {};
} } }
#endif // ESTEXCEPTIONS__20090123
