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

#ifndef DATABASE_HH
#define DATABASE_HH

#include <QVector>

#include "collection.hh"

namespace picogen_repository {

class ParametricPreset;

// A Database is the overlay of an arbitrary number of Packages, but basically
// has the same interface as a Package.
class Database {
public:
        void establishStandardConnection();

        void addCollection(Collection const &);
        void addCollections(QList<Collection> const &);

        void allParametricPresets(QVector<ParametricPreset> &ret,
                                  bool clear=true) const;
        QVector<ParametricPreset> allParametricPresets() const;
private:
        QVector<Collection> packages_;
};

} // namespace picogen_repository {

#endif // DATABASE_HH
