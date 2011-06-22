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

#ifndef ROLLWIDGET_HH
#define ROLLWIDGET_HH

#include <QWidget>

namespace picogen { namespace qt4 {

class RollWidget : public QWidget
{
        Q_OBJECT
public:
        RollWidget(QWidget *parent = 0);

        qreal roll() const;
        void setRoll(qreal a);

signals:
        void rollEdited (qreal newRoll) const;

protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void paintEvent(QPaintEvent *);
private:
        void updateIndicators(QMouseEvent *event);
        void drawArtificialHorizon(QPainter *painter);

        int radius() const;
        QPointF center() const;

        enum UpdateIndicator {
                update_none=0, update_roll
        } updateIndicator;

private:
        qreal roll_;
};

} }

#endif // ROLLWIDGET_HH
