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

#ifndef WATERWINDOW_HH
#define WATERWINDOW_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class WaterWindow;
}

namespace picogen { namespace qt4 {

class WaterWindow : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit WaterWindow(QWidget *parent = 0);
        virtual ~WaterWindow();

        void setWater (redshift::shared_ptr<cosyscene::Water>,
                       bool blockSignals=true);

public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

signals:
        void waterChanged();

// ================== Private ==================================================
private:
        void updateViews ();

private slots:
        void on_showWorldFittingTab_clicked();
        void on_showTexturingTab_clicked();
        void on_showFormationTab_clicked();

        void on_formation_formationChanged();
        void on_fitting_fittingChanged();
        void on_texturing_materialChanged();

private:
        Ui::WaterWindow *ui;
        redshift::shared_ptr<cosyscene::Water> water;
};

} }

#endif // WATERWINDOW_HH
