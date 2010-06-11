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

#include "picoden.hh"
#include "ui_picoden.h"

#include <QDir>
#include <QLabel>

PicoDen::PicoDen(QString const & repositoryRoot,
                 QWidget *parent
)
: QWidget(parent)
, ui(new Ui::PicoDen)
{
        ui->setupUi(this);
        setRoot (repositoryRoot);
}



PicoDen::PicoDen(QWidget *parent)
: QWidget(parent)
, ui(new Ui::PicoDen)
, repositoryRoot("")
{
        ui->setupUi(this);
}



PicoDen::~PicoDen() {
        delete ui;
}



void PicoDen::setRoot (QString const &root) {
        repositoryRoot = root;
        if (!repositoryRoot.endsWith("/"))
                repositoryRoot += "/";
        showScenes();
}



void PicoDen::showScenes() {
        QDir dir (repositoryRoot + "scenes/");
        const QStringList dirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

        ui->filter->clear();
        ui->filter->addItems(dirs);
}



void PicoDen::on_filter_currentIndexChanged(QString d) {
        QString const rel = repositoryRoot + "scenes/" + d + "/";
        QDir dir (rel);
        const QStringList dirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

        ui->scenesTable->setRowCount(dirs.length());

        ui->scenesTable->setColumnWidth(0, 320);

        for (int row=0; row<dirs.length(); ++row) {

                // Thumbnail?
                const QString thumbName = rel + dirs[row] + "/thumb.png";
                if (QFile::exists(thumbName)) {
                        QLabel *thumb = new QLabel (this);
                        QPixmap pix(thumbName);
                        thumb->setPixmap(pix);
                        ui->scenesTable->setCellWidget(row, 0, thumb);
                        ui->scenesTable->setRowHeight(row, pix.height());
                } else {
                }

                // Title.
                ui->scenesTable->setCellWidget(row, 1,
                                               new QLabel(dirs[row], this)
                                              );
        }
}
