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

#ifndef TRANSFORM_SER_HH_20101013
#define TRANSFORM_SER_HH_20101013

#include "../transform.hh"

namespace redshift_file {
        template<typename Arch>
        void Transform::serialize (Arch &arch) {
                using actuarius::pack;

                double left=0, right=0, up=0, down=0, forward=0, backward=0;

                if (Arch::serialize) {
                        right = x;   left = -x;
                        up = y;      down = -y;
                        forward = z; backward = -z;
                }

                switch (type) {
                case move:
                        arch & pack(right) & pack(up) & pack(forward);
                        break;
                case move_right:    arch & pack(right); break;
                case move_left:     arch & pack(left); break;
                case move_up:       arch & pack(up); break;
                case move_down:     arch & pack(down); break;
                case move_forward:  arch & pack(forward); break;
                case move_backward:  arch & pack(backward); break;
                case yaw:
                case pitch:
                case roll:
                        arch & pack(angle);
                        break;
                case scale:
                        // okay, it should be more like pack(x|y|z),
                        // but surrounding legacy made me use right|up|forward
                        arch & pack(right) & pack(up) & pack(forward);
                        break;
                };

                if (Arch::deserialize) {
                        x = -left     + right;
                        y = -down     + up;
                        z = -backward + forward;
                }
        }


        template<typename Arch>
        void TransformList::serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack (&Transform::type, Transform::Typenames, transforms);
        }
}

#endif // TRANSFORM_SER_HH_20101013
