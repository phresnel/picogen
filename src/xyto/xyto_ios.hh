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

#ifndef XYTO_IOS_HH_INCLUDED_20100726
#define XYTO_IOS_HH_INCLUDED_20100726

inline std::ostream& operator<< (std::ostream& o, Parameter const& rhs) {
        switch (rhs.type()) {
        case Parameter::Identifier:
                o << "$" << rhs.identifier() /*<< ":id"*/;
                break;
        case Parameter::Integer:
                o << "i" << rhs.integer() /*<< ":int"*/;
                break;
        case Parameter::Real:
                o << "r" << rhs.real()  /*<< ":real"*/;
                break;
        case Parameter::ParameterIndex:
                o << "#" << rhs.parameterIndex() /*<< ":pidx"*/;
                break;
        }
        return o;
}

inline std::ostream& operator<< (std::ostream& o, ParameterList const& rhs) {
        if (rhs.size()) {
                o << "(";
                for (unsigned int i=0; i<rhs.size(); ++i) {
                        if (i) o << ", ";
                        o << rhs[i];
                }
                o << ")";
        }
        return o;
}


inline std::ostream& operator<< (std::ostream& o, Symbol const& rhs) {
        o << rhs.name();
        o << rhs.parameterList();
        return o;
}

inline std::ostream& operator<< (std::ostream& o, Pattern const& rhs) {
        if (rhs.size()) {
                o << rhs[0];
        }
        for (unsigned int i=1; i<rhs.size(); ++i) {
                o << " " << rhs[i];
        }
        return o;
}

inline std::ostream& operator<< (std::ostream& o, ProductionHeader const& rhs){
        o << rhs.name() << ": ";

        if (!rhs.leftContext().empty())
                o << rhs.leftContext() << " < ";

        o << rhs.pattern();

        if (!rhs.rightContext().empty())
                o << " > " << rhs.rightContext();
        return o;
}

inline std::ostream& operator<< (std::ostream& o, ProductionBody const& rhs) {
        o << rhs.pattern();
        return o;
}

inline std::ostream& operator<< (std::ostream& o, Production const& rhs) {
        o << rhs.header() << " --> " << rhs.body() << ";";
        return o;
}

#endif // XYTO_IOS_HH_INCLUDED_20100726
