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

#include "package.hh"
#include <stdexcept>
#include <QDir>
#include <QMessageBox>
#include "parametricpreset.hh"


Package::Package(QString root) : root_(root) {
        if (!QDir(root).exists()) {
                const std::string msg = "Package \""
                        + root.toStdString()
                        + "\" not found, "
                        + "you won't see any items from that package.";
                QMessageBox::critical(0, "Package not found",
                                      QString::fromStdString(msg));
        }
}

QString Package::root() const {
        return root_;
}

void Package::allParametricPresets(QVector<ParametricPreset> &ret, bool clear) const {
        QDir folder  (QDir(root_).absolutePath()+"/parametric-presets");

        if (clear) ret.clear();
        foreach (QString str,
                 folder.entryList(QDir::NoDotAndDotDot | QDir::AllDirs))
        {
                ret.push_back(
                        ParametricPreset (*this,
                                          folder.absolutePath() + "/" + str));
        }
}

QVector<ParametricPreset> Package::allParametricPresets() const {
        QVector<ParametricPreset> ret;
        allParametricPresets(ret);
        return ret;
}
