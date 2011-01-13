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

#include "previewmainwindow.hh"
#include "ui_previewmainwindow.h"

#include <QImage>

PreviewMainWindow::PreviewMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreviewMainWindow)
{
        ui->setupUi(this);
        ui->centralWidget->addImage(QImage("C:/Dokumente und Einstellungen/All Users/Dokumente/Eigene Bilder/Beispielbilder/Blaue Berge.jpg"));
        ui->centralWidget->addImage(QImage("C:/Dokumente und Einstellungen/All Users/Dokumente/Eigene Bilder/Beispielbilder/Sonnenuntergang.jpg"));
        ui->centralWidget->addImage(QImage("C:/Dokumente und Einstellungen/All Users/Dokumente/Eigene Bilder/Beispielbilder/Wasserlilien.jpg"));
        ui->centralWidget->addImage(QImage("C:/Dokumente und Einstellungen/All Users/Dokumente/Eigene Bilder/Beispielbilder/Winter.jpg"));
        ui->centralWidget->startTimer(3000);
}

PreviewMainWindow::~PreviewMainWindow()
{
        delete ui;
}
