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

#include "lazyquadtreelodplot.hh"
#include "ui_lazyquadtreelodplot.h"

#include <QPainter>
#include <QRectF>
#include <cmath>

namespace {
        class LazyQuadtreeParams {
        public:
                LazyQuadtreeParams(double size, double lodFactor,
                                   unsigned int maxRecursion)
                        : size_(size), lodFactor_(lodFactor)
                        , maxRecursion_(maxRecursion)
                {
                }

                double size() const { return size_; }
                double lodFactor() const { return lodFactor_; }
                unsigned int maxRecursion() const { return maxRecursion_; }

                bool plausible() const {
                        if (size_ <= 0) return false;
                        if (lodFactor_ <= 0) return false;
                        return true;
                }
        private:
                double size_;
                double lodFactor_;
                unsigned int maxRecursion_;
        };

        void draw (double left, double right,
                   LazyQuadtreeParams lqt,
                   unsigned int rec, double diagonal,
                   QRectF const &surface, QPainter &painter
        ) {
                const double c = (left+right) / 2;

                if ((diagonal/(1+c)) < lqt.lodFactor() || rec>=lqt.maxRecursion()) {
                        const double rf = static_cast<double>(rec)/lqt.maxRecursion();

                        const int L = surface.left()   + (left /lqt.size())*surface.width(),
                                  R = surface.left()   + (right/lqt.size())*surface.width(),
                                  h = (rf*surface.height());

                        const QColor green (0,255,0),
                                     red   (255,0,0),
                                     col ((1-rf)*green.red()   + rf*red.red(),
                                          (1-rf)*green.green() + rf*red.green(),
                                          (1-rf)*green.blue()  + rf*red.blue());
                        painter.setBrush(QBrush(col));
                        painter.setPen(QPen(col));
                        painter.drawRect(L,
                                         surface.bottom() - h,
                                         R-L,
                                         h
                                        );
                } else {
                        draw (left, c,  lqt, 1+rec, diagonal/2, surface, painter);
                        draw (c, right, lqt, 1+rec, diagonal/2, surface, painter);
                }
        }
}

LazyQuadtreeLodPlot::LazyQuadtreeLodPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LazyQuadtreeLodPlot)
{
        ui->setupUi(this);
        ui->plotFrame->installEventFilter(this);

        on_lodFactor_valueChanged (ui->lodFactor->value());
}

LazyQuadtreeLodPlot::~LazyQuadtreeLodPlot()
{
        delete ui;
}

bool LazyQuadtreeLodPlot::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::Paint && obj == ui->plotFrame) {
                QPainter p(ui->plotFrame);
                const QRectF surface = ui->plotFrame->rect().adjusted(5,5,-5,-5);
                p.drawRect(surface);

                const LazyQuadtreeParams lqt (10000,
                                              ui->lodFactor->value(),
                                              ui->maxRecursion->value());
                if (lqt.plausible()) {
                        draw (0, lqt.size(), lqt,
                              0, std::sqrt(lqt.size()*lqt.size()+lqt.size()*lqt.size()),
                              surface, p);
                }
                return true;
        } else {
                return QObject::eventFilter(obj, event);
        }
}


void LazyQuadtreeLodPlot::on_maxRecursion_valueChanged(int ) {
        ui->plotFrame->repaint();
}

void LazyQuadtreeLodPlot::on_lodFactor_valueChanged(double v) {
        if (ui->lodFactor->value()<=0)
                ui->lodFactor->setSingleStep(0.001);
        else
                ui->lodFactor->setSingleStep(ui->lodFactor->value() * 0.1);

        ui->plotFrame->repaint();
}
