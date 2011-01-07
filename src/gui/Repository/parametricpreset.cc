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

Entity::Entity(Package package, QString path)
        : package_(package), path_(path)
{
        reload();
}

Entity::Entity() {
}

QString Entity::readAll (QString filename, quint64 maxlen) const {
        filename = QDir(path_).absolutePath() + "/" + filename;

        QFile f(filename);
        f.open(QFile::ReadOnly);
        return QString (f.read(maxlen));
}

void Entity::writeAll (QString value, QString filename, quint64 maxlen) const {
        filename = QDir(path_).absolutePath() + "/" + filename;

        QFile f(filename);
        f.open (QFile::WriteOnly);
        f.write(value.left(maxlen).toAscii());
}

QString Entity::title() const {
        return title_;
}

QString Entity::author() const {
        return author_;
}

QString Entity::email() const {
        return email_;
}

QString Entity::homepage() const {
        return homepage_;
}

Package Entity::package() const {
        return package_;
}

QString Entity::path() const {
        return path_;
}

QString Entity::name() const {
        return name_;
}

void Entity::setTitle(QString s) {
        title_ = s;
}

void Entity::setAuthor(QString s){
        author_ = s;
}

void Entity::setEmail(QString s) {
        email_ = s;
}

void Entity::setHomepage(QString s) {
        homepage_ = s;
}

void Entity::save() const {
        writeAll (title_, "title");
        writeAll (author_, "author");
        writeAll (email_, "email");
}

void Entity::reload() {
        title_ = readAll ("title");
        author_ = readAll ("author");
        email_ = readAll ("email");
}





ParametricPreset::ParametricPreset(Package package, QString path)
        : Entity (package, path)
{
        reload();
}

ParametricPreset::ParametricPreset() {
}

void ParametricPreset::reload() {
        Entity::reload();
        preset_ = readAll ("preset");
}


void ParametricPreset::save() const {        
        Entity::save();
        writeAll (preset_, "preset");
}

QString ParametricPreset::preset() const {
        return preset_;
}

/*void ParametricPreset::setName(QString s) {
        name_ = s;
}*/

void ParametricPreset::setPreset(QString s) {
        preset_ = s;
}
