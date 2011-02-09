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
#include <cmath>
#include <QMouseEvent>
#include <QRect>

namespace {
        QRect alignRect (QRectF rectf, int step) {
                QRect rect = rectf.toAlignedRect();
                rect.setLeft  ((rect.left()   / step) * step - step);
                rect.setRight ((rect.right()  / step) * step + step);
                rect.setTop   ((rect.top()    / step) * step - step);
                rect.setBottom((rect.bottom() / step) * step + step);
                return rect;
        }
        // Crappy name ...
        QString suffixedLength (qreal meters, qreal boxSize) {
                if (boxSize<10)
                        return QString::number(meters, 'f', 2) + "m";
                if (boxSize<1000)
                        return QString::number(meters, 'f', 0) + "m";
                return QString::number(meters/1000, 'f', 0) + "km";
        }
}

NavigationGraphicsView::NavigationGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    waterLevel(-1000000)
{
        setCacheMode(QGraphicsView::CacheNone);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

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
        QImage img(r.width()/1,
                   r.height()/1,
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
                                     h = hn_ * 192;
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

void NavigationGraphicsView::drawRaster(
        QPainter *painter, const QRectF &rectf, qreal step
) {
        const QRect rect = alignRect (rectf, step);

        const QTransform t = painter->transform();
        painter->setTransform(QTransform());

        const int fontSize = QFontMetrics(painter->font()).height();

        for (int x=rect.left(); x<rect.right(); x+=step) {
                painter->drawLine (t.map(QLineF(
                                x, rectf.top(), x, rectf.bottom())));
                painter->drawText (t.map(QPointF(x, rectf.top()))+QPointF(0,fontSize),
                                   suffixedLength(x, step));
        }

        for (int y=rect.top(); y<rect.bottom(); y+=step) {
                painter->drawLine (t.map(QLineF(
                                rectf.left(), y, rectf.right(), y)));

                painter->drawText (t.map(QPointF(rectf.left(), y)),
                                   suffixedLength(-y, step));
        }
        painter->setTransform(t);
}
//#include <QDebug>
void NavigationGraphicsView::drawForeground(QPainter *painter, const QRectF &rectf) {
        QPen pen(QColor(255,255,255,255), 0.5);
        pen.setCosmetic(true);
        painter->setPen(pen);

        const qreal s = std::min(rectf.width(), rectf.height());

        // Okay, below thing is of course bogus ... get over it.
        int box = 1;
        if (s<100) box = 10;
        else if (s<500) box = 50;

        else if (s<1000) box = 100;
        else if (s<5000) box = 500;

        else if (s<10000) box = 1000;
        else if (s<50000) box = 5000;

        else if (s<100000) box = 10000;
        else if (s<500000) box = 50000;

        else if (s<1000000) box = 100000;
        else if (s<5000000) box = 500000;

        else box = 1000000;
        //qDebug() << box;
        drawRaster (painter, rectf, box);
}

void NavigationGraphicsView::scrollContentsBy(int dx, int dy) {
        QGraphicsView::scrollContentsBy(dx, dy);
        viewport()->update();
}

