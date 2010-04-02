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

#include <stdexcept>
#include <QImage>
#include <QThread>
#include <QResizeEvent>
#include <QFont>

#include "renderwindow.hh"
#include "ui_renderwindow.h"
#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/jobfile.hh"
#include "../../redshift/include/interaction/progressreporter.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include "../../redshift/include/rendertargets/rendertargetlock.hh"
#include "qimagerendertarget.hh"








//=============================================================================
// RenderWindowImpl
//=============================================================================
RenderWindowImpl::RenderWindowImpl (
                redshift::shared_ptr<redshift::scenefile::Scene> scenefile
) : scenefile(scenefile), error_(false), errorMessage_("") {
}



RenderWindowImpl::~RenderWindowImpl () {
}



void RenderWindowImpl::report (
        redshift::shared_ptr<redshift::RenderTargetLock const> /*rlock*/,
        int completed, int total
) {
        if (++reportW>20) {
                copy(renderBuffer, target);
                emit updateImage(*target, (double)completed / total);
                reportW = 0;
        }
}



void RenderWindowImpl::reportDone () {
        copy(renderBuffer, target);
        emit updateImage(*target, 2.f);
}



void RenderWindowImpl::run() {
        using namespace redshift;
        using namespace redshift::interaction;
        // --

        using namespace redshift;

        try {
                const scenefile::FilmSettings &fs = scenefile->filmSettings();

                const int width = scenefile->renderSettings(0).width,
                          height =  scenefile->renderSettings(0).height;

                renderBuffer = shared_ptr<ColorRenderTarget>(
                                new ColorRenderTarget (width, height));
                target = shared_ptr<QImageRenderTarget>(new QImageRenderTarget (
                                        width, height,
                                        fs.colorscale, fs.convertToSrgb));

                redshift::shared_ptr<redshift::Scene> scene =
                                sceneDescriptionToScene(*scenefile, renderBuffer);

                shared_ptr<interaction::ProgressReporter> rep =
                        shared_ptr<redshift::interaction::ProgressReporter>(shared_from_this());
                UserCommandProcessor::Ptr commandProcessor (new PassiveCommandProcessor());

                scene->render(rep, commandProcessor, scenefile->renderSettings(0).samplesPerPixel);

        } catch (std::exception const &ex) {
                error_ = true;
                errorMessage_ = QString()
                        + "Critical exception occured:\n"
                        + ex.what();
                emit updateImage (QImage(), 2);
        } catch (...) {
                error_ = true;
                errorMessage_ = QString()
                        + "An unknown, critical exception occured";
                emit updateImage (QImage(), 2);
        }
}








//=============================================================================
// RenderWindow
//=============================================================================
RenderWindow::RenderWindow(
        redshift::shared_ptr<redshift::scenefile::Scene> scenefile,
        QWidget *parent
) :
    QDialog(parent),
    ui(new Ui::RenderWindow),
    scenefile(scenefile)
{
        ui->setupUi(this);

        // The following 3 lines are to ensure that we are properly sized.
        // It seems impossible to do proper resizing within updateImage
        // (I've tried to setMinimumSize() on pixmap, layout, myself, and also
        //  the others like repaint(), update(), and all permutations)
        QPixmap map(scenefile->renderSettings(0).width,
                    scenefile->renderSettings(0).height);
        map.fill(QColor(0,0,0));
        ui->pix->setPixmap(map);

        impl = redshift::shared_ptr<RenderWindowImpl>(
                        new RenderWindowImpl(scenefile));

        connect(
                impl.get(), SIGNAL(updateImage (QImage, double)),
                this, SLOT(updateImage (QImage, double))
        );

        impl->start();
}



RenderWindow::~RenderWindow() {
        delete ui;
}



void RenderWindow::updateImage (QImage image, double percentage) {
        if (impl->error()) {
                setWindowTitle ("Error");

                QFont font;
                font.setStyleHint (QFont::TypeWriter, QFont::PreferAntialias);
                font.setFamily("Monospace");
                font.setFixedPitch(true);
                ui->pix->setFont(font);

                ui->pix->setText(impl->errorMessage());
        } else {
                if (percentage>=1) {
                        setWindowTitle("Done.");
                } else {
                        setWindowTitle(QString::number(percentage*100, 'f', 3) + "%");
                }

                ui->pix->setPixmap(QPixmap::fromImage(image));
        }
}



void RenderWindow::changeEvent(QEvent *e) {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}
