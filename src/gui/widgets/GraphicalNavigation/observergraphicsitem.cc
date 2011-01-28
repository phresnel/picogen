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

#include "observergraphicsitem.hh"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <cmath>


ObserverGraphicsItem::ObserverGraphicsItem()
{
        this->setFlags(QGraphicsItem::ItemIsMovable);
}

QRectF ObserverGraphicsItem::boundingRect() const {
        QRectF ret;
        ret.setLeft(-12);
        ret.setRight(12);
        ret.setBottom(24);
        ret.setTop(-50);
        return ret;
}

void ObserverGraphicsItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget
) {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        // Center
        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(200,255,200,128));
        painter->drawEllipse(-12, -12, 24,24);

        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(255,200,200,128));
        painter->drawEllipse(-1.5, -1.5, 3,3);

        // Viewing direction
        QPen arrowPen;
        arrowPen.setColor(QColor(255,0,0,128));
        arrowPen.setWidthF(2);
        const double y = -50;
        painter->setPen(arrowPen);
        painter->drawLine(0,0, 0,y);
        painter->drawLine(-5,y+5, 0,y);
        painter->drawLine(+5,y+5, 0,y);
}


bool ObserverGraphicsItem::tryRotateByMouse (QPointF local) {
        const QPointF v = mapToScene(local) - scenePos();
        const double x = v.x();
        const double y = -v.y();
        const double s = std::sqrt(x*x + y*y);
        if (s>12 && std::fabs(local.x()) < 5) {
                // rotate
                setRotation(std::atan2(x,y) * 180 / 3.14159);
                return true;
        } else {
                // move
                return false;
        }
}

void ObserverGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *p) {
        if (!tryRotateByMouse(p->pos())) {
                // move
                QGraphicsItem::mouseMoveEvent(p);
        }
}

void ObserverGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *p) {
        if (!tryRotateByMouse(p->pos())) {
                // move
                QGraphicsItem::mousePressEvent(p);
        }
}
