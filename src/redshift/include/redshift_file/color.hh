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

#ifndef COLOR_HH_INCLUDED_20101013
#define COLOR_HH_INCLUDED_20101013

namespace redshift_file {
        // Rgb.
        struct WavelengthAmplitudePair {
                double wavelength;
                double amplitude;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(wavelength) & pack(amplitude);
                }
        };

        struct Spectrum {
                std::vector<WavelengthAmplitudePair> samples;

                redshift::Color toColor() const {
                        typedef redshift::Color::real_t real_t;
                        typedef std::vector<WavelengthAmplitudePair>::
                                const_iterator iterator;

                        std::vector<real_t> wavelengths;
                        std::vector<real_t> amplitudes;

                        for (iterator it = samples.begin();
                             it != samples.end();
                             ++it
                        ) {
                                wavelengths.push_back(it->wavelength);
                                amplitudes.push_back(it->amplitude);
                        }

                        return redshift::Color::FromSampled (
                                        &amplitudes[0],
                                        &wavelengths[0],
                                        samples.size());
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(samples);
                }
        };

        struct Rgb {
                double r,g,b;

                Rgb (double r, double g, double b) : r(r), g(g), b(b) {}
                Rgb () : r(1), g(1), b(1) {}

                redshift::Color toColor (redshift::SpectrumKind kind) const {
                        return redshift::Color::FromRGB(r,g,b, kind);
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack(r) & pack(g) & pack(b);
                }
        };        

        struct Color {

                enum Type {
                        RGB, Spectrum
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                Rgb rgb;
                redshift_file::Spectrum spectrum;

                Color () : type(RGB), rgb(0,1,1) {}
                Color (double r, double g, double b) : type(RGB), rgb(r,g,b) {}

                // to redshift
                redshift::Color toColor(redshift::SpectrumKind kind) const {
                        switch (type) {
                        case RGB: return rgb.toColor(kind);
                        case Spectrum: return spectrum.toColor();
                        }
                        throw std::runtime_error("unknown color type in "
                                "scenefile::Color::toColor()");
                }
                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack ("type", type, Typenames);

                        switch (type) {
                        case RGB:
                                arch & pack("rgb", rgb);
                                break;
                        case Spectrum:
                                arch & pack("spectrum", spectrum);
                                break;
                        }
                }
        };
}

#endif // COLOR_HH_INCLUDED_20101013