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

#ifndef STASHFRAME_HH
#define STASHFRAME_HH

#include <QFrame>
#include "redshift/include/sealed.hh"
class QPixmap;
namespace Ui {
        class StashFrame;
}

namespace picogen { namespace qt4_gui {

SEALED(StashFrame);
class StashFrame : public QFrame, MAKE_SEALED(StashFrame)
{
        Q_OBJECT
public:
        explicit StashFrame(QWidget *parent = 0);
        ~StashFrame();

        void setTitle (QString title);
        void setPixmap (QString name);

private:
        Ui::StashFrame *ui;
};



// Standard confirmation dialogs. TODO: stuff together with with StashView-
//                                      standard dialogs.
enum ConfirmReset {
        ConfirmReset_Abort,
        ConfirmReset_StashBeforeReset,
        ConfirmReset_Reset
};
enum ConfirmRestash {
        ConfirmRestash_Abort,
        ConfirmRestash_RestashAndKillOld,
        ConfirmRestash_RestashAndKeepOld
};
enum ConfirmRestore {
        ConfirmRestore_Abort,
        ConfirmRestore_StashBeforeRestore,
        ConfirmRestore_Restore
};

ConfirmReset   confirmReset   (QWidget *parent);
ConfirmRestash confirmRestash (QWidget *parent);
ConfirmRestore confirmRestore (QWidget *parent);

} }

#endif // STASHFRAME_HH
