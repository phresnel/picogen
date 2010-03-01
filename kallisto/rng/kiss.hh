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

#ifndef KISS_HH_INCLUDED_20100301
#define KISS_HH_INCLUDED_20100301

// Implemented after:
// * http://www.bobwheeler.com/statistics/Password/MarsagliaPost.txt
// * http://www.math.niu.edu/~rusin/known-math/99/RNG

namespace kallisto { namespace random { namespace marsaglia {

        // -- Bits ----------------------------------------------------
        struct Znew {
                enum { max = 4294901760U };

                Znew (uint32_t value=362436069) : z(value) {}
                Znew & operator = (uint32_t value) {
                        z = value;
                        return *this;
                }
                uint32_t operator () () { return znew(); }
        protected:
                uint32_t znew() {
                        // orig: (z=36969 * (z&65535) + (z>>16)) << 16

                        // max z = 4294967295
                        const uint32_t
                          alpha = z&65535,     // max:     65535
                          bravo = 36969*alpha, // max:2422763415
                          charlie = z>>16      // max:     65535
                        ;
                        z = bravo + charlie;   // max:2422828950

                        // Note that the maximum of z does not yield
                        // the maximum return value, as for the left
                        // shift. The maximum return value is reached
                        // for all z that have the lower 16 bits set.
                        return static_cast<uint32_t>(z << 16);

                }
        private:
                uint32_t z;
        };

        struct Wnew {
                enum { max = 65535 };

                Wnew (uint32_t value=521288629) : w(value) {}
                Wnew & operator = (uint32_t value) {
                        w = value;
                        return *this;
                }
                uint32_t operator () () { return wnew(); }
        protected:
                uint32_t wnew() {
                        // orig: (w=18000*(w&65535)+(w>>16))&65535

                        // All maxima are in uint32_t bounds,
                        // hence behaviour should be well-defined.
                        const uint32_t
                          alpha = w&65535,     // max:65535
                          bravo = 18000*alpha, // max:1179630000
                          charlie = w>>16      // max:36968
                        ;
                        w = bravo + charlie;   // max:1179666968
                        return w & 65535;      // max:65535
                }
        private:
                uint32_t w;
        }; // okay

        // ------------------------------------------------------------



        // -- Basically, safe typedefs to uint --------------------------------
        struct Jsr {
                Jsr (uint32_t value=123456789) : jsr(value) {}
                Jsr & operator = (uint32_t value) {
                        jsr = value;
                        return *this;
                }
                operator uint32_t () const {
                        return jsr;
                }
        private:
                uint32_t jsr;
        };

        struct Jcong {
                Jcong (uint32_t value=380116160) : jcong(value) {}
                Jcong & operator = (uint32_t value) {
                        jcong = value;
                        return *this;
                }
                operator uint32_t () const {
                        return jcong;
                }
        private:
                uint32_t jcong;
        };
        // --------------------------------------------------------------------



        // -- Aggregates ------------------------------------------------------
        class MWC : Znew, Wnew {
        public:
                enum { max = Znew::max + Wnew::max /*4294967295*/ };

                MWC (Znew const & znew_, Wnew const & wnew_)
                : Znew(znew_), Wnew(wnew_)
                {}
                MWC (uint32_t znew_, uint32_t wnew_)
                : Znew(znew_), Wnew(wnew_)
                {}
                MWC () {}

                uint32_t operator () () { return mwc(); }
        protected:
                uint32_t mwc () {
                        return znew() + wnew();
                }
        };

        class SHR3 : Jsr {
                using Jsr::operator =;
                using Jsr::operator uint32_t;
        public:
                SHR3 (Jsr const &jsr_=Jsr()) : Jsr(jsr_) {}
                SHR3 (uint32_t jsr_) : Jsr(jsr_) {}

                uint32_t operator () () { return shr3(); }
        protected:
                uint32_t shr3 () {
                        *this = *this ^ static_cast<uint32_t>(*this<<17);
                        *this = *this ^ static_cast<uint32_t>(*this>>13);
                        *this = *this ^ static_cast<uint32_t>(*this<<5);
                        return *this;
                }
        };

        class CONG : Jcong {
                using Jcong::operator =;
                using Jcong::operator uint32_t;
        public:
                CONG (Jcong const &jcong_=Jcong()) : Jcong(jcong_) {}
                CONG (uint32_t jcong_) : Jcong(jcong_) {}
                uint32_t operator () () { return cong(); }
        protected:
                uint32_t cong () {
                        return *this = static_cast<uint32_t>(
                                static_cast<uint32_t>(69069 * *this)
                                + 1234567
                        );
                }
        };

        class KISS : MWC, CONG, SHR3 {
        public:
                KISS(MWC const&mwc_, CONG const&cong_, SHR3 const&shr3_)
                : MWC(mwc_), CONG(cong_), SHR3(shr3_)
                {}
                KISS(
                        uint32_t mwc_znew, uint32_t mwc_wnew,
                        uint32_t cong,
                        uint32_t shr3
                ) : MWC(mwc_znew, mwc_wnew)
                , CONG(cong)
                , SHR3(shr3)
                {}

                KISS () {}
                uint32_t operator () () { return kiss(); }
        protected:
                uint32_t kiss () {
                        return (mwc()^cong())+shr3();
                }
        };
        // --------------------------------------------------------------------


        // -- Distributions ---------------------------------------------------
        // I think UNI should be a distribution, not an RNG on itself
        class UNI : KISS {
        public:
                UNI () {}
                UNI (KISS const &kiss) : KISS(kiss) {}

                UNI(
                        uint32_t mwc_znew, uint32_t mwc_wnew,
                        uint32_t cong,
                        uint32_t shr3
                ) : KISS(mwc_znew, mwc_wnew, cong, shr3)
                {}

                double operator () () { return uni(); }
        protected:
                double uni () {
                        return kiss() * 2.328306e-10;
                }
        };

        // I think VNI should be a distribution, not an RNG on itself
        class VNI : KISS {
        public:
                VNI () {}
                VNI (KISS const &kiss) : KISS(kiss) {}
                VNI(
                        uint32_t mwc_znew, uint32_t mwc_wnew,
                        uint32_t cong,
                        uint32_t shr3
                ) : KISS(mwc_znew, mwc_wnew, cong, shr3)
                {}
                double operator () () { return vni(); }
        protected:
                double vni () {
                        // The original by Marsaglia converted kiss() to
                        // a signed integer instead of subtracting 1.0.
                        //----------------------------------------------
                        // C++-03, section 5.5:
                        // "If during  the  evaluation of an expression,
                        //  the result is not mathematically  defined or
                        //  not in the range of representable values for
                        //  its type, the behavior is undefined,  unless
                        //  such an expression is a constant expression"
                        // C++-03, section 4.7. Integral Conversion:
                        //  3) If  the  destination type is signed,  the
                        //     value is  unchanged  if it can be  repre-
                        //     sented in the destination type  (and bit-
                        //     field width);  otherwise,  the  value  is
                        //     implementation-defined.
                        //
                        return kiss() * 4.656613e-10 - 1;
                }
        };
        // --------------------------------------------------------------------

} } }
#endif
