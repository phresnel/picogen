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

#ifndef OBJECT_TO_REDSHIFT_HH_20101014
#define OBJECT_TO_REDSHIFT_HH_20101014

// redshift fwd+Co.
namespace redshift {
        class Primitive;
        class BoundPrimitive;
}

// redshift_file fwd+Co.
namespace redshift_file {
        class Object;
}

#include "redshift/include/smart_ptr.hh"
// Historically, redshift_file::Object had two functions "toPrimitive" and
// "toBoundPrimitive", but for ease of use I wanted to name all conversion
// functions "toRedshift()". The following closure enables this.
namespace redshift_file { namespace detail {
struct RedshiftPrimitiveOrBoundPrimitive {
        operator redshift::shared_ptr<redshift::Primitive> () const;
        operator redshift::shared_ptr<redshift::BoundPrimitive> () const;
        Object const & object;
        
        RedshiftPrimitiveOrBoundPrimitive (Object const &object)
        : object(object)
        {}
};
} }

namespace redshift_file {
        detail::RedshiftPrimitiveOrBoundPrimitive toRedshift (Object const &ob);
}

#endif // OBJECT_TO_REDSHIFT_HH_20101014
