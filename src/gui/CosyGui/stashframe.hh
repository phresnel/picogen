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
class QPixmap;

namespace Ui {
    class StashFrame;
}

class StashFrame : public QFrame
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

enum ConfirmReset {
        ConfirmReset_Abort,
        ConfirmReset_StashBeforeReset,
        ConfirmReset_Reset
};
ConfirmReset confirmReset (QWidget *parent);

#endif // STASHFRAME_HH
