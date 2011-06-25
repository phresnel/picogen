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

#include "database.hh"
#include "collection.hh"
#include "parametricpreset.hh"
#include "repositorysettings.hh"

namespace picogen { namespace repository {

void Database::establishStandardConnection() {
        addCollections (RepositorySettings().collections());
}

void Database::addCollection(Collection const &package) {
        packages_.push_back(package);
}

void Database::addCollections(QList<Collection> const &collections) {
        foreach (Collection c, collections)
                addCollection (c);
}

void Database::terrainFormationPresets(
        QVector<ParametricPreset> &ret,
        bool clear
) const {
        if (clear) ret.clear();
        foreach (Collection pack, packages_) {
                pack.terrainFormationPresets(ret, false);
        }
}

QVector<ParametricPreset> Database::terrainFormationPresets() const {
        QVector<ParametricPreset> ret;
        terrainFormationPresets(ret);
        return ret;
}

} }
