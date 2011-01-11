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
#include <QDebug>


//------------------------------------------------------------------------------
namespace {
        QString userApplicationDataPath() {
                QSettings userSettings (QSettings::IniFormat,
                                        QSettings::UserScope,
                                        "picogen-team",
                                        "picogen");
                return QFileInfo(userSettings.fileName()).absolutePath();
        }

        QString systemApplicationDataPath() {
                QSettings userSettings (QSettings::IniFormat,
                                        QSettings::SystemScope,
                                        "picogen-team",
                                        "picogen");
                return QFileInfo(userSettings.fileName()).absolutePath();
        }



        QString rootCollectionsCommandLineOverride() {
                const QStringList &args = QApplication::arguments();
                QString cliOverride;
                for (QStringList::const_iterator it=args.begin(), end=args.end();
                        it != end; ++it)
                {
                        if (*it == "--root-collections-directory") {
                                ++it;
                                if (it != end) return *it;
                                return "";
                        }
                }

                return "";
        }

        QString rootCollectionsAskUser() {
                QMessageBox mb;
                mb.setWindowTitle("Error");
                mb.setText("No root-collections-directory has been found, some "
                           "features will be missing.\n"
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
                                return fd.directory().absolutePath();
                        }
                }
                return "";
        }

        QString rootCollectionsDir() {

                //==> CLI override.
                QString tmp = rootCollectionsCommandLineOverride();
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Application folder (TODO: disable on Linux?).
                tmp = QApplication::applicationDirPath() + "/collections";
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Systemwide settings.
                tmp = systemApplicationDataPath() + "/collections";
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Ask user.
                tmp = rootCollectionsAskUser();
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                return "";
        }



        void addCollections (QString root,
                             QList<picogen_repository::Collection> &collections_
                             )
        {
                using picogen_repository::Collection;

                //==> Explicitly added collections.
                QFile acf(root + "/extern");
                if (acf.open(QFile::Text | QFile::ReadOnly)) {
                        QTextStream str (&acf);
                        while (!str.atEnd()) {
                                QString tmp = str.readLine();
                                if (tmp.trimmed()[0] == '#') continue;
                                tmp = QDir(tmp).absolutePath();
                                collections_.push_back(Collection(tmp));
                        }
                        acf.close();
                }

                //==> Collections from collections-folder.
                QDir directory = QDir(root + "/");
                QStringList found;
                found = directory.entryList(QStringList("*"),
                                            QDir::AllDirs |
                                            QDir::Readable |
                                            QDir::NoDotAndDotDot);
                foreach (QString tmp, found) {
                        tmp = directory.absolutePath() + "/" + tmp;
                        collections_.push_back(Collection(tmp));
                }
        }
}
//------------------------------------------------------------------------------


RepositorySettings::RepositorySettings() {
        using picogen_repository::Collection;

        QString rootCollectionsDir = ::rootCollectionsDir();
        if ("" != rootCollectionsDir) {
                addCollections (rootCollectionsDir,
                                collections_);
        }
        addCollections (userApplicationDataPath() + "/collections",
                        collections_);


        QString info;
        foreach (Collection c, collections_)
                info += c.root() + "\n";
        QMessageBox mb;
        mb.setText(info);
        mb.exec();
}

RepositorySettings::~RepositorySettings() {
}
