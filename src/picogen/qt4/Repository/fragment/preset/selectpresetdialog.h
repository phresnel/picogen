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

#ifndef SELECTPRESETDIALOG_HH
#define SELECTPRESETDIALOG_HH

#include <QDialog>
#include "database.hh"
#include "parametricpreset.hh"
#include "../../shared_ptr.h"
#include "../../optional.h"

namespace Ui {
    class SelectPresetDialog;
}

namespace picogen { namespace qt4 {

class SelectPresetDialog : public QDialog
{
        Q_OBJECT

public:
        explicit SelectPresetDialog(QWidget *parent = 0);
        ~SelectPresetDialog();

        void setDatabase (shared_ptr<Database>);
        optional<ParametricPreset> lastSelected() const;

private:
    Ui::SelectPresetDialog *ui;
};

} }

#endif // SELECTPRESETDIALOG_HH
