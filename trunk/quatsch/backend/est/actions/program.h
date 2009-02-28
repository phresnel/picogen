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

#ifndef ACTPROGRAM__20090123
#define ACTPROGRAM__20090123
namespace quatsch {  namespace backend {  namespace est {  namespace parser_actions  {
    template <typename BACKEND> class Program {
            BACKEND &backend;
            Program ();
        public:
            
            class Begin {
                    BACKEND &backend;
                    Begin();
                public:
                    Begin (BACKEND &backend) : backend (backend) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t &, const code_iterator_t &) const {}
            };
            
            class End {
                    BACKEND &backend;
                    End ();
                public:
                    End (BACKEND &backend) : backend (backend) {}
                    template <typename code_iterator_t> void operator () (const code_iterator_t&, const code_iterator_t&) const {
                    }
            };
            
            Begin begin;
            End end;
            
            Program (BACKEND &backend) 
            : backend (backend)
            , begin (backend)
            , end (backend)
            {}
    };
} } } }
#endif // ACTPROGRAM__20090123
