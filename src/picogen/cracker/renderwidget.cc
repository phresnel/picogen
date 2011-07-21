#include "renderwidget.h"
#include "ui_renderwidget.h"

#include "render.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"

#include "cameras/pinhole.h"
#include "surfaceintegrators/cpucore.h"
#include "surfaceintegrators/whitted.h"
#include "surfaceintegrators/primarydistance.h"
#include "surfaceintegrators/surfacenormal.h"
#include "surfaceintegrators/combiner.h"
#include "surfaceintegrators/nameof.h"
#include "primitives/sphere.h"

#include "glimpse/stopwatch.hh"

#include <QImage>

RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderWidget)
{
    ui->setupUi(this);
}

RenderWidget::~RenderWidget()
{
    delete ui;
}

void RenderWidget::on_pushButton_clicked()
{
        //std::cout << "omp-max-threads: " << omp_get_max_threads() << std::endl;
        //std::cout << "omp-max-threads: " << omp_get_num_threads() << std::endl;
        glimpse::StopWatch totaltime;
        using namespace picogen::cracker;
        using std::shared_ptr;

        shared_ptr<Scene> scene (new Scene);
        shared_ptr<RenderTarget> target(new RenderTarget (320, 240));

        for (int i=0; i<128; ++i) {
                const real x = rand() / (real)RAND_MAX*100 - 50;
                const real y = rand() / (real)RAND_MAX*10;
                const real z = rand() / (real)RAND_MAX*100 + 2;
                scene->insertGenericPrimitive (Sphere(Point(x,y,z), 1.4));
        }
        glimpse::StopWatch rendertime;

        const auto integrator = WhittedIntegrator();
                                //combine (PrimaryDistanceIntegrator(0,100),
                                //         SurfaceNormalIntegrator(),
                                //         0.5);
        picogen::cracker::render (scene,
                                  integrator,
                                  PinholeCamera(1.0),
                                  target);

        const unsigned int width = target->width(),
                           height = target->height();
        QImage image (width, height, QImage::Format_RGB32);

        for (unsigned int y=0; y<height; ++y) {
                RenderTargetRow row = target->row(y);

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
        rendertime.stop();
        totaltime.stop();
        ui->perf->setText ("total: " + QString::number(totaltime())
                          + ", render: " + QString::number(rendertime()));
        ui->label->setPixmap(QPixmap::fromImage(image));

        const QString integName = QString::fromStdString(nameof(integrator));
        if (parentWidget())
                parentWidget()->setWindowTitle (integName);
        else
                setWindowTitle (integName);
}

#include "renderwidget.moc"

