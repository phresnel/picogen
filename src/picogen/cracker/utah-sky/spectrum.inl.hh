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

// C++0x TODO:
//   * In source file, explicitly instantiate SpectrumBase on floating types
//   * In this file, extern-explicitly-instantiate SpectrumBase on floating types
//   * And then, un-inline all functions herein

#include <sstream>
#include "algorithm.hh"

namespace picogen { namespace redshift {

// Spectral Data Definitions
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::X;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::Y;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::Z;
template <typename T, unsigned int N> typename SpectrumBase<T,N>::real_t SpectrumBase<T,N>::yint;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectWhite;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectCyan;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectMagenta;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectYellow;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectRed;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectGreen;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbRefl2SpectBlue;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectWhite;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectCyan;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectMagenta;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectYellow;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectRed;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectGreen;
template <typename T, unsigned int N> SpectrumBase<T,N> SpectrumBase<T,N>::rgbIllum2SpectBlue;




template <typename real_t>
inline bool spectrumSamplesSorted(const real_t *lambda, const real_t */*vals*/, int n) {
    for (int i = 0; i < n-1; ++i)
        if (lambda[i] > lambda[i+1]) {
                return false;
        }
    return true;
}


template <typename real_t>
inline void sortSpectrumSamples(real_t *lambda, real_t *vals, int n) {
    std::vector<std::pair<real_t, real_t> > sortVec;
    sortVec.reserve(n);
    for (int i = 0; i < n; ++i)
        sortVec.push_back(std::make_pair(lambda[i], vals[i]));
    std::sort(sortVec.begin(), sortVec.end());
    for (int i = 0; i < n; ++i) {
        lambda[i] = sortVec[i].first;
        vals[i] = sortVec[i].second;
    }
}



template <typename samples_t, typename real_t>
inline real_t averageSpectrumSamples(const samples_t *lambda, const samples_t *vals,
        int n, real_t lambdaStart, real_t lambdaEnd) {
    //for (int i = 0; i < n-1; ++i) Assert(lambda[i+1] > lambda[i]);
    //Assert(lambdaStart < lambdaEnd);
    // Handle cases with out-of-bounds range or single sample only
    if (lambdaEnd <= lambda[0]) return vals[0];
    if (lambdaStart >= lambda[n-1]) return vals[n-1];
    if (n == 1) return vals[0];
    real_t sum = 0.f;
    // Add contributions of constant segments before/after samples
    if (lambdaStart < lambda[0])
        sum += vals[0] * (lambda[0] - lambdaStart);
    if (lambdaEnd > lambda[n-1])
        sum += vals[n-1] * (lambdaEnd - lambda[n-1]);

    // Advance to first relevant wavelength segment
    int i = 0;
    while (lambdaStart > lambda[i+1]) ++i;
    //Assert(i+1 < n);

    // Loop over wavelength sample segments and add contributions
#define INTERP(w, i) \
        lerp((real_t)(((w) - lambda[i]) / (lambda[(i)+1] - lambda[i])), \
        (real_t)vals[i], (real_t)vals[(i)+1])
#define SEG_AVG(wl0, wl1, i) (0.5f * (INTERP(wl0, i) + INTERP(wl1, i)))
    for (; i+1 < n && lambdaEnd >= lambda[i]; ++i) {
        real_t segStart = std::max((real_t)lambdaStart, (real_t)lambda[i]);
        real_t segEnd = std::min((real_t)lambdaEnd, (real_t)lambda[i+1]);
        sum += SEG_AVG(segStart, segEnd, i) * (segEnd - segStart);
    }
#undef INTERP
#undef SEG_AVG
    return sum / (lambdaEnd - lambdaStart);
}



template <typename T, unsigned int N>
bool SpectrumBase<T, N>::static_init_called = false;



template <typename T, unsigned int N>
inline void SpectrumBase<T, N>::static_init() {

        if (static_init_called) {
                std::stringstream ss;
                ss << std::string("SpectrumBase<")
                   << SpectrumBaseTypeId_<T>::name()
                   << ", " << N
                   << ">::static_init() called multiple times";
                throw std::runtime_error(ss.str());
        }

        static_init_called = true;

        typedef T real_t;
        //std::cout << "void SpectrumBase::static_init() {\n";
        // Compute XYZ matching functions for _SampledSpectrum_
        yint = 0;
        for (int i = 0; i < num_components; ++i) {
            const real_t wl0 = lerp(real_t(i) / num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            const real_t wl1 = lerp(real_t(i+1) / num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            X[i] = averageSpectrumSamples(CIE_lambda, CIE_X, CIE_SAMPLES,
                                            wl0, wl1);
            Y[i] = averageSpectrumSamples(CIE_lambda, CIE_Y, CIE_SAMPLES,
                                            wl0, wl1);
            Z[i] = averageSpectrumSamples(CIE_lambda, CIE_Z, CIE_SAMPLES,
                                            wl0, wl1);

            /*std::cout << "        X[" << i << "] = " << X[i] << std::endl;
            std::cout << "        Y[" << i << "] = " << Y[i] << std::endl;
            std::cout << "        Z[" << i << "] = " << Z[i] << std::endl;*/
            yint += Y[i];
        }

        //std::cout << "        yint: " << yint << std::endl;

        // Compute RGB to SpectrumBase functions for _SampledSpectrum_
        for (int i = 0; i < num_components; ++i) {
            const real_t wl0 = lerp(real_t(i) / num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            const real_t wl1 = lerp(real_t(i+1) / num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            rgbRefl2SpectWhite[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectWhite,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectCyan[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectCyan,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectMagenta[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectMagenta,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectYellow[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectYellow,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectRed[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectRed,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectGreen[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectGreen,
                nRGB2SpectSamples, wl0, wl1);
            rgbRefl2SpectBlue[i] = averageSpectrumSamples(RGB2SpectLambda, RGBRefl2SpectBlue,
                nRGB2SpectSamples, wl0, wl1);

            rgbIllum2SpectWhite[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectWhite,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectCyan[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectCyan,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectMagenta[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectMagenta,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectYellow[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectYellow,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectRed[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectRed,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectGreen[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectGreen,
                nRGB2SpectSamples, wl0, wl1);
            rgbIllum2SpectBlue[i] = averageSpectrumSamples(RGB2SpectLambda, RGBIllum2SpectBlue,
                nRGB2SpectSamples, wl0, wl1);
        }
}



template <typename real_t, unsigned int N>
inline SpectrumBase<real_t, N> SpectrumBase<real_t,N>::FromSampled(
        const real_t *v,
        const real_t *lambda, int n
) {
        using std::vector;
        // Sort samples if unordered, use sorted for returned spectrum
        if (!spectrumSamplesSorted(lambda, v, n)) {
            vector<real_t> slambda(&lambda[0], &lambda[n]);
            vector<real_t> sv(&v[0], &v[n]);
            sortSpectrumSamples(&slambda[0], &sv[0], n);

            return FromSampled(&sv[0], &slambda[0], n);
        }
        SpectrumBase r;

        //std::cout << "        sampling: ";
        for (int i = 0; i < base::num_components; ++i) {
            // Compute average value of given SPD over $i$th sample's range
            const real_t lambda0 = lerp(real_t(i) / base::num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            const real_t lambda1 = lerp(real_t(i+1) / base::num_components,
                        (real_t)SAMPLED_LAMBDA_START, (real_t)SAMPLED_LAMBDA_END);
            r[i] = averageSpectrumSamples(lambda, v, n, lambda0, lambda1);
            //std::cout << r[i] << " ";
        }
        //std::cout << std::endl;
        return r;
}



template <typename real_t, unsigned int N>
inline SpectrumBase<real_t, N> SpectrumBase<real_t,N>::FromSampled(
        const real_t *v,
        unsigned int lambdaStart, unsigned int lambdaEnd,
        int n
) {
        // This function is only a quick hack!
        std::vector<real_t> lambda(n);

        for (int u=0; u<n; ++u) {
                lambda[u] = lerp(u/(real_t)n, (real_t)lambdaStart, (real_t)lambdaEnd);
        }
        const SpectrumBase ret = SpectrumBase::FromSampled (v, &lambda[0], n);
        return ret;
}



template <typename T, unsigned int N>
template <unsigned int O>
inline SpectrumBase<T, N>
  SpectrumBase<T, N>::FromSpectrum(SpectrumBase<T,O> const &spec) {
        // Quick hack as well.
        std::vector<T> amplitude(N);
        for (int u=0; u<N; ++u)
                amplitude[u] = spec[u];

        const SpectrumBase ret = SpectrumBase::FromSampled (
                        &amplitude[0],
                        SAMPLED_LAMBDA_START,
                        SAMPLED_LAMBDA_END,
                        N
        );
        return ret;
}



template <typename T, unsigned int N>
inline SpectrumBase<T,N> SpectrumBase<T,N>::FromRGB(color::RGB const &rgb, SpectrumKind kind) {

    // seems to be based on http://www.cs.utah.edu/~bes/papers/color/paper.pdf /phresnel
    SpectrumBase r;
    switch (kind) {
    case ReflectanceSpectrum: {
        // Convert reflectance SpectrumBase to RGB
        if (rgb.R <= rgb.G && rgb.R <= rgb.B) {
            // Compute reflectance _SampledSpectrum_ with _rgb.R_ as minimum
            r += rgb.R * rgbRefl2SpectWhite;
            if (rgb.G <= rgb.B) {
                r += (rgb.G - rgb.R) * rgbRefl2SpectCyan;
                r += (rgb.B - rgb.G) * rgbRefl2SpectBlue;
            }
            else {
                r += (rgb.B - rgb.R) * rgbRefl2SpectCyan;
                r += (rgb.G - rgb.B) * rgbRefl2SpectGreen;
            }
        }
        else if (rgb.G <= rgb.R && rgb.G <= rgb.B) {
            // Compute reflectance _SampledSpectrum_ with _rgb.G_ as minimum
            r += rgb.G * rgbRefl2SpectWhite;
            if (rgb.R <= rgb.B) {
                r += (rgb.R - rgb.G) * rgbRefl2SpectMagenta;
                r += (rgb.B - rgb.R) * rgbRefl2SpectBlue;
            }
            else {
                r += (rgb.B - rgb.G) * rgbRefl2SpectMagenta;
                r += (rgb.R - rgb.B) * rgbRefl2SpectRed;
            }
        }
        else {
            // Compute reflectance _SampledSpectrum_ with _rgb.B_ as minimum
            r += rgb.B * rgbRefl2SpectWhite;
            if (rgb.R <= rgb.G) {
                r += (rgb.R - rgb.B) * rgbRefl2SpectYellow;
                r += (rgb.G - rgb.R) * rgbRefl2SpectGreen;
            }
            else {
                r += (rgb.G - rgb.B) * rgbRefl2SpectYellow;
                r += (rgb.R - rgb.G) * rgbRefl2SpectRed;
            }
        }
        r *= typename SpectrumBase::real_t(.94);
    } break;
    case IlluminantSpectrum: {
        // Convert illuminant SpectrumBase to RGB
        if (rgb.R <= rgb.G && rgb.R <= rgb.B) {
            // Compute illuminant _SampledSpectrum_ with _rgb.R_ as minimum
            r += rgb.R * rgbIllum2SpectWhite;
            if (rgb.G <= rgb.B) {
                r += (rgb.G - rgb.R) * rgbIllum2SpectCyan;
                r += (rgb.B - rgb.G) * rgbIllum2SpectBlue;
            }
            else {
                r += (rgb.B - rgb.R) * rgbIllum2SpectCyan;
                r += (rgb.G - rgb.B) * rgbIllum2SpectGreen;
            }
        }
        else if (rgb.G <= rgb.R && rgb.G <= rgb.B) {
            // Compute illuminant _SampledSpectrum_ with _rgb.G_ as minimum
            r += rgb.G * rgbIllum2SpectWhite;
            if (rgb.R <= rgb.B) {
                r += (rgb.R - rgb.G) * rgbIllum2SpectMagenta;
                r += (rgb.B - rgb.R) * rgbIllum2SpectBlue;
            }
            else {
                r += (rgb.B - rgb.G) * rgbIllum2SpectMagenta;
                r += (rgb.R - rgb.B) * rgbIllum2SpectRed;
            }
        }
        else {
            // Compute illuminant _SampledSpectrum_ with _rgb.B_ as minimum
            r += rgb.B * rgbIllum2SpectWhite;
            if (rgb.R <= rgb.G) {
                r += (rgb.R - rgb.B) * rgbIllum2SpectYellow;
                r += (rgb.G - rgb.R) * rgbIllum2SpectGreen;
            }
            else {
                r += (rgb.G - rgb.B) * rgbIllum2SpectYellow;
                r += (rgb.R - rgb.G) * rgbIllum2SpectRed;
            }
        }
        r *= typename SpectrumBase::real_t(.86445f);
    } break;
    } // switch
    return clamp(r);
}


} }
