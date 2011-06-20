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

#ifndef TRANSFORM_HH_20101013
#define TRANSFORM_HH_20101013

#include <vector>
#include "shared_ptr.hh"
#include "actuarius/bits/enum.hh"

namespace redshift_file {
        struct Transform {
                enum Type {
                        move,
                        move_left,
                        move_right,
                        move_up,
                        move_down,
                        move_forward,
                        move_backward,

                        yaw,
                        pitch,
                        roll,

                        scale
                };

                static const actuarius::Enum<Type> Typenames;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch);

                Type type;
                double x,y,z;
                double angle;

                Transform ()
                : type(move), x(0), y(0), z(0), angle(0) {}

                Transform (Transform const &rhs)
                : type(rhs.type), x(rhs.x), y(rhs.y), z(rhs.z), angle(rhs.angle) {}

                Transform &operator = (Transform const &rhs) {
                        type = rhs.type;
                        x = rhs.x;
                        y = rhs.y;
                        z = rhs.z;
                        angle = rhs.angle;
                        return *this;
                }
        };

        class TransformList {
                std::vector<Transform> transforms;
        public:

                TransformList() {}
                TransformList(TransformList const &rhs)
                        : transforms(rhs.transforms) {}
                TransformList &operator = (TransformList const &rhs) {
                        transforms = rhs.transforms;
                        return *this;
                }

                Transform operator [] (int i) const {
                        return transforms [i];
                }

                int size () const {
                        return transforms.size();
                }

                void push_back (Transform const &t) {
                        transforms.push_back (t);
                }

                typedef std::vector<Transform>::iterator iterator;
                typedef std::vector<Transform>::const_iterator const_iterator;
                iterator       begin()       { return transforms.begin(); }
                const_iterator begin() const { return transforms.begin(); }
                iterator       end()         { return transforms.end(); }
                const_iterator end()   const { return transforms.end(); }

                template<typename Arch> void serialize (Arch &arch);
        };
}

#endif // TRANSFORM_HH_20101013
