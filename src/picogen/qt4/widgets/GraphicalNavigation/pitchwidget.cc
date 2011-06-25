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

#include "pitchwidget.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

#include <QMouseEvent>

#include <QVector2D>
#include <QPointF>

#include <cmath>

namespace picogen { namespace qt4_gui {

PitchWidget::PitchWidget(QWidget *parent)
        : QWidget(parent)
{
}

qreal PitchWidget::pitch() const {
        return pitch_;
}
void PitchWidget::setPitch(qreal a) {
        pitch_ = a;
        repaint();
}

QRect PitchWidget::actionRect() const {
        QRect ret = rect();
        ret.adjust(2,2,-2,-2);
        return ret;
}

void PitchWidget::drawArtificialHorizon(QPainter *painter) {

        const QRect actionRect = this->actionRect();

        const int top = actionRect.top(),
                  bottom = actionRect.bottom(),
                  height = actionRect.height(),
                  widgetCenter = actionRect.center().y();
        const int left = actionRect.left(),
                  width = actionRect.width(),
                  right = actionRect.right(),
                  widgetCenter_x = actionRect.center().x(),
                  half_left = (left+widgetCenter_x)/2,
                  half_right = (right+widgetCenter_x)/2;

        const qreal pi = 3.14159, pi05 = pi*0.5, pi025 = pi*0.25;

        const qreal horizon     = top + (0.5+pitch_/3.14159)*height,
                    horizon_a45 = top + (0.5+(pitch_+pi025)/3.14159)*height,
                    horizon_s45 = top + (0.5+(pitch_-pi025)/3.14159)*height,
                    horizon_a90 = top + (0.5+(pitch_+pi05)/3.14159)*height,
                    horizon_s90 = top + (0.5+(pitch_-pi05)/3.14159)*height
                    ;

        // Draw sky.
        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,150,255,255));
        painter->drawRoundedRect(left, top, width, horizon-top,
                                 5, 5);

        // Draw bottom.
        painter->setPen(QPen (QColor(0,0,0,0), 1));
        painter->setBrush(QColor(150,255,155,255));
        painter->drawRoundedRect(left, horizon, width, bottom-horizon,
                                 5, 5);

        // Draw horizon.
        painter->setPen(QPen (QColor(0,0,0,255), 1));
        painter->drawLine(left, horizon, right, horizon);

        painter->setPen(QPen (QColor(0,122,0,255), 1));

        painter->drawLine(half_left, horizon_a45, half_right, horizon_a45);
        painter->drawLine(half_left, horizon_s45, half_right, horizon_s45);
        painter->drawLine(half_left, horizon_a90, half_right, horizon_a90);
        painter->drawLine(half_left, horizon_s90, half_right, horizon_s90);

        painter->drawText(QRect(half_left-20, horizon_a45-8, 20, 16),
                          "-45", QTextOption(Qt::AlignVCenter|Qt::AlignRight));
        painter->drawText(QRect(half_left-20, horizon_s45-8, 20, 16),
                          "45", QTextOption(Qt::AlignVCenter|Qt::AlignRight));
        painter->drawText(QRect(half_left-20, horizon_a90-8, 20, 16),
                          "-90", QTextOption(Qt::AlignVCenter|Qt::AlignRight));
        painter->drawText(QRect(half_left-20, horizon_s90-8, 20, 16),
                          "90", QTextOption(Qt::AlignVCenter|Qt::AlignRight));

        // Draw observer horizon.
        painter->setPen(QPen (QColor(0,0,0,255), 1, Qt::DashLine));
        painter->drawLine(left, widgetCenter, right, widgetCenter);
        painter->drawText(QRect(0, widgetCenter, width, 16),
                          QString::number(pitch_/0.0174532925, 'f', 1) + "",
                          QTextOption(Qt::AlignCenter));

        // Draw widget border
        painter->setPen(QPen(QColor(0,0,0,255), 2));
        painter->setBrush(QBrush(QColor(255,255,255,0)));

        QRect tmp = rect();
        tmp.adjust(2,2,-2,-2);
        painter->drawRoundedRect(tmp, 5, 5);
}

void PitchWidget::paintEvent(QPaintEvent *) {
        QPainter painter (this);
        painter.setRenderHints(QPainter::TextAntialiasing |
                               QPainter::Antialiasing);
        drawArtificialHorizon(&painter);
}


void PitchWidget::mousePressEvent(QMouseEvent *event) {
        updateIndicator = update_pitch;
        updateIndicators(event);
        event->accept();
}

void PitchWidget::mouseMoveEvent(QMouseEvent *event) {
        updateIndicators(event);
        event->accept();
}

void PitchWidget::mouseReleaseEvent(QMouseEvent *event) {
        updateIndicators(event);
        updateIndicator = update_none;
        event->accept();
}

void PitchWidget::updateIndicators(QMouseEvent *event) {
        switch (updateIndicator) {
        case update_none: break;
        case update_pitch: {
                const QRect r = this->actionRect();
                const qreal f = ((event->pos().y()-r.top())
                                / (qreal)r.height());
                pitch_ = (-0.5 + f);
                if (pitch_<-0.5) pitch_ = -0.5;
                if (pitch_>0.5) pitch_ = 0.5;
                pitch_ *= 3.14159;
                emit pitchEdited(pitch_);
                break;
        }
        }

        repaint();
}

} }

#include "pitchwidget.moc"
