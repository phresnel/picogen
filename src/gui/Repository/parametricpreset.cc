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

#include "parametricpreset.hh"

#include <QDir>
#include <QFile>

QString readAll (QString filename, quint64 maxlen=1024) {
        QFile f(filename);
        f.open(QFile::ReadOnly);
        return QString (f.read(maxlen));
}

void writeAll (QString value, QString filename, quint64 maxlen=1024) {
        QFile f(filename);
        f.open (QFile::WriteOnly);
        f.write(value.left(maxlen).toAscii());
}


ParametricPreset::ParametricPreset(Package package, QString path)
        : package_(package), path_(path)
{
        QDir dir (path_);
        name_ = path_.split("/").last();
        title_ = readAll (dir.absolutePath() + "/title");
        author_ = readAll (dir.absolutePath() + "/author");
        email_ = readAll (dir.absolutePath() + "/email");
        preset_ = readAll (dir.absolutePath() + "/preset");
}


void ParametricPreset::save() const {
        QDir dir (path_);
        writeAll (title_, dir.absolutePath() + "/title");
        writeAll (author_, dir.absolutePath() + "/author");
        writeAll (email_, dir.absolutePath() + "/email");
        writeAll (preset_, dir.absolutePath() + "/preset");
}

Package ParametricPreset::package() const {
        return package_;
}

QString ParametricPreset::path() const {
        return path_;
}

QString ParametricPreset::name() const {
        return name_;
}

QString ParametricPreset::title() const {
        return title_;
}

QString ParametricPreset::author() const {
        return author_;
}

QString ParametricPreset::email() const {
        return email_;
}

QString ParametricPreset::preset() const {
        return preset_;
}

/*void ParametricPreset::setName(QString s) {
        name_ = s;
}*/

void ParametricPreset::setTitle(QString s) {
        title_ = s;
}

void ParametricPreset::setAuthor(QString s){
        author_ = s;
}

void ParametricPreset::setEmail(QString s) {
        email_ = s;
}

void ParametricPreset::setPreset(QString s) {
        preset_ = s;
}
