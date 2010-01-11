//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef PRIMITIVE_BOOLEANFIELD_H_INCLUDED_20100109
#define PRIMITIVE_BOOLEANFIELD_H_INCLUDED_20100109

namespace redshift { namespace primitive {
        
        DefineFinalizer(BooleanField);
        
        namespace impl { class BooleanFieldImpl; }
        class BooleanField
                : public Primitive                
                , DoFinalize(BooleanField)
        {
        public:
                BooleanField(shared_ptr<HeightFunction const> fun, real_t detail);
                ~BooleanField ();
                
                bool doesIntersect (RayDifferential const &ray) const;
                bool doesIntersect (Ray const &ray) const;

                optional<Intersection>
                        intersect(RayDifferential const &ray) const;
                
        private:
                BooleanField();
                BooleanField(BooleanField const&);
                BooleanField &operator = (BooleanField const&);
        
                impl::BooleanFieldImpl *impl;
        };
} }

#endif // PRIMITIVE_BOOLEANFIELD_H_INCLUDED_20100109
