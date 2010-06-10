//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef COVER_HH
#define COVER_HH

/*
#include <QPainter>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>


class Cover : public QGraphicsItem,
              private QGraphicsItemAnimation
{
        Q_OBJECT
public:
        Cover (QString const &filename, QGraphicsItem *parent=0)
                : centerX(0), centerY(0), pix (filename)
        {
                setAcceptHoverEvents(true);
                isHovering=false;
                currentHoverScale = 1;
                sourceHoverScale = targetHoverScale = currentHoverScale;

                ((QGraphicsItemAnimation*)this)->setItem(this);

                adjustToImageCenter.translate(-pix.width()*.5, -pix.height()*.5);
                //setTransformOriginPoint(pix.width()*.5, pix.height()*.5);

                localScaling.scale(1.f/pix.width(),1.f/pix.width());
                updateTransform();
        }

        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
                isHovering = true;

                acquireNewTimer();

                sourceHoverScale = currentHoverScale;
                targetHoverScale = 1.2;
        }

        //void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
                isHovering = false;

                acquireNewTimer();

                sourceHoverScale = currentHoverScale;
                targetHoverScale = 1;
        }

        QRectF boundingRect () const {
                return QRectF(
                        0, 0,
                        pix.width(), pix.height()
                );
        }
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
                painter->drawPixmap(0,0,pix);
        }

        QTransform transform() const {
                //return R*T*S;
                QTransform hover;
                hover.scale(currentHoverScale, currentHoverScale);

                const QTransform tmp =
                        adjustToImageCenter
                        *hover
                        *localScaling
                        *sliding
                        *localRotation;
                QTransform absPos;
                absPos.translate(centerX, centerY);

                const QTransform localPlusAbs = tmp * absPos;

                return localPlusAbs;
        }

        void rotate (qreal a, Qt::Axis axis = Qt::ZAxis) {
                localRotation.reset();
                localRotation.rotate(a, axis);
                updateTransform();
        }

        void translate (qreal x, qreal y) {
                centerX = x;
                centerY = y;
                updateTransform();
        }
        void slide (qreal x, qreal y) {
                sliding.reset();
                sliding.translate(x, y);
                updateTransform();
        }

        void scale (qreal s) {
                localScaling.reset();
                localScaling.scale(s,s);
                updateTransform();
        }

private slots:
        void timerFinished() {
                sourceHoverScale = targetHoverScale = currentHoverScale;
        }
private:
        void acquireNewTimer() {
                if (timeLine())
                        disconnect(timeLine(), SIGNAL(finished()),
                                   this, SLOT(timerFinished()));
                setTimeLine(new QTimeLine(200, this));
                connect(timeLine(), SIGNAL(finished()),
                        this, SLOT(timerFinished()));
        }

        void beforeAnimationStep (qreal step) {
                currentHoverScale = sourceHoverScale
                                  + step*(targetHoverScale-sourceHoverScale);
                updateTransform();
        }

        void updateTransform() {
                setTransform(transform());
        }

        QTransform adjustToImageCenter, sliding, localRotation, localScaling;
        qreal centerX, centerY;

        QPixmap pix;
        bool isHovering;

        qreal currentHoverScale, sourceHoverScale, targetHoverScale;

};
*/

#include <QGraphicsItem>

class QPixmap;

class Cover : public QGraphicsItem {
public:
        Cover (QString const &filename, QGraphicsItem *parent = 0);
        virtual ~Cover();
        virtual QRectF boundingRect () const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

private:
        QTransform composeTransform() const;
        QPixmap *pixmap;
};

#endif // COVER_HH
