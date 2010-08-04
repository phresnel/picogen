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

#ifndef CONSTANT_HH_INCLUDED_20100803
#define CONSTANT_HH_INCLUDED_20100803

#include <string>

class Constant {
public:
        enum Type { Integer, Real };

        std::string name() const { return name_; }
        void setName (std::string const &name) { name_ = name; }

        Type type () const { return type_; }
        void setType (Type type) { type_ = type; }

        void setReal(double f) { real_ = f; }
        void setInteger(int f) { integer_ = f; }

        double toReal () const {
                switch (type_) {
                case Real: return real_;
                case Integer: return integer_;
                }
        }

        double toInteger () const {
                switch (type_) {
                case Real: return real_;
                case Integer: return integer_;
                }
        }

private:
        std::string name_;
        Type type_;
        int integer_;
        double real_;
};

#endif // CONSTANT_HH_INCLUDED_20100803
