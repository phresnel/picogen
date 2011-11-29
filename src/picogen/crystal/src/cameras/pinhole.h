//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#ifndef PINHOLE_H
#define PINHOLE_H

#include "../camera.h"

namespace crystal { namespace cameras {
        class Pinhole : public Camera  {
        public:
                Pinhole (real frontPlaneDist) : frontPlaneDist_(frontPlaneDist)
                {}
        private:
                Ray gen (CameraSample const &sample) const
                {
                        const real u = 2*sample.lensU-1,
                                   v = 1-2*sample.lensV;
                        return Ray (Point(0, 150, 0), Direction(u, v, frontPlaneDist_));
                                    //Point(-2500, 2500, -2500), Direction(u, -frontPlaneDist_, v));
                }

                real frontPlaneDist_;
        };
} }

#endif // PINHOLE_H
