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

#include "filenameedit.h"
#include "ui_filenameedit.h"
#include <QFileDialog>

namespace picogen { namespace qt4 {

FilenameEdit::FilenameEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilenameEdit)
{
        ui->setupUi(this);
}

FilenameEdit::~FilenameEdit() {
        delete ui;
}

void FilenameEdit::on_toolButton_clicked() {
        QFileDialog d (this);
        if (d.exec() == QDialog::Accepted)
                ui->lineEdit->setText(d.selectedFiles()[0]);
}

QString FilenameEdit::filename() const {
        return ui->lineEdit->text();
}

void FilenameEdit::setFilename (QString const &str) {
        ui->lineEdit->setText(str);
}

} }

#include "filenameedit.moc"
