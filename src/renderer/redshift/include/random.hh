//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010 Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef RANDOM_HH_INCLUDED_20100701
#define RANDOM_HH_INCLUDED_20100701

#include <stdint.h>
#include "kallisto/rng/kiss.hh"

namespace redshift {

        template <uint32_t m, uint32_t a, uint32_t c>
        class LCG {
        public:
                LCG (uint32_t s) : curr(s) {}

                uint32_t operator () () {
                        return curr = ((uint64_t)(((uint64_t)a*curr)&0xFFFFFFFFUL) + c) % m;
                }

                void seed (uint32_t s) {
                        curr = s;
                }
        private:
                uint32_t curr;
        };

        template <uint32_t m, uint32_t a, uint32_t c>
        class LCGf : private LCG<m,a,c> {
        public:
                LCGf(uint32_t s) : LCG<m,a,c>(s) {}

                float operator () () {
                        return LCG<m,a,c>::operator()() * (1.f/(1.f+m));
                }

                void seed (uint32_t s) {
                        LCG<m,a,c>::seed(s);
                }
        };

        class Random {
        public:
                Random (uint32_t A, uint32_t B, uint32_t C, uint32_t D)
                : rand(A,B,C,D)
                {}

                Random () : rand(1,1,1,1) {
                }

                Random (Random const &x)
                : rand(x.rand)
                {}

                Random& operator = (Random const &rhs) {
                        rand = rhs.rand;
                        return *this;
                }

                double operator () () {
                        return rand();
                }

                void skip (uint32_t u) {
                        rand.skip(u);
                }
        private:

                kallisto::random::marsaglia::UNI rand;
        };
}

#endif
