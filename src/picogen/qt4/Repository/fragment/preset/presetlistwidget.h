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

#ifndef PRESETLISTWIDGET_HH
#define PRESETLISTWIDGET_HH

#include <QWidget>
#include "database.hh"
#include "parametricpreset.hh"
#include "../../shared_ptr.h"
#include "../../optional.h"

namespace Ui {
    class PresetListWidget;
}

namespace picogen { namespace qt4_gui {

class PresetListWidget : public QWidget
{
        Q_OBJECT

public:
        explicit PresetListWidget(QWidget *parent = 0);
        ~PresetListWidget();

        void setDatabase (shared_ptr<repository::Database>);
        optional<repository::ParametricPreset> lastSelected() const;

private:
        Ui::PresetListWidget *ui;
        shared_ptr<repository::Database> database_;
        optional<repository::ParametricPreset> lastSelected_;

private:
        void resyncView();
        void addPresetToView(repository::ParametricPreset const&);

private slots:
        void on_readOnly_toggled(bool checked);
        void on_listWidget_itemSelectionChanged();
};

} }

#endif // PRESETLISTWIDGET_HH

