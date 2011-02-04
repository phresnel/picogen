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

#include "orientationgraphicsitem.hh"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QVector2D>

#include <cmath>

OrientationGraphicsItem::OrientationGraphicsItem()
        : QGraphicsObject()
{
        setFlags (QGraphicsItem::ItemIsMovable);
}

QRectF OrientationGraphicsItem::boundingRect() const {
        QRectF ret;
        ret.setLeft(-150);
        ret.setRight(150);
        ret.setBottom(150);
        ret.setTop(-150);
        return ret;
}

void OrientationGraphicsItem::drawArtificialHorizon(QPointF center,
                                                    qreal pitch,
                                                    QPainter *painter
) {
        const int radius = 48;
        const qreal pitchDegree = pitch/0.0174532925,
                    pitchQtAngle = pitchDegree*16;

        // Absolute.
        //-----------
        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,150,255,128));
        painter->drawPie (center.x()-radius, center.y()-radius,
                          radius*2,radius*2,
                          pitchQtAngle + 0*16, 180*16);

        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,255,150,128));
        painter->drawPie (center.x()-radius, center.y()-radius,
                          radius*2,radius*2,
                          pitchQtAngle + 180*16, 180*16);

        // draw up-indicator
        const QPointF up = QPointF(-std::sin(pitch),
                                   -std::cos(pitch));
        painter->setPen(QPen (QColor(0,0,0,255), 1, Qt::DashLine));
        painter->drawLine (center,
                           center+radius*up);

        const QPointF tpos = center + (radius+20)*up;
        painter->drawText(QRectF(tpos.x()-20, tpos.y()-20, 40, 40),
                          QString::number(-pitchDegree<0 ?
                                          360-pitchDegree :
                                          -pitchDegree,
                                          'f', 1) + "°",
                          QTextOption(Qt::AlignCenter));

        // Observer.
        //-----------

        // draw observer's horizon
        painter->setPen(QPen(QColor(0,0,0,255), 1));
        painter->setBrush(QBrush(QColor(0,0,0,255)));
        painter->drawEllipse(center,2,2);

        // draw central dot
        painter->setPen(QPen(QColor(0,0,0,255), 1, Qt::DotLine));
        painter->drawLine(center-QPointF(radius,0),
                          center+QPointF(radius,0));


        // draw 45° indicators
        painter->setPen(QPen(QColor(0,0,0,255), 1, Qt::SolidLine));
        for (int a=0; a<360; a+=45) {
                const qreal rad = a * 0.0174532925;
                const QPointF dir = QPointF(std::sin(rad),-std::cos(rad));

                // draw north a bit bigger
                const int bigger = (a==0) ? 15 : 5;
                painter->drawLine(
                        center+radius*dir,
                        center+(radius+bigger)*dir
                );
        }
}

void OrientationGraphicsItem::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget
) {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        const QPointF
              pitchCenter = QPointF(32,0),
              rollCenter = QPointF(-64,0);

        drawArtificialHorizon(rollCenter, roll_, painter);
}


void OrientationGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        const QPointF
                        pitchCenter = QPointF(32,0),
                        rollCenter = QPointF(-64,0);
        const QVector2D
              pitchDiff(mapToScene(event->scenePos() - pitchCenter)),
              rollDiff(mapToScene(event->scenePos() - rollCenter));

        if (rollDiff.length() < 64) {
                updateIndicator = update_roll;
        } else {
                updateIndicator = update_none;
        }
        updateIndicators(event);
        //event->accept();
        //grabMouse();

        QGraphicsObject::mousePressEvent(event);
}

void OrientationGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        updateIndicators(event);
        event->accept();
        //QGraphicsObject::mouseMoveEvent(event);
}

void OrientationGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        updateIndicators(event);
        updateIndicator = update_none;
        //event->accept();
        //ungrabMouse();

        QGraphicsObject::mouseReleaseEvent(event);
}

void OrientationGraphicsItem::updateIndicators(QGraphicsSceneMouseEvent *event) {
        const QPointF
              pitchCenter = QPointF(32,0),
              rollCenter = QPointF(-64,0);
        const QVector2D
              pitchDiff(mapToScene(event->scenePos() - pitchCenter)),
              rollDiff(mapToScene(event->scenePos() - rollCenter));

        switch (updateIndicator) {
        case update_none: break;
        case update_roll:
                roll_ = std::atan2(-rollDiff.x(),-rollDiff.y());
                break;
        case update_pitch:
                break;
        }

        update();
}
