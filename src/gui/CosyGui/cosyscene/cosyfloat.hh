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

#ifndef COSYFLOAT_HH_20110210
#define COSYFLOAT_HH_20110210

#include <iosfwd>

namespace cosyscene {


class CosyFloat {
public:
        CosyFloat () ;
        CosyFloat (double val); // Implicit by intention. An explicit conversion
                                // to double is enough, I think.

        double toDouble() const;

        bool operator == (CosyFloat const &rhs) const;
        bool operator != (CosyFloat const &rhs) const;
        bool operator <  (CosyFloat const &rhs) const;
        bool operator <= (CosyFloat const &rhs) const;
        bool operator >  (CosyFloat const &rhs) const;
        bool operator >= (CosyFloat const &rhs) const;

        CosyFloat operator + (CosyFloat const &rhs) const;
        CosyFloat operator - (CosyFloat const &rhs) const;
        CosyFloat operator * (CosyFloat const &rhs) const;
        CosyFloat operator / (CosyFloat const &rhs) const;

        friend CosyFloat fabs (CosyFloat const &);
        friend CosyFloat sqrt (CosyFloat const &);
        friend CosyFloat pow (CosyFloat const &, CosyFloat const &);

        friend std::ostream& operator<< (std::ostream&, CosyFloat const&);
        friend std::istream& operator>> (std::istream&, CosyFloat &);
private:
        double value_;
};
CosyFloat fabs (CosyFloat const &);
CosyFloat sqrt (CosyFloat const &);
CosyFloat pow (CosyFloat const &, CosyFloat const &);

std::ostream& operator<< (std::ostream&, CosyFloat const&);
std::istream& operator>> (std::istream&, CosyFloat &);


} // namespace cosyscene

#endif // COSYFLOAT_HH_20110210
