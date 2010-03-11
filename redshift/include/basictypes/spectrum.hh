//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef SPECTRUM_HH_INCLUDED_20100311
#define SPECTRUM_HH_INCLUDED_20100311

namespace redshift {

        // The basic strategy is to derive from the expression-template
        // kallisto::array<>, and then to cherry pick the operations
        // from it that suit spectrum. For a set of available operations
        // on kallisto::array<>'s, see kallist/types/array_bits/rules.hh

        struct r_spectrum
        : kallisto::r_floating_basic
        {
                typedef void
                        math_exp,
                        reduction_all,
                        relational_equal_to_as
                ;
        };

        template <unsigned int N>
        class Spectrum : public array<real_t, N, r_spectrum> {
        public:
                enum { num_components = N };

                enum noinit_ {noinit};

                Spectrum (noinit_)
                : array<real_t,N, r_spectrum>(array<real_t,N,r_spectrum>::noinit)
                {}

                Spectrum ()
                : array<real_t,N,r_spectrum>()
                {}

                explicit Spectrum (real_t f)
                : array<real_t,N,r_spectrum>(f)
                {}

                Spectrum (const real_t (&f)[N])
                : array<real_t,N,r_spectrum>(f)
                {}

                Spectrum (const Spectrum &rhs)
                : array<real_t,N,r_spectrum>(rhs)
                {}

                template <typename REP>
                Spectrum (array<real_t,N,r_spectrum,REP> const & rhs)
                : array<real_t,N,r_spectrum>(rhs)
                {}

                using array<real_t,N,r_spectrum>::operator=;

        private:
        };


        // C++0x will offer better ways that this trap,
        // e.g. I could use a Spectrum-Expr-Helper, which itself
        // uses a lot of decltype() (*), to proxy Spectrum-Expressions,
        // and not expose functions not intended for Spectrum-clients.
        // But for now, those traps will do a good-enough job in forbidding
        // those calls on Spectrums.
        //
        // (*) e.g., we could make array<> a member of Spectrum, instead
        //     of deriving from it, and then, assuming 'c' being that member,
        //     define
        //
        //        decltype(Spectrum::c+Spectrum::c)
        //          operator + (Spectrum lhs, Spectrum rhs) {
        //              return lhs.c + rhs.c;
        //        }
        //
        //     And as we anyways want to allow clients to initialize Spectra
        //     coefficient by coefficient, this would not introduce a security hole,
        //     as to abuse this system, the user would have to
        //
        //        array<> x (freaking_invalid_float);
        //        Spectrum s (x);
        //
        //     which, as said, we want to allow anyways.
        //
        // Note that we only have to forbid what is in r_spectrum,
        // other functions than mentioned there do not exist (enable_if fu).
        // Also, we don't have to forbid the bool-reductions (all,none,any),
        // as the user won't be able to pass ==.
        template <unsigned int N>
        inline bool operator == (Spectrum<N> const &, real_t ) {
                throw std::runtime_error ("you are not allowed to call operator==(Spectrum,real_t)");
        }

        template <unsigned int N>
        inline bool black (Spectrum<N> const &s) {
                return kallisto::all(kallisto::operator==(s,real_t(0)));
        }
}

#endif // SPECTRUM_HH_INCLUDED_20100311
