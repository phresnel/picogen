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

#include "renderwindow.hh"
#include "ui_renderwindow.h"
#include "../../redshift/include/redshift.hh"
#include "../../redshift/include/jobfile.hh"
#include "../../redshift/include/interaction/rendertarget-copying-reporter.hh"
#include "../../redshift/include/rendertargets/colorrendertarget.hh"

#include "qimagerendertarget.hh"



//=============================================================================
// RenderWindowImpl
//=============================================================================
class RenderWindowImpl : public QThread {
public:
        RenderWindowImpl (RenderWindow *rw);
        virtual ~RenderWindowImpl ();

        virtual void run();
private:
        RenderWindow *renderWindow;
};


RenderWindowImpl::RenderWindowImpl (RenderWindow *rw) :
        renderWindow(rw)
{
        using namespace redshift;
        using namespace redshift::interaction;

        renderWindow->ui->pix->setPixmap(QPixmap(320,240));
}



RenderWindowImpl::~RenderWindowImpl () {
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


        shared_ptr<
                interaction::RenderTargetCopyingReporter
        > rep;

        redshift::shared_ptr<redshift::QImageRenderTarget> qrt;
        redshift::shared_ptr<redshift::ColorRenderTarget> crt;

        crt = shared_ptr<ColorRenderTarget>(new ColorRenderTarget (320, 240));
        qrt = shared_ptr<QImageRenderTarget>(new QImageRenderTarget (320, 240, fs.colorscale, fs.convertToSrgb));
        rep = shared_ptr<interaction::RenderTargetCopyingReporter>(
                new interaction::RenderTargetCopyingReporter(crt, qrt)
        );

        shared_ptr<Scene> scene = sceneDescriptionToScene(ss, crt);

        UserCommandProcessor::Ptr commandProcessor (new PassiveCommandProcessor());


        scene->render(
                        rep,
                        commandProcessor,
                        1
        );

        /*for (int y=0; y<240; ++y)
        {
                shared_ptr<RenderTargetLock> src_lock = crt->lock();
                for (int i=0; i<320; ++i) {
                        const Color c = Color::FromRGB(1.,0.5,0.25);
                        src_lock->setPixel (i, y, c);
                }
        }*/
        copy(crt, qrt);
        renderWindow->ui->pix->setPixmap(QPixmap::fromImage(*qrt));

}


//=============================================================================
// RenderWindow
//=============================================================================
RenderWindow::RenderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderWindow)
{
        ui->setupUi(this);
        impl = new RenderWindowImpl(this);
        impl->start();
}



RenderWindow::~RenderWindow() {
        delete impl;
        delete ui;
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
