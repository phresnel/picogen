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

#ifndef EXPONENTIAL_HH_INCLUDED_20100213
#define EXPONENTIAL_HH_INCLUDED_20100213

#include "../basictypes/volume.hh"
#include "../sealed.hh"

namespace redshift { namespace volume {

        // VolumeRegion.
        SEALED(Exponential);

        class Exponential
                : public DensityRegion
                , MAKE_SEALED(Exponential)
        {
        public:
                Exponential (
                        Color const & sigma_a,
                        Color const & sigma_s,
                        Color const & Lve,
                        real_t henyeyGreensteinParameter,
                        real_t baseFactor,
                        real_t exponentFactor,
                        Point  const & min,
                        Vector const & up,
                        real_t epsilon
                );

                real_t density(const Point &p, Random& rand) const;

                Interval cull (const Ray &ray) const ;

        private:
                Exponential ();
                Exponential (Exponential const &);
                Exponential& operator = (Exponential const &);

                real_t density (real_t h) const;

                const real_t baseFactor, exponentFactor;
                const Point min;
                const Vector up;
                const Plane upPlane;

                Plane approximateUpperPlane (real_t epsilon) const ;
        };
} }

#endif // EXPONENTIAL_HH_INCLUDED_20100213
