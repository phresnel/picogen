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

#ifndef WATERFITTING_HH
#define WATERFITTING_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"

namespace Ui {
        class WaterFitting;
}

namespace cosyscene {
        class WaterFitting;
}

class WaterFitting : public QWidget
{
        Q_OBJECT

public:
        explicit WaterFitting(QWidget *parent = 0);
        ~WaterFitting();

        void setFitting (
                redshift::shared_ptr<cosyscene::WaterFitting> t,
                bool blockSignals
        );

signals:
        void fittingChanged();

private:
        Ui::WaterFitting *ui;

        int previousMaxRecursion;
        redshift::shared_ptr<cosyscene::WaterFitting> fitting_;

private slots:
        void on_seaLevel_valueChanged(double );

        void on_stashButton_clicked();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked();
};


#endif // WATERFITTING_HH
