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

namespace picogen_repository {

Fragment::Fragment(Collection package, QString path)
        : package_(package), path_(path)
{
        reload();
}

Fragment::Fragment() {
}

QString Fragment::readAll (QString filename, quint64 maxlen) const {
        filename = QDir(path_).absolutePath() + "/" + filename;

        QFile f(filename);
        f.open(QFile::ReadOnly);
        return QString (f.read(maxlen));
}

void Fragment::writeAll (QString value, QString filename, quint64 maxlen) const {

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

QString Fragment::title() const {
        return title_;
}

QString Fragment::author() const {
        return author_;
}

QString Fragment::email() const {
        return email_;
}

QString Fragment::homepage() const {
        return homepage_;
}

Collection Fragment::package() const {
        return package_;
}

QString Fragment::path() const {
        return path_;
}

QString Fragment::name() const {
        return name_;
}

void Fragment::setTitle(QString s) {
        title_ = s;
}

void Fragment::setAuthor(QString s){
        author_ = s;
}

void Fragment::setEmail(QString s) {
        email_ = s;
}

void Fragment::setHomepage(QString s) {
        homepage_ = s;
}

void Fragment::save() const {
        writeAll (title_, "title");
        writeAll (author_, "author");
        writeAll (email_, "email");
        writeAll (homepage_, "homepage");
}

void Fragment::reload() {
        title_ = readAll ("title");
        author_ = readAll ("author");
        email_ = readAll ("email");
        homepage_ = readAll ("homepage");
}





ParametricPreset::ParametricPreset(Collection package, QString path)
        : Fragment (package, path)
{
        reload();
}

ParametricPreset::ParametricPreset() {
}

void ParametricPreset::reload() {
        Fragment::reload();
        preset_ = readAll ("preset");
}


void ParametricPreset::save() const {        
        Fragment::save();
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

} // namespace picogen_repository {
