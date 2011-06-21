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

#include "terrainfitting.h"
#include "ui_terrainfitting.h"
#include "cosyscene/terrain.hh"
#include "scopedblocksignals.h"
#include "stashview.h"

#include <QMessageBox>

namespace picogen { namespace qt4 {

TerrainFitting::TerrainFitting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerrainFitting),
    fitting_(new cosyscene::TerrainFitting())
{
        ui->setupUi(this);
        previousMaxRecursion = ui->maxRecursion->value();
}

TerrainFitting::~TerrainFitting() {
        delete ui;
}

void TerrainFitting::setFitting(
        redshift::shared_ptr<cosyscene::TerrainFitting> t,
        bool blockSignals)
{
        ScopedQtSignalBlock blockMe (this, blockSignals);

        fitting_ = t;
        cosyscene::TerrainFitting const &f = *t;

        ScopedQtSignalBlock blockMaxRecursion (ui->maxRecursion, blockSignals);
        ui->maxRecursion->setValue(f.lazyQuadtreeMaxRecursion());

        ScopedQtSignalBlock blockVisibleExtent (ui->visibleExtent, blockSignals);
        ui->visibleExtent->setValue(f.lazyQuadtreeVisibleExtent().toDouble());

        ScopedQtSignalBlock blockDetailCoeff(ui->detailCoefficient, blockSignals);
        ui->detailCoefficient->setValue(f.detailCoefficient().toDouble());
}

void TerrainFitting::on_visibleExtent_valueChanged(double) {
        fitting_->setLazyQuadtreeVisibleExtent (ui->visibleExtent->value());
        emit fittingChanged();
}


void TerrainFitting::on_maxRecursion_valueChanged(int) {
        const int val = ui->maxRecursion->value();

        // Larger quadtrees can become gigabytes in size. Therefore warn users.
        if (val >= 16 &&
            val > previousMaxRecursion &&
            previousMaxRecursion<16
        ) {
                QMessageBox::warning(this, "Warning",
                "Note that large values might cause an out-of-memory situation."
                "\n\n"
                "Before rendering, make sure that you have enough free memory "
                "and that you saved all your data, ideally closing other "
                "applications.");
        }
        previousMaxRecursion = val;
        fitting_->setLazyQuadtreeMaxRecursion (val);

        emit fittingChanged();
}

void TerrainFitting::on_detailCoefficient_valueChanged(double) {
        fitting_->setDetailCoefficient (ui->detailCoefficient->value());
        emit fittingChanged();
}


void TerrainFitting::on_stashButton_clicked() {
        if (StashView::StashDialog (this, fitting_)) {
                emit fittingChanged();
        }
}
void TerrainFitting::on_stashRestoreButton_clicked() {
        if (StashView::RestoreDialog (this, fitting_)) {
                setFitting(fitting_, true);
                emit fittingChanged();
        }
}
void TerrainFitting::on_stashResetButton_clicked() {
        if (StashView::ResetDialog(this, fitting_)) {
                setFitting(fitting_, true);
                emit fittingChanged();
        }
}

} }

#include "terrainfitting.moc"
