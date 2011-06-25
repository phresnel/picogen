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

#include "observer.h"

namespace picogen { namespace qt4_gui {

Observer::Observer() {
}

QVector3D Observer::position() const {
        return position_;
}

void Observer::setPosition(QVector3D value) {
        position_ = value;
}

void Observer::setPosition(qreal x, qreal y, qreal z) {
        setPosition(QVector3D(x,y,z));
}

qreal Observer::yaw() const {
        return yaw_;
}

void Observer::setYaw(qreal value) {
        yaw_ = value;
}

qreal Observer::pitch() const {
        return pitch_;
}

void Observer::setPitch(qreal value) {
        pitch_ = value;
}

qreal Observer::roll() const {
        return roll_;
}

void Observer::setRoll(qreal value) {
        roll_ = value;
}

} }
