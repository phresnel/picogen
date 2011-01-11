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

#include "repositorysettings.hh"

#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>


//------------------------------------------------------------------------------
namespace {
        QString rootCollectionCommandLineOverride() {
                const QStringList &args = QApplication::arguments();
                QString cliOverride;
                for (QStringList::const_iterator it=args.begin(), end=args.end();
                        it != end; ++it)
                {
                        if (*it == "--root-collection") {
                                ++it;
                                if (it != end) return *it;
                                return "";
                        }
                }

                return "";
        }

        QString rootCollectionAskUser() {
                QMessageBox mb;
                mb.setWindowTitle("Error");
                mb.setText("No root-collection has been found, some features "
                           " will be missing.\n"
                           "If you are an end-user, you might want to report "
                           "this at http://picogen.org/forum");
                mb.addButton(QMessageBox::Ignore);
                QPushButton *provideNow = mb.addButton(
                           "Provide path (only if you know what this means)",
                           QMessageBox::AcceptRole);
                mb.exec();
                if (provideNow == mb.clickedButton()) {
                        QFileDialog fd;
                        fd.setFileMode(QFileDialog::DirectoryOnly);
                        fd.setWindowTitle("Select a root collection");
                        if (QDialog::Accepted == fd.exec()) {
                                return fd.directory().dirName();
                        }
                }
                return "";
        }

        QString rootCollectionDir() {

                //==> CLI override
                QString tmp = rootCollectionCommandLineOverride();
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Application folder (TODO: disable on Linux?)
                tmp = QApplication::applicationDirPath() + "/root-collection";
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Ask user.
                tmp = rootCollectionAskUser();
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                return "";
        }
}
//------------------------------------------------------------------------------


RepositorySettings::RepositorySettings() {
        QDir *rootDir = new QDir (rootCollectionDir());
        delete rootDir;
}

RepositorySettings::~RepositorySettings() {
}
