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

#include "selectpresetdialog.h"
#include "ui_selectpresetdialog.h"

namespace picogen { namespace qt4 {

SelectPresetDialog::SelectPresetDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SelectPresetDialog)
{
        ui->setupUi(this);
}

SelectPresetDialog::~SelectPresetDialog() {
        delete ui;
}

void SelectPresetDialog::setDatabase (shared_ptr<repository::Database> db) {
        ui->list->setDatabase(db);
}

optional<repository::ParametricPreset> SelectPresetDialog::lastSelected() const {
        if (result() == QDialog::Rejected)
                return optional<repository::ParametricPreset>();
        return ui->list->lastSelected();
}

} }

#include "selectpresetdialog.moc"
