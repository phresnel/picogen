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

#include "stashframe.hh"
#include "ui_stashframe.h"
#include "redshift/include/smart_ptr.hh"
#include <QMessageBox>


ConfirmReset confirmReset (QWidget *parent) {
        redshift::shared_ptr<QMessageBox> msgBox (new QMessageBox (parent));
        QPushButton *resetButton = msgBox->addButton("Reset now!", QMessageBox::ActionRole);
        QPushButton *stashButton = msgBox->addButton("Stash, then reset", QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox->addButton(QMessageBox::Abort);

        msgBox->setDefaultButton(abortButton);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("This deletes your settings.");
        msgBox->exec();
        if (msgBox->clickedButton() == (QAbstractButton*)stashButton) {
                return ConfirmReset_StashBeforeReset;
        } else if (msgBox->clickedButton() == (QAbstractButton*)resetButton) {
                return ConfirmReset_Reset;
        }
        return ConfirmReset_Abort;
}


StashFrame::StashFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StashFrame)
{
    ui->setupUi(this);
}

StashFrame::~StashFrame()
{
    delete ui;
}