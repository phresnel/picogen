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

#ifndef PRESETLISTITEMWIDGET_HH
#define PRESETLISTITEMWIDGET_HH

#include <QWidget>
#include "parametricpreset.hh"

namespace Ui {
    class PresetListItemWidget;
}

namespace picogen { namespace qt4_gui {

class PresetListItemWidget : public QWidget
{
        Q_OBJECT

public:
        explicit PresetListItemWidget(QWidget *parent = 0);
        explicit PresetListItemWidget(repository::ParametricPreset const &,
                                      QWidget *parent = 0);
        ~PresetListItemWidget();

        repository::ParametricPreset preset() const;
        void setPreset (repository::ParametricPreset const &);

        void setReadOnly (bool readOnly);

private:
        Ui::PresetListItemWidget *ui;
        repository::ParametricPreset preset_;
        bool readOnly_;

private slots:
        void on_toolButton_clicked();
};

} }

#endif // PRESETLISTITEMWIDGET_HH

