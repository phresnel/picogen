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

NavigationGraphicsView::NavigationGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void NavigationGraphicsView::setHeightFunction (HeightFunction::Ptr f) {
        heightFunction = f;
        repaint();
}

void NavigationGraphicsView::drawBackground(QPainter *painter, const QRectF &rect) {

        const QRectF r = painter->worldTransform().mapRect(rect);
        QImage img(r.width(),
                   r.height(),
                   QImage::Format_RGB32);
        const int height=img.height(), width=img.width();

        double min = std::numeric_limits<float>::infinity(),
               max = -std::numeric_limits<float>::infinity();

        for (int y=0; y<img.height(); ++y) {
                const double v_ = ((height-1)-y) / (double)height,
                             v = v_*rect.height() + rect.top();
                QRgb *sl = (QRgb*)img.scanLine(y);
                for (int x=0; x<img.width(); ++x) {
                        const double u_ = x / (double)width,
                                     u = u_*rect.width() + rect.left();

                        const double h_ = heightFunction->height(u,v),
                                     h = h_ * 255;
                        const int hi = h < 0 ? 0 : h > 255 ? 255 : h;
                        sl[x] = QColor(hi,hi,hi).rgb();
                }
        }
        painter->drawImage(rect, img);
}

