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

#include <QImage>
#include <QThread>
#include <QMessageBox>
#include <QResizeEvent>

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
RenderWindowImpl::RenderWindowImpl () {

}



RenderWindowImpl::~RenderWindowImpl () {
}



void RenderWindowImpl::report (
        redshift::shared_ptr<redshift::RenderTargetLock const> rlock,
        int completed, int total
) {
        copy(renderBuffer, target);
        emit updateImage(*target, (double)completed / total);
}



void RenderWindowImpl::reportDone () {
        copy(renderBuffer, target);
        emit updateImage(*target, 2.f);
}



void RenderWindowImpl::run() {
        using namespace redshift;
        using namespace redshift::interaction;
        // --

        scenefile::Scene ss;

        scenefile::FilmSettings fs;
        fs.colorscale = 0.01;
        fs.convertToSrgb = true;
        ss.setFilmSettings(fs);

        scenefile::Background ho;
        ho.type = scenefile::Background::pss_sunsky;
        srand(clock());
        ho.sunDirection = scenefile::Normal(0.5, 0.2, 0.7);
        ho.turbidity = 2.0f;
        ss.addBackground(ho);

        scenefile::Camera ca;
        ca.title = "foo";
        scenefile::Camera::Transform pos;
        pos.type = scenefile::Camera::Transform::move;
        pos.x = 0; pos.y = 20; pos.z = 0;
        ca.transforms.push_back(pos);
        ss.addCamera(ca);


        scenefile::RenderSettings rs;
        rs.width = 320;
        rs.height = 240;
        rs.samplesPerPixel = 5;

        scenefile::SurfaceIntegrator si;
        si.type = scenefile::SurfaceIntegrator::redshift;
        scenefile::VolumeIntegrator vi;
        vi.type = scenefile::VolumeIntegrator::none;
        rs.surfaceIntegrator = si;
        rs.volumeIntegrator = vi;

        ss.addRenderSettings(rs);



        renderBuffer = shared_ptr<ColorRenderTarget>(new ColorRenderTarget (320, 240));
        target = shared_ptr<QImageRenderTarget>(
                        new QImageRenderTarget (320, 240, fs.colorscale, fs.convertToSrgb));

        scene = sceneDescriptionToScene(ss, renderBuffer);

        shared_ptr<interaction::ProgressReporter> rep =
                shared_ptr<redshift::interaction::ProgressReporter>(shared_from_this());
        UserCommandProcessor::Ptr commandProcessor (new PassiveCommandProcessor());
        scene->render(rep, commandProcessor, 1);
}








//=============================================================================
// RenderWindow
//=============================================================================
RenderWindow::RenderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderWindow)
{
        ui->setupUi(this);

        // The following 3 lines are to ensure that we are properly sized.
        // It seems impossible to do proper resizing within updateImage
        // (I've tried to setMinimumSize() on pixmap, layout, myself, and also
        //  the others like repaint(), update(), and all permutations)
        QPixmap map(320,240);
        map.fill(QColor(0,0,0));
        ui->pix->setPixmap(map);

        impl = redshift::shared_ptr<RenderWindowImpl>(new RenderWindowImpl());

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
        if (percentage>=1)
                setWindowTitle("Done.");
        else
                setWindowTitle(QString::number(percentage*100, 'f', 3) + "%");

        ui->pix->setPixmap(QPixmap::fromImage(image));
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
