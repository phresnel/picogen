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

#ifndef MATERIAL_HH
#define MATERIAL_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
    class Material;
}
class ColorPickerColor;

namespace picogen { namespace qt4 {

class Material : public QWidget
{
        Q_OBJECT
// ================== Public ===================================================
public:
        explicit Material(QWidget *parent = 0);
        virtual ~Material();

        void setMaterial (redshift::shared_ptr<cosyscene::Material>,
                          bool blockSignals);

signals:
        void materialChanged();


// ================== Private ==================================================
private slots:
        void on_colorPicker_colorChanged(ColorPickerColor const &);

        void on_stashButton_clicked       ();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked  ();

private:
        Ui::Material *ui;
        redshift::shared_ptr<cosyscene::Material> material;
};

} }

#endif // MATERIAL_HH
