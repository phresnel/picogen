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

#include "presetdialog.h"
#include "ui_presetdialog.h"

namespace picogen { namespace qt4_gui {

PresetDialog::PresetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PresetDialog)
{
        ui->setupUi(this);
        setPreset(repository::ParametricPreset());
        ui->preset->showSourceEditor(true);
}

PresetDialog::~PresetDialog() {
        delete ui;
}

void PresetDialog::setReadOnly(bool readOnly) {
        ui->preset->showSourceEditor(!readOnly);
        ui->entity->setReadOnly(readOnly);
}

repository::ParametricPreset PresetDialog::preset() const {
        return preset_;
}

void PresetDialog::setPreset (repository::ParametricPreset const &pp) {
        preset_ = pp;
        ui->entity->setEntity (&preset_);
        ui->preset->setPreset(pp.preset());
}

void PresetDialog::on_buttonBox_accepted() {
        accept();
        preset_.save();
}

void PresetDialog::on_buttonBox_rejected() {
        reject();
}

void PresetDialog::on_preset_formationChanged() {
        preset_.setPreset(ui->preset->preset());
}

} }

#include "presetdialog.moc"
