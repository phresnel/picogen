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

#include "stashframe.h"
#include "ui_stashframe.h"
#include "redshift/include/smart_ptr.hh"
#include <QMessageBox>
#include <QPixmap>


ConfirmRestash confirmRestash (QWidget *parent) {
        redshift::shared_ptr<QMessageBox> msgBox (new QMessageBox (parent));
        QPushButton *restashKill = msgBox->addButton("Remove identical", QMessageBox::ActionRole);
        QPushButton *restashKeep = msgBox->addButton("Keep all", QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox->addButton(QMessageBox::Abort);


        msgBox->setDefaultButton(abortButton);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("There is already an identical item in the stash.");
        msgBox->exec();
        if (msgBox->clickedButton() == (QAbstractButton*)restashKill) {
                return ConfirmRestash_RestashAndKillOld;
        } else if (msgBox->clickedButton() == (QAbstractButton*)restashKeep) {
                return ConfirmRestash_RestashAndKeepOld;
        }
        return ConfirmRestash_Abort;
}

ConfirmReset confirmReset (QWidget *parent) {
        redshift::shared_ptr<QMessageBox> msgBox (new QMessageBox (parent));
        QPushButton *resetButton = msgBox->addButton("Don't backup", QMessageBox::ActionRole);
        QPushButton *stashButton = msgBox->addButton("Backup", QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox->addButton(QMessageBox::Abort);

        msgBox->setDefaultButton(abortButton);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("This form is unsaved and will be overwritten.");
        msgBox->exec();
        if (msgBox->clickedButton() == (QAbstractButton*)stashButton) {
                return ConfirmReset_StashBeforeReset;
        } else if (msgBox->clickedButton() == (QAbstractButton*)resetButton) {
                return ConfirmReset_Reset;
        }
        return ConfirmReset_Abort;
}

ConfirmRestore confirmRestore (QWidget *parent) {
        redshift::shared_ptr<QMessageBox> msgBox (new QMessageBox (parent));
        QPushButton *restoreButton = msgBox->addButton("Don't backup", QMessageBox::ActionRole);
        QPushButton *stashButton = msgBox->addButton("Backup", QMessageBox::ActionRole);
        QPushButton *abortButton = msgBox->addButton(QMessageBox::Abort);

        msgBox->setDefaultButton(abortButton);
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("This form is unsaved and will be overwritten.");
        msgBox->exec();
        if (msgBox->clickedButton() == (QAbstractButton*)stashButton) {
                return ConfirmRestore_StashBeforeRestore;
        } else if (msgBox->clickedButton() == (QAbstractButton*)restoreButton) {
                return ConfirmRestore_Restore;
        }
        return ConfirmRestore_Abort;
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

void StashFrame::setTitle (QString title)
{
        ui->title->setText(title);
}

void StashFrame::setPixmap (QString name)
{
        ui->icon->setPixmap(
                QPixmap(name).scaledToHeight(ui->title->height(),
                                             Qt::SmoothTransformation));
}

#include "stashframe.moc"
