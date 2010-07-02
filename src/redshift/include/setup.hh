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

#ifndef SETUP_HH_INCLUDED_20090303
#define SETUP_HH_INCLUDED_20090303

#include <utility>

#include "meta/compiler.hh"

#define DefineFinalizer(x) \
        class x##Finalize { \
        private: \
                friend class x ; \
                x##Finalize() {} \
                x##Finalize(x##Finalize const &) {} \
        }
#define DoFinalize(x) virtual x##Finalize

namespace redshift { class Random; }

#include "redshift-kallisto.hh"
namespace redshift {

        using std::pair;
        using std::make_pair;

        // Should be in kallisto.
        class Interval {
        public:
                Interval (real_t min, real_t max)
                : min_(min), max_(max)
                {
                        assert_valid();
                }

                Interval (Interval const &rhs)
                : min_(rhs.min_), max_(rhs.max_) {}

                Interval & operator = (Interval const &rhs) {
                        min_ = rhs.min_;
                        max_ = rhs.max_;
                        return *this;
                }

                void setMin (real_t min) { min_ = min; assert_valid(); }
                void setMax (real_t max) { max_ = max; assert_valid(); }
                real_t min() const { return min_; }
                real_t max() const { return max_; }
                real_t mag() const { return max_-min_; }
        private:
                void assert_valid() {}
                real_t min_, max_;
        };
}

#include "auxiliary/mutex.hh"
#include "auxiliary/scopedlock.hh"

#include "basictypes/rgb.hh"


#include "constants.hh"


#include "smart_ptr.hh"
#include "tuple.hh"
#include "optional.hh"

#include "static_init.hh"

#endif // SETUP_HH_INCLUDED_20090303
