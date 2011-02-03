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

#include "graphicalnavigationwidget.hh"
#include "ui_graphicalnavigationwidget.h"

#include "observergraphicsitem.hh"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QPen>
#include <QBrush>


#include <cmath>

class SinCosThingy : public HeightFunction {
public:
        virtual double height(double x, double y) const {
                return 0.5 + 0.5 * std::sin(x/50) * std::cos(y/50);
        }
};

GraphicalNavigationWidget::GraphicalNavigationWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::GraphicalNavigationWidget),
        scene(new QGraphicsScene(this))
{
        ui->setupUi(this);
        ui->graphicsView->setScene(scene);

        heightFunction.reset(new SinCosThingy());

        heightmapCutout = scene->addPixmap(pixmapFromFun());

        // TODO: remember those
        ObserverGraphicsItem *obs = new ObserverGraphicsItem();
        connect (obs, SIGNAL(positionChanged(QVector3D)),
                 SLOT(onObserverPositionChanged(QVector3D)));
        connect (obs, SIGNAL(yawChanged(qreal)),
                 SLOT(onObserverYawChanged(qreal)));

        obs->setHeightFunction(heightFunction);
        scene->addItem(obs);

        /*SunGraphicsItem *sun = new SunGraphicsItem();
        sun->setObserverGraphicsItem(obs);
        sun->setSunDirection(1,1,1);
        scene->addItem(sun);*/


        /*double cx = 20, cy=30;
        scene->addEllipse(cx-12,cy-12,
                          24,24,
                          QColor(0,0,0,128),
                          QColor(200,255,200,128)
                         );
        scene->addEllipse(cx-1.5,cy-1.5,
                          3,3,
                          QColor(0,0,0,128),
                          QColor(255,200,200,128)
                         );*/
}

GraphicalNavigationWidget::~GraphicalNavigationWidget() {
        delete ui;
}

QPixmap GraphicalNavigationWidget::pixmapFromFun() const {
        QImage ret(ui->graphicsView->width(),
                   ui->graphicsView->height(),
                   QImage::Format_RGB32);

        for (int v=0; v<ret.height(); ++v) {
                QRgb *sl = (QRgb*)ret.scanLine(v);
                for (int u=0; u<ret.width(); ++u) {
                        const double h = heightFunction->height(u, v);
                        const int hi_ = 255*h,
                                  hi = hi_<0?0:hi_>255?255:hi_;
                        sl[u] = QColor(hi,hi,hi).rgb();
                }
        }
        return QPixmap::fromImage(ret);
}

void GraphicalNavigationWidget::resizeEvent(QResizeEvent *) {
        heightmapCutout->setPixmap(pixmapFromFun());
}

void GraphicalNavigationWidget::showEvent(QShowEvent *) {
        heightmapCutout->setPixmap(pixmapFromFun());
}

void GraphicalNavigationWidget::onObserverPositionChanged (QVector3D pos) {
        ui->absoluteHeight->setValue(pos.y());
}

void GraphicalNavigationWidget::onObserverYawChanged (qreal) {
}
