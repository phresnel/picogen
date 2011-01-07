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

#include <QVector>

#include "parametricpreset.hh"
#include "parametricpresetui.hh"

#include "parametricpresetsui.hh"
#include "ui_parametricpresetsui.h"

#include <QDebug>

ParametricPresetsUi::ParametricPresetsUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametricPresetsUi),
    database_(new Database())
{
        ui->setupUi(this);
}

ParametricPresetsUi::~ParametricPresetsUi()
{
        delete ui;
}

void ParametricPresetsUi::setDatabase (shared_ptr<Database> db) {
        database_ = db;
        resyncView();
}

void ParametricPresetsUi::resyncView() {
        QVector<ParametricPreset> presets;
        database_->allParametricPresets(presets);
        const int count = presets.count();

        ui->listWidget->clear();

        for (int i=0; i<count; ++i) {
                const ParametricPreset& pp = presets[i];
                addPresetToView(pp);
        }
}

void ParametricPresetsUi::addPresetToView(const ParametricPreset &pp) {
        QListWidgetItem *it = new QListWidgetItem (ui->listWidget);
        ParametricPresetUi *ppui = new ParametricPresetUi (pp, this);

        ui->listWidget->setItemWidget(it, ppui);
        it->setSizeHint(ppui->sizeHint());

        ui->listWidget->scrollToItem(it);
}

/*void ParametricPresetsUi::on_saveButton_clicked() {
        for (int i=0; i<ui->listWidget->count(); ++i) {
                QListWidgetItem *it = ui->listWidget->item(i);
                ParametricPresetUi* ppui =
                        qobject_cast<ParametricPresetUi*>(
                          ui->listWidget->itemWidget(it));
                if (0 != ppui)
                        ppui->preset().save();
        }
}*/
