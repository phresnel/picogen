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
#include "presetlistitemwidget.h"

#include "presetlistwidget.h"
#include "ui_presetlistwidget.h"

#include <QDebug>

namespace picogen { namespace qt4 {

PresetListWidget::PresetListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PresetListWidget),
    database_(new Database()),
    lastSelected_(optional<ParametricPreset>())
{
        ui->setupUi(this);
        ui->previewWidget->startTimer(3000);
}

PresetListWidget::~PresetListWidget()
{
        delete ui;
}

void PresetListWidget::setDatabase (shared_ptr<Database> db) {
        database_ = db;
        resyncView();
}

void PresetListWidget::resyncView() {
        QVector<ParametricPreset> presets;
        database_->terrainFormationPresets(presets);
        const int count = presets.count();

        ui->listWidget->clear();

        for (int i=0; i<count; ++i) {
                const ParametricPreset& pp = presets[i];
                addPresetToView(pp);
        }
}

void PresetListWidget::addPresetToView(const ParametricPreset &pp) {
        QListWidgetItem *it = new QListWidgetItem (ui->listWidget);
        PresetListItemWidget *ppui = new PresetListItemWidget (pp, this);

        ppui->setReadOnly(ui->readOnly->isChecked());
        ui->listWidget->setItemWidget(it, ppui);
        it->setSizeHint(ppui->sizeHint());

        ui->listWidget->scrollToItem(it);
}

optional<ParametricPreset> PresetListWidget::lastSelected() const {
        return lastSelected_;
}

void PresetListWidget::on_listWidget_itemSelectionChanged() {
        const QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
        if (0 == items.count()) {
                lastSelected_ = optional<ParametricPreset>();
        } else {
                QWidget *widget = ui->listWidget->itemWidget(items[0]);
                PresetListItemWidget *ppui = qobject_cast<PresetListItemWidget*> (widget);
                lastSelected_ = 0 != ppui ?
                                ppui->preset() :
                                optional<ParametricPreset>();
                if (ppui) {
                        lastSelected_ = ppui->preset();
                        ui->previewWidget->setImages(
                                        ppui->preset().previewFilenames());
                        qDebug() << ppui->preset().previewFilenames();
                } else {
                        lastSelected_ = optional<ParametricPreset>();
                        ui->previewWidget->reset();
                }
        }
}

void PresetListWidget::on_readOnly_toggled(bool checked) {
        const int count = ui->listWidget->count();
        for (int row=0; row<count; ++row) {
                QListWidgetItem *item = ui->listWidget->item(row);
                QWidget *widget = ui->listWidget->itemWidget(item);
                PresetListItemWidget *ppui = qobject_cast<PresetListItemWidget*> (widget);
                ppui->setReadOnly(checked);
        }
}

} }

#include "presetlistwidget.moc"

