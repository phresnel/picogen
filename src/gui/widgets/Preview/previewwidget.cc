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

        QEasingCurve curve;
        switch (qrand()%2) {
        case 0: curve = QEasingCurve::OutQuint; break;
        case 1: curve = QEasingCurve::OutExpo; break;
        }

        QPropertyAnimation *a0 = new QPropertyAnimation(this, "opacity");
        a0->setDuration(2000);
        a0->setStartValue(opacity());
        a0->setEndValue(1);

        QPropertyAnimation *a1 = new QPropertyAnimation(this, "rotation");
        a1->setDuration(2000);
        a1->setStartValue(rotation());
        a1->setEndValue(0);
        a1->setEasingCurve(curve);

        QPropertyAnimation *a2 = new QPropertyAnimation(this, "pos");
        a2->setDuration(2000);
        a2->setStartValue(pos());
        a2->setEndValue(QPointF(0,0));
        a2->setEasingCurve(curve);


        anim->addAnimation(a0);
        anim->addAnimation(a1);
        anim->addAnimation(a2);

        anim->start();
}

void PreviewImageObject::fadeOut() {
        anim->clear();

        QEasingCurve curve;
        switch (qrand()%3) {
        case 0: curve = QEasingCurve::InBack; break;
        case 1: curve = QEasingCurve::InQuint; break;
        case 2: curve = QEasingCurve::InExpo; break;
        }

        QPropertyAnimation *a0 = new QPropertyAnimation(this, "opacity");
        a0->setDuration(2000);
        a0->setStartValue(opacity());
        a0->setEndValue(0);

        QPropertyAnimation *a1 = new QPropertyAnimation(this, "rotation");
        a1->setDuration(2000);
        a1->setStartValue(rotation());
        a1->setEndValue(-60 + 120 * (qrand() / (double)RAND_MAX));
        a1->setEasingCurve(curve);

        QPropertyAnimation *a2 = new QPropertyAnimation(this, "pos");
        a2->setDuration(2000);
        a2->setStartValue(pos());
        a2->setEndValue(QPointF(-0.5 + qrand()/(double)RAND_MAX,
                                -0.5 + qrand()/(double)RAND_MAX));
        a2->setEasingCurve(curve);


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
        fitView();
}

void PreviewWidget::showEvent(QShowEvent *) {
        fitView();
}

void PreviewWidget::fitView() {
        const QRectF rect = QRectF(-0.5,-0.5, 1, 1);
        ui->graphicsView->fitInView(rect,
                                    Qt::KeepAspectRatio);
        ui->graphicsView->setSceneRect(rect);
}
