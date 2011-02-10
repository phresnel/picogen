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

#include "float_compare.hh"
#include "cosyfloat.hh"

#include <cmath>

namespace cosyscene {



CosyFloat::CosyFloat() : value_(0) {
}



CosyFloat::CosyFloat(double value) : value_(value) {
}



double CosyFloat::toDouble() const {
        return value_;
}


bool CosyFloat::operator == (CosyFloat const &rhs) const {
        return detail::almost_eqal_abserr(toDouble(),
                                          rhs.toDouble(),
                                          0.0000001);
}
bool CosyFloat::operator != (CosyFloat const &rhs) const {
        return !(*this == rhs);
}
bool CosyFloat::operator <  (CosyFloat const &rhs) const {
        return (*this != rhs) && (value_ < rhs.value_);
}
bool CosyFloat::operator <= (CosyFloat const &rhs) const {
        return (*this==rhs) || (*this<=rhs);
}
bool CosyFloat::operator >  (CosyFloat const &rhs) const {
        return (*this!=rhs) && (*this>rhs);
}
bool CosyFloat::operator >= (CosyFloat const &rhs) const {
        return (*this==rhs) || (*this>=rhs);
}


CosyFloat CosyFloat::operator + (CosyFloat const &rhs) const {
        return CosyFloat (value_ + rhs.value_);
}
CosyFloat CosyFloat::operator - (CosyFloat const &rhs) const  {
        return CosyFloat (value_ - rhs.value_);
}
CosyFloat CosyFloat::operator * (CosyFloat const &rhs) const {
        return CosyFloat (value_ * rhs.value_);
}
CosyFloat CosyFloat::operator / (CosyFloat const &rhs) const {
        return CosyFloat (value_ / rhs.value_);
}



CosyFloat fabs (CosyFloat const &p) {
        return CosyFloat (std::fabs(p.value_));
}
CosyFloat sqrt (CosyFloat const &p) {
        return CosyFloat (std::sqrt(p.value_));
}
CosyFloat pow (CosyFloat const &l, CosyFloat const &r) {
        return CosyFloat (std::pow(l.value_, r.value_));
}


}
