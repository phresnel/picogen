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

#include <sstream>
#include <stdexcept>
#include "real.hh"
#include "array.hh"
#include "rgb.hh"

namespace crystal { namespace redshift {
        enum { spectrum_samples = 9 };
} }

namespace crystal { namespace redshift {

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
                        reduction_isnan,
                        reduction_isinf,
                        relational_equal_to_as
                ;
        };

        template <typename T, unsigned int N>
        class spectrum_base : public array<T, N, r_spectrum> {
        public:
                enum { num_components = N };

                typedef T real_t;

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
        template <typename T, unsigned int N>
        inline bool operator == (spectrum_base<T,N> const &, real_t ) {
                throw std::runtime_error ("you are not allowed to call operator==(spectrum_base,real_t)");
        }

        template <typename T, unsigned int N>
        inline spectrum_base<T,N> max (spectrum_base<T,N> const &, T) {
                throw std::runtime_error ("you are not allowed to call max(spectrum_base)");
        }

        template <typename T, unsigned int N>
        inline bool black (spectrum_base<T, N> const &s) {
                return kallisto::all(kallisto::operator==(s,spectrum_base<T, N>::real_t(0)));
        }

        template <typename T, unsigned int N>
        inline spectrum_base<T,N> clamp (spectrum_base<T,N> const &s) {
                return kallisto::max(s, typename spectrum_base<T, N>::real_t(0));
        }

        template <typename T, unsigned int N>
        inline bool isnan (spectrum_base<T,N> const &s) {
                return kallisto::isnan (s);
        }
        template <typename T, unsigned int N>
        inline bool isinf (spectrum_base<T,N> const &s) {
                return kallisto::isinf (s);
        }



        // Helper for spectrum internal use.
        template <typename T> struct SpectrumBaseTypeId_ ;
        template <> struct SpectrumBaseTypeId_<float> {
                static const char *name () { return "float"; }
        };
        template <> struct SpectrumBaseTypeId_<double> {
                static const char *name () { return "double"; }
        };
        template <> struct SpectrumBaseTypeId_<long double> {
                static const char *name () { return "long double"; }
        };
} }

#include <vector>
namespace crystal { namespace redshift {

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

        enum SpectrumKind {
                ReflectanceSpectrum,
                IlluminantSpectrum
        };

        template <typename real_t>
        extern bool spectrumSamplesSorted(const real_t *lambda, const real_t *vals, int n);

        template <typename real_t>
        extern void sortSpectrumSamples(real_t *lambda, real_t *vals, int n);

        //typedef spectrum_base<real_t,6> _spectrum_base;

        template <typename T, unsigned int N=spectrum_samples>
        class SpectrumBase : public spectrum_base<T,N> {
                typedef spectrum_base<T,N> base;
        public:
                enum noinit_ {noinit};

                enum { num_components = base::num_components };
                typedef typename base::real_t real_t;

                SpectrumBase (noinit_) : base(base::noinit) {}
                SpectrumBase () : base() {
                }

                template <typename U>
                explicit SpectrumBase (U f) : base(real_t(f)) {
                        //*this = FromRGB(real_t(f),real_t(f),real_t(f));
                }

                SpectrumBase (const real_t (&f)[base::num_components])
                : base(f)
                {
                        //*this = FromRGB(f,f,f);
                }

                SpectrumBase (const SpectrumBase &rhs) : base(rhs) {
                }

                template <typename U>
                explicit SpectrumBase (SpectrumBase<U> const & rhs) {
                        for (int i=0; i<num_components; ++i) {
                                (*this)[i] = static_cast<T>(rhs[i]);
                        }
                }

                template <typename REP>
                SpectrumBase  (array<real_t,base::num_components,r_spectrum,REP> const & rhs)
                : base(rhs) {
                }

                using base::operator=;

                real_t y() const {
                        real_t yy = 0.f;
                        for (int i = 0; i < base::num_components; ++i) {
                                yy += Y[i] * (*this)[i];
                        }
                        return yy / yint;
                }


                real_t at (real_t const lambda) const {
                        const real_t f = (lambda - SAMPLED_LAMBDA_START)
                                        /(SAMPLED_LAMBDA_END-SAMPLED_LAMBDA_START);
                        const real_t scaled = f * (base::num_components-1);
                        const int i = scaled;

                        if (i<0) return (*this)[0];
                        if (i+1>=base::num_components)
                                return (*this)[base::num_components-1];

                        const real_t u = scaled - i;
                        return (1-u)*(*this)[i] + u*(*this)[i+1];
                }


                color::XYZ toXYZ() const {
                        color::XYZ ret;
                        for (int i = 0; i < base::num_components; ++i) {
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

                template <typename U>
                typename kallisto::traits::enable_if_c<
                        num_components==SpectrumBase<U>::num_components,
                        SpectrumBase<U>
                >::type spectrum_cast() const {
                        SpectrumBase<U> ret (SpectrumBase<U>::noinit);
                        for (int i; i<num_components; ++i) {
                                ret [i] = static_cast<U>((*this)[i]);
                        }
                        return ret;
                }

        public: // Named constructors.
                static SpectrumBase FromSampled(
                        const real_t *v,
                        const real_t *lambda,
                        int n
                );

                static SpectrumBase FromSampled(
                        const real_t *v,
                        unsigned int lambdaStart, unsigned int lambdaEnd,
                        int n
                );

                template <unsigned int O>
                static SpectrumBase FromSpectrum(SpectrumBase<T,O> const &spec);

                static SpectrumBase FromRGB(color::RGB const &rgb, SpectrumKind kind) ;
                template <typename U>
                static SpectrumBase FromRGB(U R, U G, U B, SpectrumKind kind) {
                        return FromRGB(color::RGB(R,G,B), kind);
                }

                static SpectrumBase X, Y, Z;
                static real_t yint;

                static SpectrumBase rgbRefl2SpectWhite, rgbRefl2SpectCyan;
                static SpectrumBase rgbRefl2SpectMagenta, rgbRefl2SpectYellow;
                static SpectrumBase rgbRefl2SpectRed, rgbRefl2SpectGreen;
                static SpectrumBase rgbRefl2SpectBlue;
                static SpectrumBase rgbIllum2SpectWhite, rgbIllum2SpectCyan;
                static SpectrumBase rgbIllum2SpectMagenta, rgbIllum2SpectYellow;
                static SpectrumBase rgbIllum2SpectRed, rgbIllum2SpectGreen;
                static SpectrumBase rgbIllum2SpectBlue;

        public: // Static functions.
                static void static_init ();
        private:
                static bool static_init_called;


                /*void test_static_init() {
                        if (!static_init_called) {
                                std::stringstream ss;
                                ss << std::string("SpectrumBase<")
                                   << SpectrumBaseTypeId_<T>::name()
                                   << ", " << N
                                   << "> created, but static_init() "
                                   << "has not been called yet.";
                                throw std::runtime_error(ss.str());
                        }
                }*/
        };
} }

#include "spectrum.inl.hh"


namespace crystal { namespace redshift {
        //class Rgb;
        //typedef Rgb Color;
        enum { longspectrum_samples = 64 };
        enum { referencespectrum_samples = SAMPLED_LAMBDA_END-SAMPLED_LAMBDA_START };
        enum { halfreferencespectrum_samples = (SAMPLED_LAMBDA_END-SAMPLED_LAMBDA_START)/2 };

        typedef SpectrumBase<real_t,spectrum_samples> Spectrum, Color;
        typedef SpectrumBase<real_t,longspectrum_samples> LongSpectrum;
        typedef SpectrumBase<real_t,referencespectrum_samples> ReferenceSpectrum;
        typedef SpectrumBase<real_t,halfreferencespectrum_samples> HalfReferenceSpectrum;
} }


#endif // SPECTRUM_HH_INCLUDED_20100311
