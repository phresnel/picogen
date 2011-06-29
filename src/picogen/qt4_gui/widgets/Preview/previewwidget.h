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

#ifndef PREVIEWWIDGET_HH
#define PREVIEWWIDGET_HH

#include <QWidget>
#include <QGraphicsItemAnimation>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>

class QGraphicsScene;
class QParallelAnimationGroup;


/*
class PreviewImageAnimation : public QGraphicsItemAnimation {
        Q_OBJECT
public:
        explicit PreviewImageAnimation(QWidget *parent = 0);
        ~PreviewImageAnimation();

        void fadeOut();
        void fadeIn();

protected:
        void beforeAnimationStep (qreal step);

private:
        enum FadeMode { fade_in, fade_out };
        FadeMode fadeMode;
};
*/
namespace picogen { namespace qt4_gui {

class PreviewImageObject : public QObject, public QGraphicsPixmapItem {
        Q_OBJECT
        Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)
        Q_PROPERTY(QPointF pos READ pos WRITE setPos)
        Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
public:
        PreviewImageObject (QGraphicsItem* parent=0);
        virtual ~PreviewImageObject();

        void fadeIn();
        void fadeOut();

private:
        QParallelAnimationGroup *anim;
};

} }


namespace Ui {
        class PreviewWidget;
}

namespace picogen { namespace qt4_gui {
class PreviewWidget : public QWidget
{
        Q_OBJECT

public:
        explicit PreviewWidget(QWidget *parent = 0);
        ~PreviewWidget();

        void addImage (QImage);
        void addImage (QString filename);
        void addImages (QStringList filenames);
        void setImages (QStringList filenames);
        void reset();

        void resizeEvent(QResizeEvent *);
        void showEvent(QShowEvent *);

private:
        Ui::PreviewWidget *ui;
        //QList<PreviewImageAnimation*> animations;
        QList<PreviewImageObject*> images;
        QGraphicsScene *scene;
        int current;

private:
        void timerEvent(QTimerEvent *event);
        void fitView();
};

} }

#endif // PREVIEWWIDGET_HH
