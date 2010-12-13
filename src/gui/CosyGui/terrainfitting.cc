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

#include "terrainfitting.hh"
#include "ui_terrainfitting.h"

#include <QMessageBox>

TerrainFitting::TerrainFitting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TerrainFitting)
{
        ui->setupUi(this);
        previousMaxRecursion = ui->maxRecursion->value();
}

TerrainFitting::~TerrainFitting() {
        delete ui;
}

void TerrainFitting::on_maxRecursion_valueChanged(int val) {
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
}
