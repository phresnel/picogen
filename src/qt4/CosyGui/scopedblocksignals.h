//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef SCOPEDBLOCKSIGNALS_HH
#define SCOPEDBLOCKSIGNALS_HH

#include <QObject>
#include "redshift/include/sealed.hh"
SEALED(ScopedQtSignalBlock);
class ScopedQtSignalBlock : MAKE_SEALED(ScopedQtSignalBlock) {
public:
        ScopedQtSignalBlock (QObject *obj, bool block)
        : obj(obj), wasBlocked (obj->blockSignals(block))
        {
        }
        ~ScopedQtSignalBlock () {
                obj->blockSignals(wasBlocked);
        }

private:
        ScopedQtSignalBlock();
        ScopedQtSignalBlock(ScopedQtSignalBlock const&);
        ScopedQtSignalBlock& operator=(ScopedQtSignalBlock const&);

        QObject *obj;
        bool wasBlocked;
};

#endif // SCOPEDBLOCKSIGNALS_HH
