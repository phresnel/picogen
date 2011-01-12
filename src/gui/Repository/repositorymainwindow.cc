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

#include "repositorymainwindow.hh"
#include "ui_repositorymainwindow.h"

//#include "collection.hh"
//#include "parametricpreset.hh"
#include "shared_ptr.hh"

#include <QDir>
#include <QMessageBox>
#include <QDebug>

namespace picogen_repository {

RepositoryMainWindow::RepositoryMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RepositoryMainWindow)
{
        ui->setupUi(this);
        shared_ptr<Database> db(new Database());
        db->establishStandardConnection();
        ui->parametricPresets->setDatabase(db);
}

RepositoryMainWindow::~RepositoryMainWindow() {
        delete ui;
}

} // namespace picogen_repository {
