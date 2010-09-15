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

#include "renderwidget.hh"
#include "ui_renderwidget.h"

#include <QImage>

#include "redshift/include/jobfile.hh"
#include "redshift/include/auxiliary/scopedlock.hh"
#include "redshift/include/basictypes/film.hh"
//#include "../../redshift/include/image-export.hh"

RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderWidget),
    renderThread(0)
{
        ui->setupUi(this);
}



RenderWidget::~RenderWidget() {
        if (renderThread) delete renderThread;
        delete ui;
}



void RenderWidget::setSceneAndRender (
        redshift::shared_ptr<const redshift::scenefile::Scene> scenefile,
        int renderSettingsIndex, int cameraIndex
) {
        //redshift::ScopedLock lock_(mutex);
        if (renderThread) delete renderThread;
        renderThread = new RenderWidgetThread(this, scenefile, renderSettingsIndex, cameraIndex);
        renderThread->start();
}




void RenderWidget::updateImage (const redshift::Film &film,
                                const redshift::scenefile::FilmSettings &fs
) {
        redshift::ScopedLock lock_(mutex);
        using redshift::Color;
        using redshift::scenefile::RenderSettings;

        QImage image(film.width(), film.height(), QImage::Format_RGB32);

        const double s = 255 * fs.colorscale;
        for (unsigned int y=0; y<film.height(); ++y)
        for (unsigned int x=0; x<film.width(); ++x) {
                const Color color =
                              film.average_or_zero(x,y);
                const redshift::color::RGB rgb =
                                color.toRGB();
                const int rf = s * rgb.R,
                          gf = s * rgb.G,
                          bf = s * rgb.B,
                          rs = rf<0?0:rf>255?255:rf,
                          gs = gf<0?0:gf>255?255:gf,
                          bs = bf<0?0:bf>255?255:bf
                ;
                image.setPixel(
                        x, y,
                        qRgb(rs,gs,bs)
                );
        }

        if (!image.size().isNull()) {
                ui->image->setPixmap(QPixmap::fromImage(image).scaled(
                        ui->image->size(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
                //ui->pix->setPixmap(QPixmap::fromImage(image));
        }
}






RenderWidgetThread::RenderWidgetThread(
        RenderWidget *renderWidget,
        redshift::shared_ptr<const redshift::scenefile::Scene> scenefile,
        int renderSettingsIndex,
        int cameraIndex
)
: renderWidget_(renderWidget)
, scenefile_(scenefile)
, renderBuffer_ (new redshift::Film (
        scenefile_->renderSettings(renderSettingsIndex).width,
        scenefile_->renderSettings(renderSettingsIndex).height))
, renderSettingsIndex_(renderSettingsIndex)
, cameraIndex_(cameraIndex)
, updateLatency_(1)
, reporter (new RenderWidgetProgressReporter(this))
, commandProcessor (new RenderWidgetUserCommandProcessor())
{
}



RenderWidgetThread::~RenderWidgetThread() {
        commandProcessor->setQuit(true);
        wait();
}



void RenderWidgetThread::run() {
        using namespace redshift;
        using namespace redshift::interaction;

        try {
                updateWatch_.restart();
                firstReport_ = true;
                renderImage (renderBuffer_,
                             *scenefile_,
                             renderSettingsIndex_,
                             cameraIndex_);

                renderWidget_->updateImage (*renderBuffer_,
                                            scenefile_->filmSettings());


        } catch (std::exception const &ex) {
                qWarning(ex.what());
                error_ = true;
                errorMessage_ = QString()
                        + "Critical exception occured:\n"
                        + ex.what();
                //emit updateImage (2);
        } catch (...) {
                error_ = true;
                errorMessage_ = QString()
                        + "An unknown, critical exception occured";
                //emit updateImage (2);
        }
}



void RenderWidgetThread::renderImage (
                  redshift::shared_ptr<redshift::Film> film,
                  const redshift::scenefile::Scene &scenef,
                  int renderSettingsIndex, int cameraIndex)
{
        using namespace redshift;
        using namespace redshift::interaction;

        // Prepare.
        const redshift::scenefile::RenderSettings rs =
                        scenef.renderSettings(renderSettingsIndex);
        const redshift::scenefile::FilmSettings fs =
                        scenef.filmSettings();

        redshift::shared_ptr<redshift::Scene> scene;

        scene = sceneDescriptionToScene(scenef,
                                        film,
                                        renderSettingsIndex,
                                        cameraIndex);
        // Render.
        scene->render (
                reporter,
                commandProcessor,
                16,//rs.samplesPerPixel,
                rs.min_y,
                rs.max_y,
                rs.userSeed
        );
}



void RenderWidgetThread::report (int completed, int total) {
        if (firstReport_ || updateWatch_() >= updateLatency_) {
                renderWidget_->updateImage(*renderBuffer_,
                                           scenefile_->filmSettings());
                updateWatch_.restart();
                firstReport_ = false;
        }
}
