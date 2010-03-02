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

#include "../../include/volume/list.hh"


namespace redshift { namespace volume {



List::List () {
}



void List::add (shared_ptr<VolumeRegion> volume) {
        volumes.push_back (volume);
}



// absorption
Color List::sigma_a (const Point &p, const Vector &w, Random& rand) const {
        Color ret = Color::fromRgb(0,0,0);
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->sigma_a (p, w, rand);
        }
        return ret;
}



// out scattering probability
Color List::sigma_s (const Point &p, const Vector &w, Random& rand) const {
        Color ret = Color::fromRgb(0,0,0);
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->sigma_s (p, w, rand);
        }
        return ret;
}



// emission
Color List::Lve (const Point &p,const Vector &w, Random& rand) const {
        Color ret = Color::fromRgb(0,0,0);
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->Lve (p, w, rand);
        }
        return ret;
}



real_t List::p (
        const Point &p,
        const Vector &w_in,
        const Vector &w_out, Random& rand
) const {
        real_t ret = 0;
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->p(p, w_in, w_out, rand);
        }
        return ret;
}



Color List::sigma_t (const Point &p, const Vector &w, Random& rand) const {
        Color ret = Color::fromRgb(0,0,0);
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->sigma_t (p, w, rand);
        }
        return ret;
}



Color List::tau (
        const Ray &r, const Interval &i,
        real_t step, real_t offset, Random& rand
) const {
        Color ret = Color::fromRgb(0,0,0);
        for (const_iterator it = volumes.begin(); it!=volumes.end(); ++it) {
                ret = ret + (*it)->tau (r, i, step, offset, rand);
        }
        return ret;
}



} }

