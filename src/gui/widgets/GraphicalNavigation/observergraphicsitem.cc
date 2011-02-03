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


ObserverGraphicsItem::ObserverGraphicsItem() :
        QGraphicsObject(),
        mouseMoveEffect(mm_do_nothing),
        autoHeightMode(KeepRelativeHeight),
        heightFunction(new ZeroHeightFunction)
{
        this->setFlags(QGraphicsItem::ItemIsMovable);
}

QRectF ObserverGraphicsItem::boundingRect() const {
        QRectF ret;
        ret.setLeft(-64);
        ret.setRight(64);
        ret.setBottom(64);
        ret.setTop(-64);
        return ret;
}


void ObserverGraphicsItem::paintViewingDirection(QPainter *painter) const {
        const QTransform prev = painter->transform();
        QTransform rot = prev;
        rot = rot.rotateRadians(observer_.yaw());

        painter->setTransform(rot);

        QPen arrowPen;
        arrowPen.setColor(QColor(255,0,0,128));
        arrowPen.setWidthF(2);
        const double y = -50;
        painter->setPen(arrowPen);
        painter->drawLine(0,0, 0,y);
        painter->drawLine(-5,y+5, 0,y);
        painter->drawLine(+5,y+5, 0,y);

        painter->setBrush(QColor(200,255,200,128));
        painter->drawPie(QRect(-64,-64,128,128), 83*16,14*16);

        painter->setTransform(prev);
}

void ObserverGraphicsItem::paintHorizon(QPainter *painter) const {
        painter->setPen(QColor(200,200,255,200));

        QRadialGradient rad (QPointF(0,0), 64, QPointF(0,0));
        rad.setColorAt(0, QColor(200,200,255,130));
        rad.setColorAt(0.6, QColor(255,200,155,130));
        rad.setColorAt(1, QColor(255,200,155,0));
        painter->setBrush(rad);
        painter->drawEllipse(-64, -64, 128,128);
}

void ObserverGraphicsItem::paintSun (QPainter *painter) const {
        const QTransform local = painter->transform();
        QTransform localNoRot = QTransform::fromTranslate(pos().x(), pos().y());
        painter->setTransform (localNoRot);

        painter->setPen(QPen(
                QColor(255,128,64,255),
                1,
                Qt::DotLine
        ));

        const QLineF sunLine (0,0, 40,40);
        painter->drawLine(sunLine);

        painter->setPen(QPen(
                QColor(255,255,100,255),
                1,
                Qt::SolidLine
        ));
        painter->setBrush(QBrush(
                QColor(255,255,100,32),
                Qt::SolidPattern
        ));
        painter->drawEllipse(sunLine.p2(),10,10);

        painter->setTransform (local);
}

void ObserverGraphicsItem::paintCenterMarker (QPainter *painter) const {
        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(200,255,200,128));
        painter->drawEllipse(-12, -12, 24,24);

        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(255,200,200,128));
        painter->drawEllipse(-1.5, -1.5, 3,3);
}

void ObserverGraphicsItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget
) {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        paintHorizon(painter);
        paintViewingDirection(painter);
        paintSun(painter);
        paintCenterMarker(painter);
}

QVector2D ObserverGraphicsItem::relateMouseToOwnPos (QPointF mouse) {
        return QVector2D(mapToScene(mouse) - scenePos());
}

void ObserverGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *p) {
        switch (mouseMoveEffect) {
        case mm_do_nothing:
                QGraphicsItem::mouseMoveEvent(p);
                break;
        case mm_change_position: {
                QGraphicsItem::mouseMoveEvent(p);
                const QPointF sp = scenePos();

                const double x=sp.x(), z=sp.y();
                double y;
                switch (autoHeightMode) {
                case KeepRelativeHeight:
                        y = heightFunction->height(x,z) + heightAboveGroundAtStartMove;
                        break;
                case KeepAbsoluteHeight:
                        y = observer_.position().y();
                        break;
                }

                observer_.setPosition(x, y, z);
                emit positionChanged(observer_.position());
                break;
        }
        case mm_change_yaw: {
                const QVector2D v = relateMouseToOwnPos(p->pos());
                observer_.setYaw(std::atan2(v.x(),-v.y()));
                emit yawChanged(observer_.yaw());
                update();
                break;
        }
        }
}

void ObserverGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *p) {
        const QVector2D v = relateMouseToOwnPos(p->pos());
        if (v.length()>12) {
                mouseMoveEffect = mm_change_yaw;
        } else {
                mouseMoveEffect = mm_change_position;
                heightAboveGroundAtStartMove =
                        observer_.position().y() -
                        heightFunction->height(observer_.position().x(),
                                               observer_.position().z());
        }
        QGraphicsItem::mousePressEvent(p);
}

void ObserverGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        mouseMoveEffect = mm_do_nothing;
        QGraphicsItem::mouseReleaseEvent(event);
}

void ObserverGraphicsItem::setAutoHeightMode(AutoHeightMode ahm) {
        autoHeightMode = ahm;
}

void ObserverGraphicsItem::setHeightFunction(HeightFunction::Ptr hf) {
        heightFunction = hf;
}
