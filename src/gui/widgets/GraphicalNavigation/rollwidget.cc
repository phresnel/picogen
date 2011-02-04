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

#include "rollwidget.hh"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

#include <QMouseEvent>

#include <QVector2D>
#include <QPointF>

#include <cmath>

RollWidget::RollWidget(QWidget *parent)
        : QWidget(parent)
{
}

int RollWidget::radius() const {
        return rect().width()<rect().height() ?
               rect().width()/2-30 :
               rect().height()/2-30;
}

QPointF RollWidget::center() const {
        return rect().center();
}

void RollWidget::drawArtificialHorizon(QPainter *painter) {
        const QPointF center = this->center();
        const int radius = this->radius();
        const qreal rollDegree = roll_/0.0174532925,
                    rollQtAngle = rollDegree*16;

        // Absolute.
        //-----------
        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,150,255,255));
        painter->drawPie (center.x()-radius, center.y()-radius,
                          radius*2,radius*2,
                          rollQtAngle + 0*16, 180*16);

        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,255,150,255));
        painter->drawPie (center.x()-radius, center.y()-radius,
                          radius*2,radius*2,
                          rollQtAngle + 180*16, 180*16);

        // draw up-indicator
        const QPointF up = QPointF(-std::sin(roll_),
                                   -std::cos(roll_));
        painter->setPen(QPen (QColor(0,0,0,255), 1, Qt::DashLine));
        painter->drawLine (center,
                           center+radius*up);

        const QPointF tpos = center + (radius+20)*up;
        painter->drawText(QRectF(tpos.x()-20, tpos.y()-20, 40, 40),
                          QString::number(-rollDegree<0 ?
                                          360-rollDegree :
                                          -rollDegree,
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

void RollWidget::paintEvent(QPaintEvent *) {
        QPainter painter (this);
        painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing);
        painter.setPen(QPen(QColor(0,0,0,255), 2));
        painter.setBrush(QBrush(QColor(255,255,255,255)));

        QRect tmp = rect();
        tmp.adjust(2,2,-2,-2);
        painter.drawRoundedRect(tmp, 5, 5);
        drawArtificialHorizon(&painter);
}


void RollWidget::mousePressEvent(QMouseEvent *event) {
        updateIndicator = update_roll;
        updateIndicators(event);
        event->accept();
}

void RollWidget::mouseMoveEvent(QMouseEvent *event) {
        updateIndicators(event);
        event->accept();
}

void RollWidget::mouseReleaseEvent(QMouseEvent *event) {
        updateIndicators(event);
        updateIndicator = update_none;
        event->accept();
}

void RollWidget::updateIndicators(QMouseEvent *event) {
        const QVector2D diff (event->pos()-center());
        switch (updateIndicator) {
        case update_none: break;
        case update_roll:
                roll_ = std::atan2(-diff.x(),-diff.y());
                break;
        }
        //update();
        repaint();
}
