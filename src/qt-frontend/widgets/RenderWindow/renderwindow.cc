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
#include <QProcess>
#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTime>

#include "renderwindow.hh"
#include "ui_renderwindow.h"
#include "../../redshift/include/jobfile.hh"
#include "../../redshift/include/image-export.hh"
#include "../../redshift/include/interaction/progressreporter.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"
#include "../../redshift/include/rendertargets/rendertargetlock.hh"
#include "../../redshift/include/basictypes/film.hh"



QString getImageSavePath (QWidget *parent) {
        QFileDialog dialog(parent);
        dialog.setWindowTitle("Set a target filename");
        dialog.setFileMode(QFileDialog::AnyFile);

        QStringList nameFilters;
        nameFilters << "OpenEXR HDR (*.exr)"
                    << "Portable Network Graphics (*.png)"
                    << "Bitmap (*.bmp)"
                    << "JPEG (*.jpg *.jpeg)"
                    << "Portable Pixmap (*.ppm)"
                    << "Tagged Image File Format (*.tif *.tiff)"
                    << "X11 Bitmap (*.xbm)"
                    << "X11 Pixmap (*.xpm)"
                    ;
        dialog.setNameFilters(nameFilters);

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        again:

        if (dialog.exec()) {
                QString pathToTarget = dialog.selectedFiles()[0];
                const QFileInfo fi (pathToTarget);
                const QString   filename = fi.fileName();

                const bool hasValidExtension =
                        filename.endsWith(".exr", Qt::CaseInsensitive)
                     || filename.endsWith(".bmp", Qt::CaseInsensitive)
                     || filename.endsWith(".jpg", Qt::CaseInsensitive)
                     || filename.endsWith(".jpeg", Qt::CaseInsensitive)
                     || filename.endsWith(".png", Qt::CaseInsensitive)
                     || filename.endsWith(".ppm", Qt::CaseInsensitive)
                     || filename.endsWith(".tif", Qt::CaseInsensitive)
                     || filename.endsWith(".tiff", Qt::CaseInsensitive)
                     || filename.endsWith(".xbm", Qt::CaseInsensitive)
                     || filename.endsWith(".xpm", Qt::CaseInsensitive);
                // The following should also allow for hidden names like ".foo"
                const bool hasExtension = filename.lastIndexOf('.') > 0;

                if (hasExtension && !hasValidExtension) {
                        QMessageBox::information(parent,
                           "Unsupported filename extension",
                           "Please choose a filename with one of the following extensions:\n"
                           " * .exr\n"
                           " * .bmp\n"
                           " * .jpg / .jpeg\n"
                           " * .png\n"
                           " * .ppm\n"
                           " * .tif / .tiff\n"
                           " * .xbm\n"
                           " * .xpm\n"
                        );
                        goto again;
                } else if (!hasExtension){
                        // No filename given, so try to guess from file-dialog.
                        if (dialog.selectedNameFilter().contains(".exr"))
                                pathToTarget += ".exr";
                        else if (dialog.selectedNameFilter().contains(".bmp"))
                                pathToTarget += ".bmp";
                        else if (dialog.selectedNameFilter().contains(".jpg"))
                                pathToTarget += ".jpg";
                        else if (dialog.selectedNameFilter().contains(".png"))
                                pathToTarget += ".png";
                        else if (dialog.selectedNameFilter().contains(".ppm"))
                                pathToTarget += ".ppm";
                        else if (dialog.selectedNameFilter().contains(".tif"))
                                pathToTarget += ".tif";
                        else if (dialog.selectedNameFilter().contains(".xbm"))
                                pathToTarget += ".xbm";
                        else if (dialog.selectedNameFilter().contains(".xpm"))
                                pathToTarget += ".xpm";
                }

                // Check if overwrites.
                if (QFile::exists(pathToTarget) &&
                    QMessageBox::question(parent, "Overwrite file?",
                                          QString()+
                                          "Do you really want to overwrite the file "
                                          + "\"" + pathToTarget + "\"?",
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No
                                          ) == QMessageBox::No
                ) {
                        goto again;
                }

        /*QString name = QFileDialog::getSaveFileName(this, "Select a file to save to");
        if (name != "") {*/

                // Check if we can write.
                QFile file (pathToTarget);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QMessageBox::warning(parent, "Failed to save",
                             "The file \"" + file.fileName() + "\" could not be"
                             +" opened for writing. Please select another file "
                             +" or one that does not exist yet.");
                        goto again;
                }

                return pathToTarget;
        }
        return "";
}





//=============================================================================
// RenderWindowImpl
//=============================================================================
RenderWindowImpl::RenderWindowImpl (
                redshift::shared_ptr<redshift::scenefile::Scene> scenefile,
                int renderSettings, int camera, double updateLatency
) : renderSettings(renderSettings), camera(camera)
  , scenefile(scenefile)
  , error_(false), errorMessage_("")
  , running(false)
  , wantsToQuit(false)
  , wantsToPause(false)
  , realTime()
  , updateLatency(updateLatency)
  , firstReport(true)
{
}



RenderWindowImpl::~RenderWindowImpl () {
}



void RenderWindowImpl::report (int completed, int total) {
        if (firstReport || realTime() >= updateLatency) {
                emit updateImage((double)completed / (double)total);
                realTime.restart();
                firstReport = false;
        }
}



void RenderWindowImpl::reportDone () {
        emit updateImage(2.f);
}



void RenderWindowImpl::doCopy() {
        /*using namespace redshift;
        {
                RenderTarget::WriteLockPtr writeLock = target->lock();
                for (unsigned int y=0; y<renderBuffer->height(); ++y) {
                        for (unsigned int x=0; x<renderBuffer->width(); ++x) {
                                writeLock->setPixel(x, y,
                                                    renderBuffer->average(x, y));
                        }
                }
        }
        target->flip();*/
}



redshift::shared_ptr<redshift::Film> RenderWindowImpl::film() const {
        return renderBuffer;
}



void RenderWindowImpl::run() {
        using namespace redshift;
        using namespace redshift::interaction;

        running = true;

        try {
                const int
                      width = scenefile->renderSettings(renderSettings).width,
                      height = scenefile->renderSettings(renderSettings).height;

                renderBuffer = shared_ptr<Film>(
                                new Film (width, height));
                /*target = shared_ptr<QImageRenderTarget>(new QImageRenderTarget (
                                        width, height,
                                        fs.colorscale, fs.convertToSrgb));*/

                redshift::shared_ptr<redshift::Scene> scene =
                                        sceneDescriptionToScene(*scenefile,
                                                renderBuffer,
                                                renderSettings, camera);

                shared_ptr<interaction::ProgressReporter> rep =
                        shared_ptr<redshift::interaction::ProgressReporter>(shared_from_this());
                UserCommandProcessor::Ptr commandProcessor =
                                UserCommandProcessor::Ptr(shared_from_this());

                scene->render(
                     rep, commandProcessor,
                     scenefile->renderSettings(renderSettings).samplesPerPixel,
                     scenefile->renderSettings(renderSettings).min_y,
                     scenefile->renderSettings(renderSettings).max_y,
                     scenefile->renderSettings(renderSettings).userSeed
                );
                scenefile.reset();
        } catch (std::exception const &ex) {
                error_ = true;
                errorMessage_ = QString()
                        + "Critical exception occured:\n"
                        + ex.what();
                emit updateImage (2);
        } catch (...) {
                error_ = true;
                errorMessage_ = QString()
                        + "An unknown, critical exception occured";
                emit updateImage (2);
        }


        running = false;
}



void RenderWindowImpl::tick () {
        // We are running in an extra thread + RenderWindow will update our
        // state, so nothing to see here.
}



bool RenderWindowImpl::userWantsToQuit () const {
        return wantsToQuit;
}



bool RenderWindowImpl::userWantsToPause () const {
        return wantsToPause;
}



void RenderWindowImpl::setUserWantsToQuit (bool b) {
        wantsToQuit = b;
}



void RenderWindowImpl::setUserWantsToPause (bool b) {
        wantsToPause = b;
}



void RenderWindowImpl::saveQuit() {
        setUserWantsToQuit(true);
        while (running); // spin
}



//=============================================================================
// RenderWindow
//=============================================================================
// TODO: this function should not be part of render-window, as it is specific to
//       simplex-gui
void RenderWindow::RenderProcess (QString pathToSource,
                                  int renderSettings, int camera
) {
        QStringList args;
        args << "picogen-production-render";
        args << pathToSource;
        args << QString::number(renderSettings);
        args << QString::number(camera);

        const QString program = QApplication::applicationFilePath();
        if (!QProcess::startDetached(program, args)) {
                QMessageBox::information(0, "Nada.", "Sorry, but an error occured.");
        }
}


RenderWindow::RenderWindow(
        redshift::shared_ptr<redshift::scenefile::Scene> scenefile,
        int renderSettings, int camera,
        QWidget *parent,
        double updateLatency
) :
    QDialog(parent),
    ui(new Ui::RenderWindow),
    scenefile(scenefile),
    filmSettings(new redshift::scenefile::FilmSettings(
                    scenefile->filmSettings())),
    updateLatency(updateLatency),
    realTime(), compTime()
{
        ui->setupUi(this);

        // Part 1 of my resizing nightmare (there is also Part 2).
        //  The following lines shall ensure that upon initialization,
        //  the pixmap-label is properly sized. Within the resize method,
        //  we then set Ignore, so we can also shrink, not only grow.
        QPixmap map(scenefile->renderSettings(renderSettings).width,
                    scenefile->renderSettings(renderSettings).height);
        map.fill(QColor(0,0,0));

        ui->pix->setSizePolicy(QSizePolicy::Preferred,
                               QSizePolicy::Preferred);
        ui->pix->setPixmap(map);

        impl = redshift::shared_ptr<RenderWindowImpl>(
          new RenderWindowImpl(scenefile, renderSettings, camera, updateLatency));

        connect(
                impl.get(), SIGNAL(updateImage (double)),
                this, SLOT(updateImage(double))
        );

        scenefile.reset();
        realTime.restart();
        compTime.restart();
        impl->start();
}



RenderWindow::~RenderWindow() {
        impl->saveQuit();
        delete ui;
}



void RenderWindow::updateImage (double percentage) {
        redshift::ScopedLock lock(updateMutex);
        if (impl->error()) {
                setWindowTitle ("Error");

                QFont font;
                font.setStyleHint (QFont::TypeWriter, QFont::PreferAntialias);
                font.setFamily("Monospace");
                font.setFixedPitch(true);
                ui->pix->setFont(font);

                ui->pix->setText(impl->errorMessage());
                ui->saveImageButton->setEnabled(false);
        } else {
                // Compose time string.

                QString timeS = "";
                /*real*/ {
                        double minutes = realTime() / 60.0;
                        int hours = 0;
                        while (minutes >= 60) {
                                ++hours;
                                minutes -= 60;
                        }

                        timeS += "real: ";
                        if (hours>0)
                                timeS += QString::number(hours) + "h ";
                        if ((int)minutes>0)
                                timeS += QString::number((int)minutes) + "min ";
                        timeS += QString::number((int)(60*(minutes-(int)minutes))) + "sec";
                }
                /*comp time*/ {
                        timeS += ", cpu time: ";
                        double minutes = compTime() / 60.0;
                        int hours = 0;
                        while (minutes >= 60) {
                                ++hours;
                                minutes -= 60;
                        }
                        if (hours>0)
                                timeS += QString::number(hours) + "h ";
                        if ((int)minutes>0)
                                timeS += QString::number((int)minutes) + "min ";
                        timeS += QString::number((int)(60*(minutes-(int)minutes))) + "sec";
                }


                // Set window title.
                if (percentage>=1) {
                        setWindowTitle(timeS + " / " + "Done (unsaved).");
                } else {
                        setWindowTitle(timeS + " / " + QString::number(percentage*100, 'f', 3) + "%");
                }


                // Set image.
                //this->image = image; // TODO: profile that assignment
                using namespace redshift;
                {
                        const redshift::Film &film = *impl->film();
                        this->image = QImage(film.width(), film.height(),
                                             QImage::Format_RGB32);

                        const real_t s = 255 * filmSettings->colorscale;
                        if (filmSettings->convertToSrgb) {
                                for (unsigned int y=0; y<film.height(); ++y)
                                for (unsigned int x=0; x<film.width(); ++x) {
                                        const Color color =
                                                      film.average_or_zero(x,y);
                                        const redshift::color::SRGB rgb =
                                                        color.toRGB().toSRGB();
                                        const int rf = s * rgb.R,
                                                  gf = s * rgb.G,
                                                  bf = s * rgb.B,
                                                  rs = rf<0?0:rf>255?255:rf,
                                                  gs = gf<0?0:gf>255?255:gf,
                                                  bs = bf<0?0:bf>255?255:bf
                                        ;
                                        this->image.setPixel(
                                                x, y,
                                                qRgb(rs,gs,bs)
                                        );
                                }
                        } else {
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
                                        this->image.setPixel(
                                                x, y,
                                                qRgb(rs,gs,bs)
                                        );
                                }
                        }
                }
                if (!this->image.size().isNull()) {
                        ui->pix->setPixmap(QPixmap::fromImage(this->image).scaled(
                                ui->pix->size(),
                                Qt::KeepAspectRatio,
                                Qt::FastTransformation));
                        //ui->pix->setPixmap(QPixmap::fromImage(image));
                }
        }
}



void RenderWindow::on_saveImageButton_clicked() {
        const QString path = getImageSavePath(this);
        if ("" != path) {
                setWindowTitle("saving " + path + "... ");
                if (path.endsWith(".exr")) {
                        redshift::saveOpenEXR(*this->impl->film(),
                                              path.toAscii());
                } else {
                        this->image.save(path, 0, 100);
                }
                setWindowTitle("Saved as " + path);
        }
}



void RenderWindow::resizeEvent (QResizeEvent *) {
        if (ui->pix->pixmap()) {
                // Part 2 of my resizing nightmare. See also Part 1.
                ui->pix->setSizePolicy(QSizePolicy::Ignored,
                                       QSizePolicy::Ignored);

                QSize scaled = image.size();
                scaled.scale(ui->pix->size(), Qt::KeepAspectRatio);

                const double areaScaled = scaled.width() * scaled.height();
                const double areaOrig = image.width() * image.height();
                const double zoom = 100 * (areaScaled / areaOrig);
                const double diff = fabs (zoom - 100);

                if (diff < 10) {
                        if (!this->image.isNull())
                                ui->pix->setPixmap(QPixmap::fromImage(this->image));
                        setWindowTitle("Zoom: 100%");
                } else {
                        if (!this->image.isNull())
                                ui->pix->setPixmap(QPixmap::fromImage(this->image).scaled(
                                        scaled,
                                        Qt::KeepAspectRatio,
                                        Qt::FastTransformation));
                        if (zoom>=0 && zoom <= 1000000)
                                setWindowTitle("Zoom: " + QString::number(zoom) + "%");
                }
        }
}



void RenderWindow::on_pauseButton_clicked(bool checked) {
        impl->setUserWantsToPause (checked);
}
