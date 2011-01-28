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

ObserverGraphicsItem::ObserverGraphicsItem()
{
        this->setFlags(QGraphicsItem::ItemIsMovable);
}

QRectF ObserverGraphicsItem::boundingRect() const {
        const double cx = 0, cy=0;
        return QRectF(
                cx+-12, cy+-12,
                24, 24
        );
}

void ObserverGraphicsItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget
) {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        const double cx = 0, cy=0;

        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(200,255,200,128));
        painter->drawEllipse(cx-12, cy-12, 24,24);

        painter->setPen(QColor(0,0,0,128));
        painter->setBrush(QColor(255,200,200,128));
        painter->drawEllipse(cx-1.5, cy-1.5, 3,3);
}
