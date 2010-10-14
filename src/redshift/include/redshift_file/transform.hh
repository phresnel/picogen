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
                        roll
                };

                static const actuarius::Enum<Type> Typenames;

                redshift::Transform toRedshiftTransform () const {
                        using redshift::Transform;
                        const double to_radians = redshift::constants::pi/180;
                        switch (type) {
                        case move: return Transform::translation(x,y,z);
                        case move_left:
                        case move_right: return Transform::translation(x,0,0);
                        case move_up:
                        case move_down: return Transform::translation(0,y,0);
                        case move_forward:
                        case move_backward:  return Transform::translation(0,0,z);

                        case yaw:   return Transform::rotationY(angle*to_radians);
                        case pitch: return Transform::rotationX(angle*to_radians);
                        case roll:  return Transform::rotationZ(angle*to_radians);
                        };
                        return redshift::Transform();
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        /*
                        switch (type) {
                        case move:
                                arch & pack(x) & pack(y) & pack(z);
                                break;
                        case move_right: arch & pack(x);            break;
                        case move_left:  arch & pack(x); x = -x;    break;
                        case move_up:    arch & pack(y);            break;
                        case move_down:  arch & pack(y);  y = -y;   break;
                        case move_forward: arch & pack(z);          break;
                        case move_backward:
                                if (Arch::serialize)
                                arch & pack(z); z = -z; break;
                        case yaw:
                        case pitch:
                        case roll:
                                arch & pack(angle);
                                break;
                        };*/

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
                        };

                        if (Arch::deserialize) {
                                x = -left     + right;
                                y = -down     + up;
                                z = -backward + forward;
                        }
                }

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

                redshift::Transform toRedshiftTransform () const {
                        typedef std::vector<Transform>::const_iterator iterator;
                        redshift::Transform ret;
                        for (iterator it = transforms.begin(); it!=transforms.end(); ++it) {
                                ret = ret * it->toRedshiftTransform();
                        }
                        return ret;
                }

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack (&Transform::type, Transform::Typenames, transforms);
                }
        };
}

#endif // TRANSFORM_HH_20101013
