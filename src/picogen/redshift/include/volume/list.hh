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

#ifndef VOLUME_LIST_HH_INCLUDED_20100228
#define VOLUME_LIST_HH_INCLUDED_20100228

#include "../basictypes/volume.hh"
#include "../sealed.hh"
#include "../smart_ptr.hh"

namespace picogen { namespace redshift { namespace volume {

        // VolumeRegion.
        SEALED(List);

        class List
                : public VolumeRegion
                , MAKE_SEALED(List)
        {
        public:
                List () ;

                void add (shared_ptr<VolumeRegion> volume) ;

                Interval cull (const Ray &ray) const ;

                Color sigma_a (const Point &p, const Vector &w, Random& rand) const;
                Color sigma_s (const Point &p, const Vector &w, Random& rand) const;

                Color Lve (const Point &p,const Vector &w, Random& rand) const;

                real_t p (const Point &p,
                          const Vector &w_in,const Vector &w_out, Random& rand
                ) const;

                Color sigma_t (const Point &p, const Vector &w, Random& rand) const;

                Color tau (const Ray &r, const Interval &i,
                           real_t step, real_t offset, Random& rand) const;

        private:
                List (List const &);
                List& operator = (List const &);

                std::vector<shared_ptr<VolumeRegion> > volumes;
                typedef std::vector<shared_ptr<VolumeRegion> >::const_iterator const_iterator;
        };
} } }

#endif // VOLUME_LIST_HH_INCLUDED_20100228
