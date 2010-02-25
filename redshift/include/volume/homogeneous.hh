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

#ifndef HOMOGENEOUS_HH_INCLUDED_20100211
#define HOMOGENEOUS_HH_INCLUDED_20100211

#include "../basictypes/volume.hh"

namespace redshift { namespace volume {

        // VolumeRegion.
        DefineFinalizer(Homogeneous);

        class Homogeneous
                : public VolumeRegion
                , DoFinalize(Homogeneous)
        {
        public:
                Homogeneous (
                        Color const & sigma_a,
                        Color const & sigma_s,
                        Color const & Lve,
                        real_t henyeyGreensteinParameter
                ) ;


                Color sigma_a (const Point &p, const Vector &w) const;
                Color sigma_s (const Point &p, const Vector &w) const;

                Color Lve (const Point &p,const Vector &w) const;

                real_t p (const Point &p,
                          const Vector &w_in,const Vector &w_out
                ) const;

                Color sigma_t (const Point &p, const Vector &w) const;

                Color tau (const Ray &r, const Interval &i,
                           real_t step, real_t offset) const;

        private:
                Homogeneous ();
                Homogeneous (Homogeneous const &);
                Homogeneous& operator = (Homogeneous const &);

                const Color sigma_a_;
                const Color sigma_s_;
                const Color Lve_;
                const real_t henyeyGreensteinParameter;
        };
} }

#endif // HOMOGENEOUS_HH_INCLUDED_20100211
