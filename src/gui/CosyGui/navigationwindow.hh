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

#ifndef NAVIGATIONWINDOW_HH
#define NAVIGATIONWINDOW_HH

#include <QWidget>
#include <QVector3D>
#include "redshift/include/smart_ptr.hh"
#include "createredshiftsceneclosure.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class NavigationWindow;
}



class NavigationWindow : public QWidget {
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit NavigationWindow(QWidget *parent = 0);
        virtual ~NavigationWindow();

        void setScene (redshift::shared_ptr<cosyscene::Scene> scene,
                       bool blockSignals=true);
        void setCreateRedshiftClosure (CreateRedshiftSceneClosure::Ptr);

signals:
        void navigationChanged ();

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);
        void updateTerrain();
        void updateWater();


// ================== Private ==================================================
private:
        void updateViews ();
        void updateFromViews(bool refreshIfAutoRefreshEnabled = false);

        void setNavigationByValue (cosyscene::Navigation const &,
                                   bool blockSignals=true);

        void refreshPreview();

private slots:
        void on_graphicalNavigation_positionChanged (QVector3D);
        void on_graphicalNavigation_orientationChanged (qreal,qreal,qreal);

        void on_refreshButton_clicked();

        // Tabs.
        void on_showRenderTab_clicked();
        void on_showRealTimeTab_clicked();
        void on_showPrecisionTab_clicked();

        // Stashing.
        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

private:
        Ui::NavigationWindow *ui;
        redshift::shared_ptr<cosyscene::Navigation> navigation_;
        redshift::shared_ptr<cosyscene::Scene> scene_;
        CreateRedshiftSceneClosure::Ptr createRedshiftScene;
};

#endif // NAVIGATIONWINDOW_HH
