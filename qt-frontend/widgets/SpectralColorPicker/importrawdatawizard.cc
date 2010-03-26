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

#include "importrawdatawizard.hh"
#include "ui_importrawdatawizard.h"

#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>


ImportRawDataWizard::ImportRawDataWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::ImportRawDataWizard)
{
    ui->setupUi(this);
}



ImportRawDataWizard::~ImportRawDataWizard() {
        delete ui;
}



void ImportRawDataWizard::changeEvent(QEvent *e) {
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void ImportRawDataWizard::on_openFileDialog_pressed() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);

        dialog.setWindowTitle("Select a file");

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (dialog.exec()) {
                ui->filename->setText(dialog.selectedFiles()[0]);
        }
}
