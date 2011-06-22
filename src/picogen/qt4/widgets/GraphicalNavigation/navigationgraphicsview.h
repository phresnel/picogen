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

#ifndef NAVIGATIONGRAPHICSVIEW_HH
#define NAVIGATIONGRAPHICSVIEW_HH

#include <QGraphicsView>
#include "heightfunction.h"

namespace picogen { namespace qt4 {

class NavigationGraphicsView : public QGraphicsView
{
        Q_OBJECT
public:
        explicit NavigationGraphicsView(QWidget *parent = 0);
        void setHeightFunction (HeightFunction::Ptr);
        void setWaterLevel (qreal wl);

        void setPixelSize (int);

protected:
        void drawBackground(QPainter *painter, const QRectF &rect);
        void drawForeground(QPainter *painter, const QRectF &rect);
        //void paintEvent(QPaintEvent *event);

        //void dragMoveEvent(QDragMoveEvent *event);
        void scrollContentsBy(int dx, int dy);

private:
        HeightFunction::Ptr heightFunction;
        qreal hmin, hmax, hirange;
        qreal waterLevel;

        bool needsMaximaUpdate_;

        QPointF dragBeginPos;
        int pixelSize_;

        void guessMaxima();
        void drawRaster(QPainter *painter, const QRectF &rectf, qreal boxSize);
};

} }

#endif // NAVIGATIONGRAPHICSVIEW_HH
