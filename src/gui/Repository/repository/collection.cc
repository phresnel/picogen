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

#include "collection.hh"
#include <stdexcept>
#include <QDir>
#include <QMessageBox>
#include "parametricpreset.hh"

namespace picogen_repository {


Collection::Collection(QString root) : root_(root) {
        if (!QDir(root).exists()) {
                const std::string msg = "Collection \""
                        + root.toStdString()
                        + "\" not found, "
                        + "you won't see any items from that Collection.";
                QMessageBox::critical(0, "Collection not found",
                                      QString::fromStdString(msg));
        }
}

QString Collection::root() const {
        return root_;
}

void Collection::terrainFormationPresets(QVector<ParametricPreset> &ret, bool clear) const {
        QDir folder  (QDir(root_).absolutePath()+"/terrain-formation-presets");

        if (clear) ret.clear();
        foreach (QString str,
                 folder.entryList(QDir::NoDotAndDotDot | QDir::AllDirs))
        {
                ret.push_back(
                        ParametricPreset (*this,
                                          folder.absolutePath() + "/" + str));
        }
}

QVector<ParametricPreset> Collection::terrainFormationPresets() const {
        QVector<ParametricPreset> ret;
        terrainFormationPresets(ret);
        return ret;
}

} // namespace picogen_repository {
