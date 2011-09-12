#include "renderwidget.h"
#include "ui_renderwidget.h"

#include "render.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"

#include "cameras/pinhole.h"
#include "surfaceintegrators/cpucore.h"
#include "surfaceintegrators/path.h"
#include "surfaceintegrators/primarydistance.h"
#include "surfaceintegrators/surfacenormal.h"
#include "surfaceintegrators/combiner.h"
#include "surfaceintegrators/nameof.h"
#include "primitives/sphere.h"

#include "glimpse/stopwatch.hh"

#include <QImage>

using namespace picogen::cracker;

RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderWidget)
{
    ui->setupUi(this);
    on_resetsceneButton_clicked();
}

RenderWidget::~RenderWidget()
{
    delete ui;
}

void RenderWidget::on_pushButton_clicked()
{
        if (static_cast<unsigned>(ui->imageWidth->value())  != target_->width()
         || static_cast<unsigned>(ui->imageHeight->value()) != target_->height())
        {
                target_.reset (new RenderTarget(ui->imageWidth->value(),
                                                ui->imageHeight->value()));
        }

        //std::cout << "omp-max-threads: " << omp_get_max_threads() << std::endl;
        //std::cout << "omp-max-threads: " << omp_get_num_threads() << std::endl;
        glimpse::StopWatch totaltime;
        using namespace picogen::cracker;
        using std::shared_ptr;

        glimpse::StopWatch rendertime;

        const auto integrator = combine (PathIntegrator(),
                                         combine(SurfaceNormalIntegrator(),
                                                 PrimaryDistanceIntegrator(0,100),
                                                 1),
                                         0);
        const auto renderer = createRenderer (scene_,
                                              integrator,
                                              PinholeCamera(1.0));
        for (int i=0; i<ui->samplesPerRun->value(); ++i) {
                renderer.render (target_);
                if (i) updateDisplay (*target_);
        }
        rendertime.stop();
        totaltime.stop();
        ui->perf->setText ("total: " + QString::number(totaltime())
                          + ", render: " + QString::number(rendertime()));
        updateDisplay (*target_);

        const QString integName = QString::fromStdString(nameof(integrator));
        if (parentWidget())
                parentWidget()->setWindowTitle (integName);
        else
                setWindowTitle (integName);
}

void RenderWidget::updateDisplay (picogen::cracker::RenderTarget const &target) {
        using namespace picogen::cracker;

        const unsigned int width = target.width(),
                           height = target.height();
        QImage image (width, height, QImage::Format_RGB32);

        for (unsigned int y=0; y<height; ++y) {
                auto row = target.row(y);

                for (unsigned int x=0; x<width; ++x) {
                        Pixel const &p = row[x];
                        Color const &c = p.color();
                        const int r_ = c.r()*255,
                                  g_ = c.g()*255,
                                  b_ = c.b()*255,
                                  r = r_<0?0:r_>255?255:r_,
                                  g = g_<0?0:g_>255?255:g_,
                                  b = b_<0?0:b_>255?255:b_;
                        image.setPixel (x, y,
                                        QColor(r,g,b).rgb());
                }
        }

        ui->label->setScaledContents(true);
        ui->label->setPixmap(QPixmap::fromImage(image));
        repaint();
        QApplication::processEvents();
}

void RenderWidget::on_resetsceneButton_clicked() {
        target_.reset (new RenderTarget(ui->imageWidth->value(),
                                        ui->imageHeight->value()));

        scene_.reset (new Scene);
        for (int i=0; i<0*32; ++i) {
                const real x = rand() / (real)RAND_MAX*100 - 50;
                const real y = rand() / (real)RAND_MAX*10 - 5;
                const real z = rand() / (real)RAND_MAX*30 + 2;
                scene_->insertGenericPrimitive (Sphere(Point(x,y,z), 2.4));
        }

        const auto o = ui->samplesPerRun->value();
        ui->samplesPerRun->setValue(1);
        on_pushButton_clicked();
        ui->samplesPerRun->setValue(o);
}

#include "renderwidget.moc"
