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

#include "graphicalnavigationwidget.h"
#include "ui_graphicalnavigationwidget.h"

#include "observergraphicsitem.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QPen>
#include <QBrush>

#include <cmath>
#include <limits>

namespace picogen { namespace qt4 {

namespace {
        class SinCosThingy : public HeightFunction {
        public:
                virtual double height(double x, double y) const {
                        return 0.5 + 0.5 * std::sin(x/50) * std::cos(y/50);
                }
        };

        class CrossThingy : public HeightFunction {
        public:
                virtual double height(double x, double y) const {
                        if (x>0 && y>0) return 0.7;
                        return ((x>-5 && x<5) || (y>-5 && y<5))*0.5+0.5;
                }
        };
}

GraphicalNavigationWidget::GraphicalNavigationWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::GraphicalNavigationWidget),
        scene(new QGraphicsScene(this))
{
        ui->setupUi(this);
        ui->graphicsView->setScene(scene);

        // Add huge rect so that translation will actually work.
        // Otherwise, alignment won't allow us to.
        // See also http://bugreports.qt.nokia.com/browse/QTBUG-7328 .
        // -> Essentially, non-functioning translation is a result of
        //    GraphicsView's alignment efforts.
        scene->addRect(-10000000,-10000000,
                        20000000, 20000000,
                        QPen(),QBrush(QColor(255,122,64,0)))->setZValue(-2);

        /*const int width = 100000, width05 = width/2;
        for (int y=-1000000; y<1000000; y+=1000) {
                scene->addLine(-width05, y, width05, y, QPen(QColor(128,32,32,255)));
                scene->addLine(y, -width05, y, width05, QPen(QColor(128,32,32,255)));
        }*/

        observerGraphicsItem = new ObserverGraphicsItem();
        setHeightFunction(HeightFunction::Ptr(new SinCosThingy()));
        connect (observerGraphicsItem, SIGNAL(positionChanged(QVector3D)),
                                       SLOT(onObserverPositionChanged(QVector3D)));
        connect (observerGraphicsItem, SIGNAL(orientationChanged(qreal,qreal,qreal)),
                                       SLOT(onObserverOrientationChanged(qreal,qreal,qreal)));


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

void GraphicalNavigationWidget::resizeEvent(QResizeEvent *) {
}

void GraphicalNavigationWidget::showEvent(QShowEvent *) {
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

void GraphicalNavigationWidget::updateOwnPitch(qreal v) {
        const bool b = ui->pitch->blockSignals(true);
        ui->pitch->setValue(v/0.0174532925);
        ui->pitch->blockSignals(b);
}

void GraphicalNavigationWidget::updateOwnRoll(qreal v) {
        const bool b = ui->roll->blockSignals(true);
        ui->roll->setValue(-v/0.0174532925);
        ui->roll->blockSignals(b);
}

void GraphicalNavigationWidget::onObserverPositionChanged (QVector3D pos) {
        updateOwnPosition (pos);
        emit positionChanged(pos);
}

void GraphicalNavigationWidget::onObserverOrientationChanged (qreal yaw,
                                                              qreal pitch,
                                                              qreal roll)
{
        updateOwnYaw(yaw);
        updateOwnPitch(pitch);
        updateOwnRoll(roll);
        emit orientationChanged(yaw,pitch,roll);
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
}
void GraphicalNavigationWidget::on_pitch_valueChanged(double v) {
        observerGraphicsItem->setObserverPitch(v*0.0174532925);
        ui->pitchWidget->setPitch(v*0.0174532925);
}
void GraphicalNavigationWidget::on_roll_valueChanged(double v) {
        // We negated roll for users sake.
        observerGraphicsItem->setObserverRoll(-v*0.0174532925);
        ui->rollWidget->setRoll(-v*0.0174532925);
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

void GraphicalNavigationWidget::setYaw (qreal v) {
        updateOwnYaw(v);
        observerGraphicsItem->setObserverYaw(v);
}

void GraphicalNavigationWidget::setPitch (qreal v) {
        updateOwnPitch(v);
        observerGraphicsItem->setObserverPitch(v);
        ui->pitchWidget->setPitch(v);
}

void GraphicalNavigationWidget::setRoll (qreal v) {
        updateOwnRoll(v);
        observerGraphicsItem->setObserverRoll(v);
        ui->rollWidget->setRoll(v);
}

void GraphicalNavigationWidget::on_rollWidget_rollEdited (qreal v) {
        updateOwnRoll(v);
        observerGraphicsItem->setObserverRoll(v);
}

void GraphicalNavigationWidget::on_pitchWidget_pitchEdited (qreal v) {
        updateOwnPitch(v);
        observerGraphicsItem->setObserverPitch(v);
}


QVector3D GraphicalNavigationWidget::position() const {
        return observerGraphicsItem->observerPosition();
}

qreal GraphicalNavigationWidget::yaw() const {
        return observerGraphicsItem->observerYaw();
}

qreal GraphicalNavigationWidget::pitch() const {
        return observerGraphicsItem->observerPitch();
}

qreal GraphicalNavigationWidget::roll() const {
        return observerGraphicsItem->observerRoll();
}

void GraphicalNavigationWidget::setHeightFunction (HeightFunction::Ptr f) {
        heightFunction = f;

        observerGraphicsItem->setHeightFunction(heightFunction);
        ui->graphicsView->setHeightFunction(heightFunction);

        // update relative height spinbox
        const QVector3D pos = observerGraphicsItem->observerPosition();
        const bool b = ui->relativeHeight->blockSignals(true);
        ui->relativeHeight->setValue(pos.y() - heightFunction->height(pos.x(),pos.z()));
        ui->relativeHeight->blockSignals(b);
}

void GraphicalNavigationWidget::setWaterLevel (qreal w) {
        ui->graphicsView->setWaterLevel(w);
}

void GraphicalNavigationWidget::on_zoomIn_clicked() {
        ui->graphicsView->scale(1.5, 1.5);
}

void GraphicalNavigationWidget::on_zoomOut_clicked() {
        ui->graphicsView->scale(0.75, 0.75);
}

void GraphicalNavigationWidget::on_resetZoom_clicked() {
        ui->graphicsView->resetTransform();
}

void GraphicalNavigationWidget::on_findMe_clicked() {
        ui->graphicsView->centerOn(observerGraphicsItem);
}

void GraphicalNavigationWidget::on_pixelSize_valueChanged(int value) {
        ui->graphicsView->setPixelSize(value);
}

} }

#include "graphicalnavigationwidget.moc"
