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

#include <QDir>
#include <QFile>
#include <QDebug>

#include "metadata.hh"

namespace picogen { namespace qt4 {

Meta::Meta(QString path)
        : path_(path)
{
        reload();
}

Meta::Meta() {
}

QString Meta::read (QString filename, quint64 maxlen) const {
        filename = QDir(path_).absolutePath() + "/" + filename;

        QFile f(filename);
        f.open(QFile::ReadOnly);
        return QString (f.read(maxlen));
}

void Meta::write (QString value, QString filename, quint64 maxlen) const {

        filename = QDir(path_).absolutePath() + "/" + filename;

        QFile f(filename);

        if (value.simplified() != "") {
                f.open (QFile::WriteOnly);
                f.write(value.left(maxlen).toAscii());
        } else {
                if (f.exists())
                        f.remove();
        }
}

QString Meta::title() const {
        return title_;
}

QString Meta::author() const {
        return author_;
}

QString Meta::email() const {
        return email_;
}

QString Meta::homepage() const {
        return homepage_;
}

QString Meta::path() const {
        return path_;
}

QStringList Meta::previewFilenames() const {
        return previewFilenames_;
}

void Meta::setTitle(QString s) {
        title_ = s;
}

void Meta::setAuthor(QString s){
        author_ = s;
}

void Meta::setEmail(QString s) {
        email_ = s;
}

void Meta::setHomepage(QString s) {
        homepage_ = s;
}

void Meta::save() const {
        write (title_, "title");
        write (author_, "author");
        write (email_, "email");
        write (homepage_, "homepage");
}

void Meta::reload() {
        title_ = read ("title");
        author_ = read ("author");
        email_ = read ("email");
        homepage_ = read ("homepage");

        QString previewFolder = QDir(path_).absolutePath() + "/preview/";

        QDir previewDir = QDir(previewFolder);
        QStringList filenames = previewDir
                                .entryList(QStringList()
                                           <<"*.jpg"
                                           <<"*.png",
                                           QDir::Files, QDir::Name);
        previewFilenames_.clear();
        foreach (QString f, filenames)
                previewFilenames_.push_back(previewDir.absoluteFilePath(f));
}

} }
