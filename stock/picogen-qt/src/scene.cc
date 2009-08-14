//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include "../include/scene.hh"
//#include "../include/heightmap-layers.hh"


namespace scene { namespace heightmap {


////////////////////////////////////////////////////////////////////////////////
// Composition
////////////////////////////////////////////////////////////////////////////////
Composition::Composition (SyncToView* syncToView_)
        : syncToView(syncToView_)
{
}

Composition::Composition ()
        : syncToView()
{
}

unsigned int Composition::generateId () const {
      again:
        const int id = rand();
        for (size_t i=0; i<data.size(); ++i) {
                if (0 == id || id == data [i].id) {
                        goto again;
                }
        }
        return id;
}

redshift::optional<RowData> Composition::getRowById (int id) const {
        for (size_t i=0; i<data.size(); ++i) {
                if (data [i].id == id)
                        return data [i];
        }
        return false;
}

bool Composition::setRowById (int id, RowData rowData) {
        for (size_t i=0; i<data.size(); ++i) {
                if (data [i].id == id) {
                        data [i] = rowData;
                        if (0 != syncToView)
                                syncToView->syncToView();
                        return true;
                }
        }
        return false;
}

void Composition::setSyncObject (SyncToView* syncToView_) {
        syncToView = syncToView_;
}

void Composition::resetSyncObject () {
        syncToView = 0;
}



} }
