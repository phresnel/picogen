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

#ifndef IRRLICHTRENDERER_HH
#define IRRLICHTRENDERER_HH

namespace irr {
        class IrrlichtDevice;
}

#include <QWidget>

namespace Ui {
    class IrrlichtRenderer;
}

class IrrlichtRenderer
: public QWidget
{
        Q_OBJECT

public:
        explicit IrrlichtRenderer(QWidget *parent = 0);
        ~IrrlichtRenderer();

        void resizeEvent(QResizeEvent *);
        void timerEvent(QTimerEvent *);
protected:


private:
        Ui::IrrlichtRenderer *ui;

        irr::IrrlichtDevice *device;
        bool redraw;

        void drawIrrlicht();
        void initIrrlicht();

private slots:
};

#endif // IRRLICHTRENDERER_HH
