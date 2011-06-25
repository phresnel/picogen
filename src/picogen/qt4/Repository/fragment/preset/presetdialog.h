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

#ifndef PRESETDIALOG_HH
#define PRESETDIALOG_HH

#include <QDialog>

namespace Ui {
    class PresetDialog;
}

#include "parametricpreset.hh"

namespace picogen { namespace qt4_gui {

class PresetDialog : public QDialog
{
        Q_OBJECT

public:
        explicit PresetDialog(QWidget *parent = 0);
        ~PresetDialog();

        repository::ParametricPreset preset() const;
        void setPreset (repository::ParametricPreset const &);

        void setReadOnly (bool readOnly);

private:
        Ui::PresetDialog *ui;
        repository::ParametricPreset preset_;

private slots:

        void on_buttonBox_rejected();
        void on_buttonBox_accepted();

        void on_preset_formationChanged();
};

} }

#endif // PRESETDIALOG_HH
