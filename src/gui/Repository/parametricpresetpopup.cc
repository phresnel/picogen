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

#include "parametricpresetpopup.hh"
#include "ui_parametricpresetpopup.h"

ParametricPresetPopup::ParametricPresetPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametricPresetPopup)
{
        ui->setupUi(this);
        setPreset(ParametricPreset());
        ui->preset->showSourceEditor(true);
}

ParametricPresetPopup::~ParametricPresetPopup() {
        delete ui;
}

ParametricPreset ParametricPresetPopup::preset() const {
        return preset_;
}

void ParametricPresetPopup::setPreset (ParametricPreset const &pp) {
        preset_ = pp;
        ui->title->setText(pp.title());
        ui->email->setText(pp.email());
        ui->author->setText(pp.author());
        ui->preset->setPreset(pp.preset());
}

void ParametricPresetPopup::on_buttonBox_accepted() {
        accept();
        preset_.save();
}

void ParametricPresetPopup::on_buttonBox_rejected() {
        reject();
}

void ParametricPresetPopup::on_title_editingFinished() {
        preset_.setTitle (ui->title->text());
}

void ParametricPresetPopup::on_author_editingFinished() {
        preset_.setAuthor (ui->author->text());
}

void ParametricPresetPopup::on_email_editingFinished() {
        preset_.setEmail(ui->email->text());
}

void ParametricPresetPopup::on_preset_formationChanged() {
        preset_.setPreset(ui->preset->preset());
}
