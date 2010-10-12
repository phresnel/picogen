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

#ifndef COLOR_HH_20101012
#define COLOR_HH_20101012

#include <vector>
#include "actuarius/bits/enum.hh"

namespace cosyscene {

//------------------------------------------------------------------------------
class WavelengthAmplitudePair {
public:

        WavelengthAmplitudePair (double wavelength, double amplitude);
        WavelengthAmplitudePair ();

        double wavelength() const;
        double amplitude() const;

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch) ;
private:
        double wavelength_;
        double amplitude_;
};
inline bool operator == (WavelengthAmplitudePair const & lhs,
                         WavelengthAmplitudePair const & rhs) {
        return lhs.wavelength() == rhs.wavelength()
            && lhs.amplitude()  == rhs.amplitude();
}
inline bool operator != (WavelengthAmplitudePair const & lhs,
                         WavelengthAmplitudePair const & rhs) {
        return !(lhs == rhs);
}



//------------------------------------------------------------------------------
class Spectrum {
public:
        Spectrum();
        Spectrum (std::vector<WavelengthAmplitudePair> const &samples);

        std::vector<WavelengthAmplitudePair> samples() const;
        void setSamples (std::vector<WavelengthAmplitudePair> const &);

        size_t size() const;
        WavelengthAmplitudePair operator [] (size_t i) const ;

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch);
private:
        std::vector<WavelengthAmplitudePair> samples_;
};
inline bool operator == (Spectrum const & lhs, Spectrum const & rhs) {
        if (lhs.size() != rhs.size())
                return false;
        for (size_t i=0; i<lhs.size(); ++i) {
                if (lhs[i] != rhs[i]) return false;
        }
        return true;
}



//------------------------------------------------------------------------------
class Rgb {
public:
        Rgb();
        Rgb (double r, double g, double b);
        double r() const;
        double g() const;
        double b() const;

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch);
private:
        double r_, g_, b_;
};
inline bool operator == (Rgb const &lhs, Rgb const &rhs) {
        return lhs.r() == rhs.r()
            && lhs.g() == rhs.g()
            && lhs.b() == rhs.b();
}



//------------------------------------------------------------------------------
class Color {
public:
        enum Kind {
                Spectrum,
                Rgb
        };

        static const actuarius::Enum<Kind> Typenames;

        Kind kind() const;
        cosyscene::Rgb rgb() const;
        cosyscene::Spectrum spectrum() const;

        void toRgb (cosyscene::Rgb const &);
        void toSpectrum(cosyscene::Spectrum const &);

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch);

private:
        Kind kind_;
        cosyscene::Rgb rgb_;
        cosyscene::Spectrum spectrum_;
};
inline bool operator == (Color const &lhs, Color const &rhs) {
        if (lhs.kind() != rhs.kind())
                return false;
        switch (lhs.kind()) {
        case Color::Spectrum:
                return lhs.spectrum() == rhs.spectrum();
        case Color::Rgb:
                return lhs.rgb() == rhs.rgb();
        }
}

} // namespace cosyscene

#endif // COLOR_HH_20101012
