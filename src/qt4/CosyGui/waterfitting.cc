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

#include "waterfitting.h"
#include "ui_waterfitting.h"
#include "cosyscene/water.hh"
#include "scopedblocksignals.h"
#include "stashview.h"

#include <QMessageBox>

WaterFitting::WaterFitting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaterFitting),
    fitting_(new cosyscene::WaterFitting())
{
        ui->setupUi(this);
}

WaterFitting::~WaterFitting() {
        delete ui;
}

void WaterFitting::setFitting(
        redshift::shared_ptr<cosyscene::WaterFitting> t,
        bool blockSignals)
{
        ScopedQtSignalBlock blockMe (this, blockSignals);

        fitting_ = t;
        cosyscene::WaterFitting const &f = *t;

        ScopedQtSignalBlock blockSeaLevel (ui->seaLevel, blockSignals);
        ui->seaLevel->setValue(f.seaLevel().toDouble());
}

void WaterFitting::on_seaLevel_valueChanged(double val) {
        fitting_->setSeaLevel(val);
        emit fittingChanged();
}


void WaterFitting::on_stashButton_clicked() {
        if (StashView::StashDialog (this, fitting_)) {
                emit fittingChanged();
        }
}
void WaterFitting::on_stashRestoreButton_clicked() {
        if (StashView::RestoreDialog (this, fitting_)) {
                setFitting(fitting_, true);
                emit fittingChanged();
        }
}
void WaterFitting::on_stashResetButton_clicked() {
        if (StashView::ResetDialog(this, fitting_)) {
                setFitting(fitting_, true);
                emit fittingChanged();
        }
}


#include "waterfitting.moc"
