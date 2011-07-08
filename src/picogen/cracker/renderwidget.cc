#include "renderwidget.h"
#include "ui_renderwidget.h"

#include "render.h"
#include "scene.h"
#include "rendertarget.h"
#include "rendertargetrow.h"

#include "cameras/pinhole.h"
#include "surfaceintegrators/primarydistance.h"

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
        glimpse::StopWatch totaltime;
        using namespace picogen::cracker;
        using std::shared_ptr;

        shared_ptr<Scene> scene (new Scene);
        shared_ptr<RenderTarget> target(new RenderTarget (320, 240));

        glimpse::StopWatch rendertime;
        picogen::cracker::render (scene,
                                  PrimaryDistanceIntegrator(0,1000),
                                  PinholeCamera(0.001),
                                  target);

        const unsigned int width = target->width(),
                           height = target->height();
        QImage image (width, height, QImage::Format_RGB32);

        for (unsigned int y=0; y<height; ++y) {
                RenderTargetRow row = target->row(y);

                #pragma omp parallel for
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
        ui->perf->setText ("[wallclock] total: " + QString::number(totaltime())
                           + ", render: " + QString::number(rendertime()));
        ui->label->setPixmap(QPixmap::fromImage(image));
}

#include "renderwidget.moc"

