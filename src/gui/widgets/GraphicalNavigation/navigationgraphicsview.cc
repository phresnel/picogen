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

#include "navigationgraphicsview.hh"
#include <limits>
#include <QMouseEvent>

NavigationGraphicsView::NavigationGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    waterLevel(-1000000)
{
        setCacheMode(QGraphicsView::CacheBackground);
        setDragMode(QGraphicsView::ScrollHandDrag);

        heightFunction.reset(new ZeroHeightFunction);
        guessMaxima();
}

void NavigationGraphicsView::setHeightFunction (HeightFunction::Ptr f) {
        heightFunction = f;
        guessMaxima();
        repaint();
}

void NavigationGraphicsView::guessMaxima() {
        int i=0;
        hmin = std::numeric_limits<float>::infinity();
        hmax = -std::numeric_limits<float>::infinity();

        qsrand(0);
        const qreal r = 100000, r05 = r / 2;
        while (++i<32768) {
                const qreal u = (qrand() / (qreal)RAND_MAX) * r - r05,
                            v = (qrand() / (qreal)RAND_MAX) * r - r05,
                            h = heightFunction->height(u,v);
                if (h<hmin) hmin = h;
                if (h>hmax) hmax = h;
        }
        if (hmax > hmin)
                hirange = 1 / (hmax - hmin);
        else
                hirange = 0;
}

void NavigationGraphicsView::setWaterLevel(qreal wl) {
        waterLevel = wl;
        repaint();
}

void NavigationGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {
        const QRectF r = painter->worldTransform().mapRect(rect);
        QImage img(r.width(),
                   r.height(),
                   QImage::Format_RGB32);
        const int height=img.height(), width=img.width();

        for (int y=0; y<height; ++y) {
                const double v_ = y / (double)height,
                             v = v_ * rect.height() + rect.top();
                QRgb *sl = (QRgb*)img.scanLine(y);
                for (int x=0; x<width; ++x) {
                        const double u_ = x / (double)width,
                                     u = u_*rect.width() + rect.left();

                        const double h_ = heightFunction->height(u,-v),
                                     hn_ = (h_-hmin) * hirange,
                                     h = hn_ * 255;
                        const int hi = h < 0 ? 0 : h > 255 ? 255 : h;

                        // Blue is the water.
                        if (h_ < waterLevel)
                                sl[x] = QColor(122,122,255).rgb();
                        else
                                sl[x] = QColor(hi,hi,hi).rgb();
                }
        }
        painter->drawImage(rect, img);
}

