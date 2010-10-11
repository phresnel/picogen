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

#ifndef COLORPICKERCOLOR_HH
#define COLORPICKERCOLOR_HH

#include <spectralcolorpicker.hh>
#include <tristimuluscolorpicker.hh>

struct ColorPickerColor {
        enum Mode { Spectral, Tristimulus } mode;

        QVector<SpectralSample> spectral;
        TristimulusColor tristimulus;

        ColorPickerColor () : mode(Tristimulus), spectral(), tristimulus() {}
        ColorPickerColor (ColorPickerColor const & rhs)
        : mode(rhs.mode), spectral(rhs.spectral), tristimulus(rhs.tristimulus)
        {}
        ColorPickerColor &operator = (ColorPickerColor const & rhs) {
                mode = rhs.mode;
                spectral = rhs.spectral;
                tristimulus = rhs.tristimulus;
                return *this;
        }

        bool operator == (ColorPickerColor const & rhs) const {
                if (mode != rhs.mode) return false;
                if (tristimulus != rhs.tristimulus) return false;

                if (spectral.count() != rhs.spectral.count())
                        return false;

                for (int i=0; i<spectral.count(); ++i) {
                        if (spectral[i] != rhs.spectral[i])
                                return false;
                }

                return true;
        }

        bool operator != (ColorPickerColor const &rhs) const {
                return !(*this == rhs);
        }

        redshift::ReferenceSpectrum referenceSpectrum () const {
                std::vector<double> wavelengths;
                std::vector<double> amplitudes;
                foreach (SpectralSample sample, spectral) {
                        wavelengths.push_back(sample.wavelength);
                        amplitudes.push_back(sample.amplitude);
                }
                return redshift::ReferenceSpectrum::FromSampled(
                                &amplitudes[0],
                                &wavelengths[0],
                                wavelengths.size());
        }

        redshift::Spectrum spectrum () const {
                std::vector<redshift::real_t> wavelengths;
                std::vector<redshift::real_t> amplitudes;
                foreach (SpectralSample sample, spectral) {
                        wavelengths.push_back(sample.wavelength);
                        amplitudes.push_back(sample.amplitude);
                }
                return redshift::Spectrum::FromSampled(
                                &amplitudes[0],
                                &wavelengths[0],
                                wavelengths.size());
        }

        redshift::color::RGB rgb() const {
                return redshift::color::RGB(
                                tristimulus.redF(),
                                tristimulus.greenF(),
                                tristimulus.blueF());
        }

        redshift::color::RGB convertToRGB() const {
                switch (mode) {
                case Tristimulus:
                        return rgb();
                case Spectral:
                        return spectrum().toRGB();
                }
                throw std::runtime_error(
                        "unhandled mode in colorpicker.hh:convertToRGB()");
        }
        /*QColor toQColor () const {
                using redshift::Spectrum;

                switch (mode) {
                case Tristimulus:
                        return tristimulus.toQColor();
                case Spectral: {
                                const redshift::color::RGB rgb = toRGB();
                                const int
                                        r_ = rgb.R * 255,
                                        g_ = rgb.G * 255,
                                        b_ = rgb.B * 255,
                                        r = r_ < 0 ? 0 : r_ > 255 ? 255 : r_,
                                        g = g_ < 0 ? 0 : g_ > 255 ? 255 : g_,
                                        b = b_ < 0 ? 0 : b_ > 255 ? 255 : b_;
                                return QColor(r, g, b);
                        }
                }
                throw std::runtime_error(
                        "unhandled mode in colorpicker.hh:toColor()");
        }*/
};

#endif // COLORPICKERCOLOR_HH
