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

namespace picogen_repository {

//------------------------------------------------------------------------------
namespace {
        QString userApplicationDataPath() {
                QSettings userSettings (QSettings::IniFormat,
                                        QSettings::UserScope,
                                        "picogen",
                                        "picogen");
                return QFileInfo(userSettings.fileName()).absolutePath();
        }

        QString systemApplicationDataPath() {
                QSettings userSettings (QSettings::IniFormat,
                                        QSettings::SystemScope,
                                        "picogen",
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
                qDebug() << "checking for collections in cli override:" << tmp;
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Application folder (TODO: disable on Linux?).
                tmp = QApplication::applicationDirPath() + "/collections";
                qDebug() << "checking for collections in application directory:" << tmp;
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                //==> Systemwide settings.
                tmp = systemApplicationDataPath() + "/collections";
                qDebug() << "checking for collections in system-wide application data-path:" << tmp;
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;

                return "";
        }


        QString userCollectionsDir() {
                //==> Userwide settings.
                QString tmp = userApplicationDataPath() + "/collections";
                qDebug() << "checking for collections in user-wide application data-path:" << tmp;
                if ("" != tmp && QDir(tmp).exists())
                        return tmp;
                return "";
        }



        void addCollections (QString root,
                             QList<picogen_repository::Collection> &collections_
                             )
        {
                using picogen_repository::Collection;

                // A collections folder looks like this:
                // "collections"
                // -> "extern"  : text file containing absolute paths to collections
                //                not found in this folder
                // -> <folder>  : each folder represents a collection

                //==> Explicitly added collections.
                QFile acf(root + "/extern");
                if (acf.open(QFile::Text | QFile::ReadOnly)) {
                        QTextStream str (&acf);
                        while (!str.atEnd()) {
                                QString tmp = str.readLine().trimmed();
                                if (tmp.length() == 0 || tmp[0] == '#')
                                        continue;
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

        // Try to add standard collections (system-wide, user-wide).
        const QString rcd = rootCollectionsDir();
        if ("" != rcd) {
                addCollections (rcd, collections_);
        }

        const QString ucd = userCollectionsDir();
        if ("" != ucd) {
                addCollections (ucd, collections_);
        }

        // If neither a root-collection nor a user-collection was found,
        // ask user directly.
        if ("" == rcd && "" == ucd) {
                //==> Ask user.
                QString tmp = rootCollectionsAskUser();
                qDebug() << "checking for collections in user-provided path:" << tmp;
                if ("" != tmp && QDir(tmp).exists()) {
                        addCollections (tmp,
                                        collections_);
                }

        }
}

RepositorySettings::~RepositorySettings() {
}

QList<picogen_repository::Collection> RepositorySettings::collections() const {
        return collections_;
}


} // namespace picogen_repository {
