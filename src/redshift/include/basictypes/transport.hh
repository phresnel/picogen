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

#ifndef TRANSPORT_HH_INCLUDED_20090110
#define TRANSPORT_HH_INCLUDED_20090110

#include "../geometry.hh"
#include "spectrum.hh"

namespace redshift {
        class Scene;
        class Sample;
        class Random;

        // Quick hack until all integrators are iterative
        class LiRecursion {
        public:
                unsigned int depth() const { return depth_; }
                friend class Scene;
        private:
                LiRecursion () : depth_(0) {}
                LiRecursion (unsigned int depth) : depth_(depth) {}
                LiRecursion operator ++ () const { return LiRecursion(depth_+1); }
                LiRecursion operator -- () const { return LiRecursion(depth_-1); }
                unsigned int depth_;
        };
}

namespace redshift {
        class Distance {
        public:
                Distance() : d(std::numeric_limits<real_t>::infinity()) {}
                explicit Distance(real_t d) : d(d) {}
                real_t toReal () const {
                        return d;
                }
        private:
                real_t d;
        };
        class DistantRadiance {
        public:
                DistantRadiance (Color const &color, Distance const &distance)
                : color_(color), distance_(distance)
                {}

                DistantRadiance (DistantRadiance const &rhs)
                : color_(rhs.color_), distance_(rhs.distance_)
                {}

                DistantRadiance& operator = (DistantRadiance const &rhs) {
                        color_ = rhs.color_;
                        distance_ = rhs.distance_;
                        return *this;
                }

                Color color() const { return color_; }
                Distance distance() const { return distance_; }
        private:
                DistantRadiance() {}

                Color color_;
                Distance distance_;
        };
}

namespace redshift {
        class Integrator {
        public:
                virtual ~Integrator() {}
                virtual DistantRadiance Li (
                        const Scene &scene,
                        const RayDifferential &raydiff,
                        const Sample &sample,
                        const LiRecursion &liRecursion,
                        Random& rand
                ) const = 0;
        };
}

#endif // TRANSPORT_HH_INCLUDED_20090110
