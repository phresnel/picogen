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

#include "job.hh"

#include <QDir>
#include <QtLockedFile>
#include <QTextStream>
#include "renderjobwidget.hh"
#include "ui_renderjobwidget.h"

RenderJobWidget::RenderJobWidget(QFile &jobfile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderJobWidget),
    jobFilename(jobfile.fileName()),
    statusDirectory(jobfile.fileName() + ".status")
{
        ui->setupUi(this);

        job = loadFromFile(jobfile);
        if (job->scene.renderSettingsCount() == 0
         || job->scene.cameraCount() == 0
        ) {
                isValid_ = false;
                return;
        }
        isValid_ = true;
        prepareJob(*job, statusDirectory);

        ui->filename->setText(jobFilename);

        updateProgress();
}

RenderJobWidget::~RenderJobWidget()
{
        delete job;
        delete ui;
}

bool RenderJobWidget::isValid() const {
        return isValid_;
}

void RenderJobWidget::changeEvent(QEvent *e)
{
        QWidget::changeEvent(e);
        switch (e->type()) {
                case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void RenderJobWidget::updateProgress() {
        const Status status = readStatus(statusDirectory);
        ui->progressBar->setValue(
                100*(status.currentScanline - job->getRenderSettings().min_y)
                / job->getRenderSettings().max_y);
}
