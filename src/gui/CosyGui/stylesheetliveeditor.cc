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

#include "stylesheetliveeditor.h"
#include "ui_stylesheetliveeditor.h"

StylesheetLiveEditor::StylesheetLiveEditor(QWidget *applyTo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StylesheetLiveEditor),
    applyTo(applyTo)
{
        ui->setupUi(this);
        ui->plainTextEdit->setPlainText(applyTo->styleSheet());
}

StylesheetLiveEditor::~StylesheetLiveEditor()
{
        delete ui;
}

void StylesheetLiveEditor::on_buttonBox_clicked(QAbstractButton* button)
{
        switch (ui->buttonBox->buttonRole(button)) {
        case QDialogButtonBox::ApplyRole:
                applyTo->setStyleSheet(ui->plainTextEdit->toPlainText());
                break;
        case QDialogButtonBox::AcceptRole:
                close();
                break;
        default:
                break;
        }
}

void StylesheetLiveEditor::on_actionApply_triggered()
{
        applyTo->setStyleSheet(ui->plainTextEdit->toPlainText());
}

#include "stylesheetliveeditor.moc"
