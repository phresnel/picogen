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

#ifndef TERRAINFITTING_HH
#define TERRAINFITTING_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
        class TerrainFitting;
}

namespace picogen { namespace qt4_gui {

class TerrainFitting : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit TerrainFitting(QWidget *parent = 0);
        virtual ~TerrainFitting();

        void setFitting (redshift::shared_ptr<cosyscene::TerrainFitting>,
                         bool blockSignals);

signals:
        void fittingChanged();


// ================== Private ==================================================
private slots:
        void on_detailCoefficient_valueChanged(double);
        void on_visibleExtent_valueChanged    (double);
        void on_maxRecursion_valueChanged     (int);

        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

private:
        Ui::TerrainFitting *ui;

        int previousMaxRecursion;
        redshift::shared_ptr<cosyscene::TerrainFitting> fitting_;
};

} }

#endif // TERRAINFITTING_HH
