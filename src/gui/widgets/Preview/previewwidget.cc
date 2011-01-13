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

#include "previewwidget.hh"
#include "ui_previewwidget.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimeLine>

#include <QDebug>

PreviewImageAnimation::PreviewImageAnimation(QWidget *parent)
        : QGraphicsItemAnimation(parent)
{
}

PreviewImageAnimation::~PreviewImageAnimation() {
        if (timeLine())
                timeLine()->stop();
}

void PreviewImageAnimation::fadeOut() {
        fadeMode = fade_out;

        if (item()) {
                qreal t = timeLine() ? timeLine()->currentValue() : 1;
                setPosAt(0, posAt(t));
                setRotationAt(0, rotationAt(t));
        }

        setPosAt(1, QPointF(0,300));
        setRotationAt (1, -60 + 120 * (qrand() / (double)RAND_MAX));
}

void PreviewImageAnimation::fadeIn(){
        fadeMode = fade_in;

        if (item()) {
                qreal t = timeLine() ? timeLine()->currentValue() : 1;
                setPosAt(0, posAt(t));
                setRotationAt(0, rotationAt(t));
        }

        setPosAt(1, QPointF(0,0));
        setRotationAt (1, -7.5 + 15 * (qrand() / (double)RAND_MAX));
}

void PreviewImageAnimation::beforeAnimationStep (qreal step) {
        if (!item()) return;

        switch(fadeMode) {
        case fade_in:
                item()->setOpacity(step);
                break;
        case fade_out: {
                item()->setOpacity(1-step);
                break;
        }
        }
}






PreviewWidget::PreviewWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PreviewWidget),
        scene(new QGraphicsScene(this)),
        current(0)
{
        ui->setupUi(this);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->scale(0.3,0.3);
}

PreviewWidget::~PreviewWidget() {
        delete ui;
}

void PreviewWidget::addImage (QImage const &img) {

        QGraphicsPixmapItem *item = ui->graphicsView
                                    ->scene()
                                    ->addPixmap(QPixmap::fromImage(img));

        PreviewImageAnimation *animation = new PreviewImageAnimation(this);
        QTimeLine *timer = new QTimeLine(1000, this);

        timer->setFrameRange(0, 1);
        timer->setCurveShape(QTimeLine::EaseInOutCurve);

        animation->setItem(item);
        animation->setTimeLine(timer);

        item->setOpacity(0);
        animations.push_back(animation);

        // so that image 0 is always shown first
        if (animations.count() == 1) {
                item->setOpacity(1);
        }
}

void PreviewWidget::timerEvent(QTimerEvent *) {
        if (animations.count() <= 1)
                return;

        const int next = (current+1) % animations.count();

        animations[current]->fadeOut();
        animations[current]->timeLine()->stop();
        animations[current]->timeLine()->start();

        animations[next]->fadeIn();
        animations[next]->timeLine()->stop();
        animations[next]->timeLine()->start();

        current = next;
}
