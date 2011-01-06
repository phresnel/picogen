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

#include "parametricpresetui.hh"
#include "ui_parametricpresetui.h"
#include "parametricpresetpopup.hh"

ParametricPresetUi::ParametricPresetUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametricPresetUi)
{
        ui->setupUi(this);
}

ParametricPresetUi::ParametricPresetUi(ParametricPreset const &pp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametricPresetUi)
{
        ui->setupUi(this);
        setPreset(pp);
}

ParametricPresetUi::~ParametricPresetUi() {
        delete ui;
}

ParametricPreset ParametricPresetUi::preset() const {
        return preset_;
}

void ParametricPresetUi::setPreset (ParametricPreset const &pp) {
        preset_ = pp;
        ui->text->setText(
            QString("\"%1\" by %2%3")
            .arg(pp.title() == "" ? "<untitled>" : pp.title())
            .arg(pp.author() == "" ? "<unknown>" : pp.author())
            .arg(pp.email() == "" ? "" : (" (" + pp.email() + ")"))
        );
}

void ParametricPresetUi::on_toolButton_clicked() {
        ParametricPresetPopup popup (this);
        popup.setPreset(preset_);
        if (QDialog::Accepted == popup.exec())
                setPreset (popup.preset());
}
