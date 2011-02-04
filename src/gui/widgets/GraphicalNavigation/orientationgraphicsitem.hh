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

#ifndef ORIENTATIONGRAPHICSITEM_HH
#define ORIENTATIONGRAPHICSITEM_HH

#include <QGraphicsObject>

class OrientationGraphicsItem : public QGraphicsObject
{
        Q_OBJECT
public:
        OrientationGraphicsItem();
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
        void updateIndicators(QGraphicsSceneMouseEvent *event);
        static void drawArtificialHorizon(QPointF center, qreal pitch, QPainter *painter);

        enum UpdateIndicator {
                update_none=0, update_pitch, update_roll
        } updateIndicator;

private:
        qreal roll_;
};

#endif // ORIENTATIONGRAPHICSITEM_HH
