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

#include "presetlistitemwidget.h"
#include "ui_presetlistitemwidget.h"
#include "presetdialog.h"

namespace picogen_repository {

PresetListItemWidget::PresetListItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PresetListItemWidget)
{
        ui->setupUi(this);
        setReadOnly(true);
}

PresetListItemWidget::PresetListItemWidget(ParametricPreset const &pp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PresetListItemWidget)
{
        ui->setupUi(this);
        setPreset(pp);
}

PresetListItemWidget::~PresetListItemWidget() {
        delete ui;
}

ParametricPreset PresetListItemWidget::preset() const {
        return preset_;
}

void PresetListItemWidget::setPreset (ParametricPreset const &pp) {
        preset_ = pp;
        ui->text->setText(
            QString("<big><b>%1</b></big><br>"
                    "<i>by %5%6</i><br>"
                    "<small><small>from collection <b>%2</b> by <i>%3%4</i></small></small><br>")
            .arg(pp.title() == "" ? "<untitled>" : pp.title())
            .arg(pp.package().title() == "" ? "<unknown>" : pp.package().title())
            .arg(pp.package().author() == "" ? "<unknown>" : pp.package().author())
            .arg(pp.package().homepage() == "" ? "" : (" (" + pp.package().homepage() + ")"))

            .arg(pp.author() == "" ? "<unknown>" : pp.author())
            .arg(pp.homepage() == "" ? "" : (" (" + pp.homepage() + ")"))
        );
}

void PresetListItemWidget::on_toolButton_clicked() {
        PresetDialog popup (this);
        popup.setReadOnly(readOnly_);
        popup.setPreset(preset_);
        if (QDialog::Accepted == popup.exec())
                setPreset (popup.preset());
}

void PresetListItemWidget::setReadOnly(bool readOnly) {
        readOnly_ = readOnly;

        if (readOnly_) {
                ui->readOnlyText->setText("read only");
        } else {
                ui->readOnlyText->setText("");
        }
}

} // namespace picogen_repository {

#include "presetlistitemwidget.moc"

