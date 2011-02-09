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

        void setPosition (QVector3D position);
        void setPosition (qreal x, qreal y, qreal z);
        void setYaw (qreal yaw);
        void setPitch (qreal pitch);
        void setRoll (qreal roll);

        QVector3D position() const;
        qreal yaw() const;
        qreal pitch() const;
        qreal roll() const;

        void setHeightFunction (HeightFunction::Ptr);
        void setWaterLevel (qreal h);
signals:
        void positionChanged (QVector3D newPosition);
        void orientationChanged (qreal newYaw, qreal newPitch, qreal newRoll);

private:
        void resizeEvent(QResizeEvent *);
        void showEvent(QShowEvent *);

private:
        Ui::GraphicalNavigationWidget *ui;
        QGraphicsScene *scene;
        HeightFunction::Ptr heightFunction;
        ObserverGraphicsItem *observerGraphicsItem;

        void updateOwnPosition (QVector3D pos);
        void updateOwnYaw (qreal yaw);
        void updateOwnPitch (qreal yaw);
        void updateOwnRoll (qreal yaw);

        static QString degreeToName(qreal degree);

private slots:
        void on_findMe_clicked();
        void on_resetZoom_clicked();
        void on_zoomOut_clicked();
        void on_zoomIn_clicked();
        void on_rollWidget_rollEdited (qreal);
        void on_pitchWidget_pitchEdited (qreal);
        void on_yaw_valueChanged(double );
        void on_pitch_valueChanged(double );
        void on_roll_valueChanged(double );
        void on_north_valueChanged(double );
        void on_east_valueChanged(double );
        void on_relativeHeight_valueChanged(double );
        void on_absoluteHeight_valueChanged(double );
        void on_keepRelative_toggled(bool checked);
        void on_keepAbsolute_toggled(bool checked);
        void onObserverPositionChanged (QVector3D);
        void onObserverOrientationChanged (qreal,qreal,qreal);
};

#endif // GRAPHICALNAVIGATIONWIDGET_HH
