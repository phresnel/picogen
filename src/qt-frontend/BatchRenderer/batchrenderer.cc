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

#include <QtLockedFile>
#include <QFileSystemWatcher>
#include <QDir>
#include <QStringList>
#include <QLabel>

#include "renderjobwidget.hh"

#include "batchrenderer.hh"
#include "ui_batchrenderer.h"



BatchRenderer::BatchRenderer(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::BatchRenderer)
, jobPath("C:/Dokumente und Einstellungen/smach/Eigene Dateien/foo/")
, watcher(new QFileSystemWatcher(QStringList() << jobPath, this))
{
        ui->setupUi(this);

        connect (watcher, SIGNAL(directoryChanged(QString)),
                 this, SLOT(watcher_directoryChanged(QString const &)));

        watcher_directoryChanged(jobPath);
}



BatchRenderer::~BatchRenderer()
{
        delete ui;
}



void BatchRenderer::changeEvent(QEvent *e)
{
        QMainWindow::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void BatchRenderer::watcher_directoryChanged (const QString & path) {
        QDir dir (path);
        dir.setFilter(QDir::Files);
        const QStringList files = dir.entryList();

        int r = 0;
        foreach (QString file, files) {
                if (knownFiles.contains(file))
                        continue;
                QtLockedFile f(dir.absoluteFilePath(file));
                f.open(QIODevice::ReadOnly);
                if (f.lock(QtLockedFile::ReadLock, true)) {
                        knownFiles.insert(file);

                        RenderJobWidget *p = new RenderJobWidget(f, this);
                        if (p->isValid()) {
                                ui->jobView->setRowCount(r+1);
                                ui->jobView->setCellWidget(r, 0, p);
                                ++r;
                        } else {
                                delete p;
                        }
                        f.unlock();
                        f.close();
                }
        }
}
