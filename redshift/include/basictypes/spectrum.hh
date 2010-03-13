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
                        math_max_as,
                        reduction_all,
                        relational_equal_to_as
                ;
        };

        template <unsigned int N>
        class spectrum_base : public array<real_t, N, r_spectrum> {
        public:
                enum { num_components = N };

                typedef redshift::real_t real_t;

                enum noinit_ {noinit};

                spectrum_base (noinit_)
                : array<real_t,N, r_spectrum>(array<real_t,N,r_spectrum>::noinit)
                {}

                spectrum_base ()
                : array<real_t,N,r_spectrum>()
                {}

                explicit spectrum_base (real_t f)
                : array<real_t,N,r_spectrum>(f)
                {}

                spectrum_base (const real_t (&f)[N])
                : array<real_t,N,r_spectrum>(f)
                {}

                spectrum_base (const spectrum_base &rhs)
                : array<real_t,N,r_spectrum>(rhs)
                {}

                template <typename REP>
                spectrum_base (array<real_t,N,r_spectrum,REP> const & rhs)
                : array<real_t,N,r_spectrum>(rhs)
                {}

                using array<real_t,N,r_spectrum>::operator=;
        private:
                /*static real_t XWeight[N];
                static real_t YWeight[N];
                static real_t ZWeight[N];*/
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
        inline bool operator == (spectrum_base<N> const &, real_t ) {
                throw std::runtime_error ("you are not allowed to call operator==(spectrum_base,real_t)");
        }

        template <unsigned int N>
        inline bool max (spectrum_base<N> const &) {
                throw std::runtime_error ("you are not allowed to call max(spectrum_base)");
        }

        template <unsigned int N>
        inline bool black (spectrum_base<N> const &s) {
                return kallisto::all(kallisto::operator==(s,real_t(0)));
        }

        template <unsigned int N>
        inline spectrum_base<N> clamp (spectrum_base<N> const &s) {
                return kallisto::max(s, typename spectrum_base<N>::real_t(0));
        }
}

#include <vector>
namespace redshift {

        enum { SAMPLED_LAMBDA_START = 400 };
        enum { SAMPLED_LAMBDA_END = 700 };
        enum { CIE_SAMPLES = 471 };

        enum { RGB_TO_SPECTRUM_SAMPLES = 32 };

        extern const real_t CIE_X[CIE_SAMPLES];
        extern const real_t CIE_Y[CIE_SAMPLES];
        extern const real_t CIE_Z[CIE_SAMPLES];
        extern const real_t CIE_lambda[CIE_SAMPLES];
        static const int nRGB2SpectSamples = 32;
        extern const real_t RGB2SpectLambda[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectWhite[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectCyan[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectMagenta[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectYellow[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectRed[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectGreen[nRGB2SpectSamples];
        extern const real_t RGBRefl2SpectBlue[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectWhite[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectCyan[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectMagenta[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectYellow[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectRed[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectGreen[nRGB2SpectSamples];
        extern const real_t RGBIllum2SpectBlue[nRGB2SpectSamples];

        extern bool spectrumSamplesSorted(const real_t *lambda, const real_t *vals, int n);
        extern void sortSpectrumSamples(real_t *lambda, real_t *vals, int n);

        typedef spectrum_base<6> _spectrum_base;
        class Spectrum : public _spectrum_base {
                typedef _spectrum_base base;
        public:
                enum noinit_ {noinit};

                Spectrum (noinit_) : base(base::noinit) {}
                Spectrum () : base() {}
                explicit Spectrum (real_t f) : base(f) {}
                Spectrum (const real_t (&f)[base::num_components]) : base(f) {}
                Spectrum (const Spectrum &rhs) : base(rhs) {}

                template <typename REP>
                Spectrum (array<real_t,base::num_components,r_spectrum,REP> const & rhs)
                : base(rhs)
                {}

                using base::operator=;

                real_t y() const {
                        real_t yy = 0.f;
                        for (int i = 0; i < num_components; ++i) {
                                yy += Y[i] * (*this)[i];
                        }
                        return yy / yint;
                }


                real_t at (real_t const lambda) const {
                        const real_t f = (lambda - SAMPLED_LAMBDA_START)
                                        /(SAMPLED_LAMBDA_END-SAMPLED_LAMBDA_START);
                        const real_t scaled = f * (num_components-1);
                        const int i = scaled;

                        if (i<0) return (*this)[0];
                        if (i+1>=num_components) return (*this)[num_components-1];

                        const real_t u = scaled - i;
                        return (1-u)*(*this)[i] + u*(*this)[i+1];
                }


                color::XYZ toXYZ() const {
                        color::XYZ ret;
                        for (int i = 0; i < num_components; ++i) {
                            ret.X += X[i] * (*this)[i];
                            ret.Y += Y[i] * (*this)[i];
                            ret.Z += Z[i] * (*this)[i];
                        }
                        ret.X /= yint;
                        ret.Y /= yint;
                        ret.Z /= yint;
                        return ret;
                }

                color::RGB toRGB() const {
                        return toXYZ().toRGB();
                }

        public: // Named constructors.
                static Spectrum FromSampled(
                        const real_t *v,
                        const real_t *lambda,
                        int n
                );

                static Spectrum FromSampled(
                        const real_t *v,
                        unsigned int lambdaStart, unsigned int lambdaEnd,
                        int n
                );

                static Spectrum FromRGB(color::RGB const &rgb) ;

        public: // Static functions.
                static void static_init ();

        private:
                static Spectrum X, Y, Z;
                static real_t yint;

                static Spectrum rgbRefl2SpectWhite, rgbRefl2SpectCyan;
                static Spectrum rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
                static Spectrum rgbRefl2SpectRed, rgbRefl2SpectGreen;
                static Spectrum rgbRefl2SpectBlue;
                static Spectrum rgbIllum2SpectWhite, rgbIllum2SpectCyan;
                static Spectrum rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
                static Spectrum rgbIllum2SpectRed, rgbIllum2SpectGreen;
                static Spectrum rgbIllum2SpectBlue;
        };
}

#endif // SPECTRUM_HH_INCLUDED_20100311
