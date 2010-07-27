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

#ifndef PARAMETER_HH_INCLUDED_20100726
#define PARAMETER_HH_INCLUDED_20100726

#include <string>

class Parameter {
public:
        enum Type {
                Identifier,
                Integer,
                Real,
                ParameterIndex // never parsed
        };

        Type type() const;
        void setType (Type type);

        void setInteger (int v);
        void setReal (double v);
        void setIdentifier (std::string v);

        int integer () const;
        double real () const;
        std::string identifier () const;

        void toParameterIndex (int index);
        int parameterIndex() const;
private:
        Type type_;
        int intval;
        int index;
        double realval;
        std::string idval;
};

#endif // PARAMETER_HH_INCLUDED_20100726
