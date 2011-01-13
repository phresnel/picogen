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
#include <QGraphicsItem>
#include <QGraphicsObject>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QTimeLine>

#include <QDebug>
#include <QResizeEvent>

PreviewImageObject::PreviewImageObject (QGraphicsItem* parent)
        : QGraphicsPixmapItem(parent)
        , anim(new QParallelAnimationGroup(this))
{
}
PreviewImageObject::~PreviewImageObject() {
}
void PreviewImageObject::fadeIn() {
        anim->clear();

        QPropertyAnimation *a0 = new QPropertyAnimation(this, "opacity");
        a0->setDuration(1000);
        a0->setStartValue(opacity());
        a0->setEndValue(1);

        QPropertyAnimation *a1 = new QPropertyAnimation(this, "rotation");
        a1->setDuration(1000);
        a1->setStartValue(rotation());
        a1->setEndValue(0);

        QPropertyAnimation *a2 = new QPropertyAnimation(this, "pos");
        a2->setDuration(1000);
        a2->setStartValue(pos());
        a2->setEndValue(QPointF(0,0));


        anim->addAnimation(a0);
        anim->addAnimation(a1);
        anim->addAnimation(a2);

        anim->start();
}

void PreviewImageObject::fadeOut() {
        anim->clear();

        QPropertyAnimation *a0 = new QPropertyAnimation(this, "opacity");
        a0->setDuration(1000);
        a0->setStartValue(opacity());
        a0->setEndValue(0);

        QPropertyAnimation *a1 = new QPropertyAnimation(this, "rotation");
        a1->setDuration(1000);
        a1->setStartValue(rotation());
        a1->setEndValue(-60 + 120 * (qrand() / (double)RAND_MAX));

        QPropertyAnimation *a2 = new QPropertyAnimation(this, "pos");
        a2->setDuration(1000);
        a2->setStartValue(pos());
        a2->setEndValue(QPointF(0.9,0));


        anim->addAnimation(a0);
        anim->addAnimation(a1);
        anim->addAnimation(a2);

        anim->start();
}



PreviewWidget::PreviewWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::PreviewWidget),
        scene(new QGraphicsScene(this)),
        current(0)
{
        ui->setupUi(this);
        ui->graphicsView->setScene(scene);
}

PreviewWidget::~PreviewWidget() {
        foreach (PreviewImageObject *ob, images)
                delete ob;
        delete ui;
}

void PreviewWidget::addImage (QImage img) {
        PreviewImageObject *item = new PreviewImageObject();
        item->setPixmap(QPixmap::fromImage(img));
        item->setScale(1.0 / item->boundingRect().width());
        item->setOffset(-0.5*item->boundingRect().width(),
                        -0.5*item->boundingRect().height());

        if (images.count() == 0)
                item->setOpacity(1);
        else
                item->setOpacity(0);

        images.push_back(item);
        scene->addItem(item);
}

void PreviewWidget::timerEvent(QTimerEvent *) {
        if (images.count() <= 1)
                return;

        const int next = (current+1) % images.count();
        images[current]->fadeOut();
        images[next]->fadeIn();
        current = next;
}

void PreviewWidget::resizeEvent(QResizeEvent *) {
        /*ui->graphicsView->fitInView(QRectF(-1,-1, 1, 1),
                                    Qt::KeepAspectRatio);*/
        fitView();
}

void PreviewWidget::showEvent(QShowEvent *) {
        /*ui->graphicsView->fitInView(QRectF(-1,-1, 1, 1),
                                    Qt::KeepAspectRatio);*/
        fitView();
}

void PreviewWidget::fitView() {
        ui->graphicsView->fitInView(QRectF(-1,-1, 1, 1),
                                    Qt::KeepAspectRatio);
        QTransform t = ui->graphicsView->transform();
        qDebug() << t;
}
