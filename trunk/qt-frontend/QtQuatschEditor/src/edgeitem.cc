//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include <QtGui>
#include "edgeitem.hh"
#include <nodeitem.hh>


EdgeItem::EdgeItem (NodeItem *a, NodeItem *b)
: itemA(a), itemB(b)
{
        /*animation.setItem (this);*/
        setItem (this);
        setTimeLine(&timer);
        timer.setFrameRange(0, 1000);
        startAnimation();

        setCacheMode (QGraphicsItem::DeviceCoordinateCache);
        setPen(QPen(QColor(200,170,150), 2));
        setZValue (0.0);
}



EdgeItem::~EdgeItem() {
        itemA = itemB = 0;
}



void EdgeItem::paint (QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget
) {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        update();
        painter->setPen(this->pen());
        painter->drawLine(this->line().p1(), this->line().p2());
}



void EdgeItem::afterAnimationStep (qreal step) {
        Q_UNUSED(step)
        update();
}



bool EdgeItem::isConnectedTo (NodeItem *a) const {
        return a == this->itemA || a == this->itemB;
}



void EdgeItem::update () {
        const float
                width_a = itemA->boundingRect().width(),
                height_a = itemA->boundingRect().height(),
                height_b = itemB->boundingRect().height()
        ;
        setLine(QLineF(
                itemA->pos()+QPointF(width_a, height_a*0.5f),
                itemB->pos()+QPointF(0.0f, height_b*0.5f)
        ));
        QGraphicsLineItem::update();
}



void EdgeItem::startAnimation() {
        timer.stop();
        timer.start();
}

