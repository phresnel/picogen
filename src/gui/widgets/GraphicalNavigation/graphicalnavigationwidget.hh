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

#ifndef GRAPHICALNAVIGATIONWIDGET_HH
#define GRAPHICALNAVIGATIONWIDGET_HH

#include <QWidget>
#include <QVector3D>
#include "heightfunction.hh"
#include "observergraphicsitem.hh"

class QGraphicsScene;
class QGraphicsPixmapItem;

namespace Ui {
        class GraphicalNavigationWidget;
}

class GraphicalNavigationWidget : public QWidget
{
        Q_OBJECT

public:
        explicit GraphicalNavigationWidget(QWidget *parent = 0);
        ~GraphicalNavigationWidget();

        void setSunDirection (double x, double y, double z);

private:
        void resizeEvent(QResizeEvent *);
        void showEvent(QShowEvent *);

private:
        Ui::GraphicalNavigationWidget *ui;
        QGraphicsScene *scene;
        QGraphicsPixmapItem *heightmapCutout;
        HeightFunction::Ptr heightFunction;
        ObserverGraphicsItem *observerGraphicsItem;

        QPixmap pixmapFromFun() const;
        static QString degreeToName(qreal degree);

private slots:
        void on_yaw_valueChanged(double );
        void on_north_valueChanged(double );
        void on_east_valueChanged(double );
        void on_relativeHeight_valueChanged(double );
        void on_absoluteHeight_valueChanged(double );
        void on_keepRelative_toggled(bool checked);
        void on_keepAbsolute_toggled(bool checked);
        void onObserverPositionChanged (QVector3D);
        void onObserverYawChanged (qreal);
};

#endif // GRAPHICALNAVIGATIONWIDGET_HH
