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

#ifndef OBSERVERGRAPHICSITEM_HH
#define OBSERVERGRAPHICSITEM_HH

#include <QGraphicsObject>
#include <QVector2D>
#include "heightfunction.h"
#include "observer.h"

namespace picogen { namespace qt4 {

enum AutoHeightMode {
        KeepAbsoluteHeight,
        KeepRelativeHeight
};

class ObserverGraphicsItem : public QGraphicsObject
{
        Q_OBJECT
public:
        ObserverGraphicsItem();

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

        void setAutoHeightMode (AutoHeightMode);
        void setHeightFunction (HeightFunction::Ptr);

        void setObserverAbsolutePosition (QVector3D pos);
        void setObserverAbsolutePosition (qreal, qreal, qreal);
        void setObserverAbsoluteHeight (double);
        void setObserverRelativeHeight (double);
        void setObserverEast (double);
        void setObserverNorth (double);

        void setObserverYaw (double);
        void setObserverPitch (double);
        void setObserverRoll (double);

        QVector3D observerPosition() const;
        qreal observerYaw() const;
        qreal observerPitch() const;
        qreal observerRoll() const;

signals:
        void positionChanged (QVector3D newPosition);
        void orientationChanged (qreal newYaw, qreal newPitch, qreal newRoll);

private:
        QVector2D relateMouseToOwnPos (QPointF mouse);

        void paintViewingDirection(QPainter *painter) const;
        void paintHorizon(QPainter *painter) const;
        void paintSun(QPainter *painter) const;
        void paintCenterMarker (QPainter *painter) const;

private:
        Observer observer_;

protected:
        enum MouseMoveEffect {
                mm_do_nothing = 0,
                mm_change_position,
                mm_change_yaw
        };
        MouseMoveEffect mouseMoveEffect;
        AutoHeightMode autoHeightMode;
        HeightFunction::Ptr heightFunction;

        double heightAboveGroundAtStartMove;

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

} }

#endif // OBSERVERGRAPHICSITEM_HH
