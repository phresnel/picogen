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


#include "openscenefile.hh"
#include "ui_openscenefile.h"
#include <QMessageBox>

OpenSceneFile::OpenSceneFile(
                QString const &path,
                bool warnOnOverwrite,
                QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenSceneFile),
    path(path),
    warnOnOverwrite(warnOnOverwrite)
{
        ui->setupUi(this);
        result_ = Cancel;
}



OpenSceneFile::~OpenSceneFile() {
        delete ui;
}



void OpenSceneFile::changeEvent(QEvent *e) {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void OpenSceneFile::on_newInstance_clicked() {
        result_ = OpenInNewInstance;
        close();
}



void OpenSceneFile::on_currentInstance_clicked() {
        if (!warnOnOverwrite ||
            QMessageBox::question(
                this, "Confirm",
                "This will overwrite your current scene\n"
                "without mercy, without return."
                "\n\nProceed?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No
            ) == QMessageBox::Yes
        ) {
                result_ = OpenInCurrentInstance;
                close();
        }
}



void OpenSceneFile::on_cancel_clicked() {
        result_ = Cancel;
        close();
}
