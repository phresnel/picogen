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

#ifndef PARAMETRICPRESETUI_HH
#define PARAMETRICPRESETUI_HH

#include <QWidget>
#include "parametricpreset.hh"

namespace Ui {
    class ParametricPresetUi;
}

class ParametricPresetUi : public QWidget
{
        Q_OBJECT

public:
        explicit ParametricPresetUi(QWidget *parent = 0);
        explicit ParametricPresetUi(ParametricPreset const &, QWidget *parent = 0);
        ~ParametricPresetUi();

        ParametricPreset preset() const;
        void setPreset (ParametricPreset const &);

private:
        Ui::ParametricPresetUi *ui;
        ParametricPreset preset_;

private slots:
        void on_toolButton_clicked();
};

#endif // PARAMETRICPRESETUI_HH
