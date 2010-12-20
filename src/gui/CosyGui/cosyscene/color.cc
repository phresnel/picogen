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

#include "color.hh"

#include <cmath>

namespace cosyscene {

const actuarius::Enum<Color::Kind> Color::Typenames =
( actuarius::Nvp<Color::Kind>(Color::Rgb, "rgb")
| actuarius::Nvp<Color::Kind>(Color::Spectrum, "spectrum")
);


//------------------------------------------------------------------------------
WavelengthAmplitudePair::WavelengthAmplitudePair()
: wavelength_(0)
, amplitude_(0)
{
}

WavelengthAmplitudePair::WavelengthAmplitudePair(double w, double a)
: wavelength_(w)
, amplitude_(a)
{
}

double WavelengthAmplitudePair::amplitude() const {
        return amplitude_;
}

double WavelengthAmplitudePair::wavelength() const {
        return wavelength_;
}



//------------------------------------------------------------------------------
Spectrum::Spectrum() {
}

Spectrum::Spectrum (std::vector<WavelengthAmplitudePair> const &samples)
: samples_(samples)
{
}

std::vector<WavelengthAmplitudePair> Spectrum::samples() const {
        return samples_;
}

void Spectrum::setSamples (std::vector<WavelengthAmplitudePair> const &s) {
        samples_ = s;
}

size_t Spectrum::size() const {
        return samples_.size();
}

WavelengthAmplitudePair Spectrum::operator [] (size_t i) const {
        return samples_[i];
}



//------------------------------------------------------------------------------
Rgb::Rgb() : r_(1), g_(1), b_(1) {
}

Rgb::Rgb (double r, double g, double b) : r_(r), g_(g), b_(b) {
}

double Rgb::r() const {
        return r_;
}

double Rgb::g() const {
        return g_;
}

double Rgb::b() const {
        return b_;
}



//------------------------------------------------------------------------------
Color::Color () : kind_(Rgb), rgb_(1,1,1) {
}
Color::Kind Color::kind() const {
        return kind_;
}
Rgb Color::rgb() const {
        return rgb_;
}
Spectrum Color::spectrum() const {
        return spectrum_;
}
void Color::toRgb (cosyscene::Rgb const &val) {
        kind_ = Rgb;
        rgb_ = val;
}
void Color::toSpectrum(cosyscene::Spectrum const &val) {
        kind_ = Spectrum;
        spectrum_ = val;
}



}
