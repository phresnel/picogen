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

class QGraphicsPixmapItem;
class QGraphicsScene;



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




namespace Ui {
        class PreviewWidget;
}

class PreviewWidget : public QWidget
{
        Q_OBJECT

public:
        explicit PreviewWidget(QWidget *parent = 0);
        ~PreviewWidget();

        void addImage (QImage const &);

private:
        Ui::PreviewWidget *ui;
        QList<PreviewImageAnimation*> animations;
        QGraphicsScene *scene;
        int current;

private:
        void timerEvent(QTimerEvent *event);
};

#endif // PREVIEWWIDGET_HH
