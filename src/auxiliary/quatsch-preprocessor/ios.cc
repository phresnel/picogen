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

#include <ostream>
#include <stdexcept>
#include "ios.hh"

namespace quatsch_preprocessor {

std::ostream& operator<< (std::ostream& o, DomainScalar const& v) {
        return o << v.value();
}

std::ostream& operator<< (std::ostream& o, DomainInterval const& v) {
        return o << "[" << v.from() << " .. " << v.to() << "]";
}

std::ostream& operator<< (std::ostream& o, DomainValue const& v) {
        switch (v.type()) {
        case Scalar: return o << v.scalar();
        case Interval: return o << v.interval();
        }
        throw std::runtime_error (
        "in std::ostream& operator<< (std::ostream& o, DomainValue const& v): "
        "unhandled value"
        );
};

std::ostream& operator<< (std::ostream& o, Domain const& v) {
        typedef Domain::const_iterator iterator;
        
        bool first = true;
        for (iterator it=v.begin(), end=v.end(); it!=end; ++it) {
                if (!first) o << ", ";
                o << *it;
                first = false;
        }
        return o;
}

std::ostream& operator<< (std::ostream& o, DeclaredType const& v) {
        switch (v) {
        case Integer: return o << "integer";
        case Real:    return o << "integer";
        }
        throw std::runtime_error (
        "in std::ostream& operator<< (std::ostream& o, DeclaredType const& v): "
        "unhandled value"
        );
}

std::ostream& operator<< (std::ostream& o, Declaration const& v) {
        return o << v.id() << " : " << v.type()
                 << " = {" << v.domain() << "}";
}

}

