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

#include "../../actuarius/actuarius.hh"
#include "jobfile.hh"
#include <sstream>
#undef main

struct RenderJob {

        std::string title;
        redshift::scenefile::Scene scene;

        // Serialization.
        template<typename Arch>
        void serialize (Arch &arch) {
                using actuarius::pack;
                arch & pack("scene", scene);
                arch & pack("title", title);
        }
};

#include <QDir>
#include "renderjobwidget.hh"
#include "ui_renderjobwidget.h"

RenderJobWidget::RenderJobWidget(const QString &jobfile, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderJobWidget),
    jobFilename(jobfile),
    statusDirectory(jobfile + ".status")
{
        ui->setupUi(this);

        std::ifstream ifs(jobfile.toAscii());
        if (!ifs) {
                qWarning("ifs not opened");
        }
        RenderJob job;
        actuarius::IArchive (ifs) & actuarius::pack("job", job);

        updateStatus();

        ui->filename->setText(QString::fromStdString(job.title));
}

RenderJobWidget::~RenderJobWidget()
{
        delete ui;
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

void RenderJobWidget::updateStatus() {
        ui->filename->setText(jobFilename);
        if (!QDir().exists(statusDirectory)) {
                ui->progressBar->setValue(0);
        } else {
                ui->progressBar->setValue(50);
        }
}
