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
        //ui->graphicsView->setTransform(ui->graphicsView->transform().scale(1,-1));

        heightFunction.reset(new SinCosThingy());

        heightmapCutout = scene->addPixmap(pixmapFromFun());

        observerGraphicsItem = new ObserverGraphicsItem();
        connect (observerGraphicsItem, SIGNAL(positionChanged(QVector3D)),
                                       SLOT(onObserverPositionChanged(QVector3D)));
        connect (observerGraphicsItem, SIGNAL(yawChanged(qreal)),
                                       SLOT(onObserverYawChanged(qreal)));

        observerGraphicsItem->setHeightFunction(heightFunction);

        observerGraphicsItem->setObserverAbsoluteHeight(5);
        observerGraphicsItem->setObserverEast(0);
        observerGraphicsItem->setObserverNorth(0);
        observerGraphicsItem->setObserverYaw(0);

        scene->addItem(observerGraphicsItem);

        ui->keepAbsolute->setChecked(true);
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

void GraphicalNavigationWidget::updateOwnPosition (QVector3D pos) {
        bool b = ui->absoluteHeight->blockSignals(true);
        ui->absoluteHeight->setValue(pos.y());
        ui->absoluteHeight->blockSignals(b);

        b = ui->relativeHeight->blockSignals(true);
        ui->relativeHeight->setValue(pos.y() - heightFunction->height(pos.x(),pos.z()));
        ui->relativeHeight->blockSignals(b);

        b = ui->east->blockSignals(true);
        ui->east->setValue(pos.x());
        ui->east->blockSignals(b);

        b = ui->north->blockSignals(true);
        ui->north->setValue(pos.z());
        ui->north->blockSignals(b);
}

void GraphicalNavigationWidget::updateOwnYaw(qreal yaw) {
        const bool b = ui->yaw->blockSignals(true);
        ui->yaw->setValue(yaw/0.0174532925);
        ui->yaw->blockSignals(b);
}

void GraphicalNavigationWidget::onObserverPositionChanged (QVector3D pos) {
        updateOwnPosition (pos);
        emit positionChanged(pos);
}

void GraphicalNavigationWidget::onObserverYawChanged (qreal v) {
        updateOwnYaw(v);
        emit yawChanged(v);
}

void GraphicalNavigationWidget::on_keepAbsolute_toggled(bool checked) {
        if (checked)
                observerGraphicsItem->setAutoHeightMode(KeepAbsoluteHeight);
}

void GraphicalNavigationWidget::on_keepRelative_toggled(bool checked) {
        if (checked)
                observerGraphicsItem->setAutoHeightMode(KeepRelativeHeight);
}

void GraphicalNavigationWidget::on_absoluteHeight_valueChanged(double v) {
        observerGraphicsItem->setObserverAbsoluteHeight(v);
}

void GraphicalNavigationWidget::on_relativeHeight_valueChanged(double v) {
        observerGraphicsItem->setObserverRelativeHeight(v);
}

void GraphicalNavigationWidget::on_east_valueChanged(double v) {
        observerGraphicsItem->setObserverEast(v);
}

void GraphicalNavigationWidget::on_north_valueChanged(double v) {
        observerGraphicsItem->setObserverNorth(v);
}

void GraphicalNavigationWidget::on_yaw_valueChanged(double v) {
        observerGraphicsItem->setObserverYaw(v*0.0174532925);
        ui->yaw->setSuffix("° (" + degreeToName(v) + ")");
}

QString GraphicalNavigationWidget::degreeToName(qreal degree) {
        if (degree < 22.5)          return "N";
        else if (degree < 45+22.5)  return "NE";
        else if (degree < 90+22.5)  return "E";
        else if (degree < 135+22.5) return "SE";
        else if (degree < 180+22.5) return "S";
        else if (degree < 225+22.5) return "SW";
        else if (degree < 270+22.5) return "W";
        else if (degree < 315+22.5) return "NW";
        else                        return "N";
}

void GraphicalNavigationWidget::setPosition (QVector3D pos) {
        updateOwnPosition(pos);
        observerGraphicsItem->setObserverAbsolutePosition(pos);
}

void GraphicalNavigationWidget::setPosition (qreal x, qreal y, qreal z) {
        setPosition (QVector3D (x, y, z));
}

void GraphicalNavigationWidget::setYaw (qreal yaw) {
        updateOwnYaw(yaw);
        observerGraphicsItem->setObserverYaw(yaw);
}

QVector3D GraphicalNavigationWidget::position() const {
        return observerGraphicsItem->observerPosition();
}

qreal GraphicalNavigationWidget::yaw() const {
        return observerGraphicsItem->observerYaw();
}
