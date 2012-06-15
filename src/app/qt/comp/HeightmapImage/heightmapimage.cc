#include "heightmapimage.h"
#include "ui_heightmapimage.h"

#include "quatsch.h"
#include "phase5/callable/to_callable.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <sstream>

HeightmapImage::HeightmapImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeightmapImage)
{
    ui->setupUi(this);
}

HeightmapImage::~HeightmapImage()
{
    delete ui;
}

#include <utility>

struct HeightmapSize { unsigned int width, height; };
struct HeightmapRegion { float left, right, front, back;
                         float width() { return right - left; }
                         float depth() { return back - front; }
                       };
struct HeightmapMaxima { float min, max; };

void map (std::function<float(float,float)> source,
          HeightmapSize size,
          HeightmapRegion region,
          std::function<void (int, int, float)> target)
{
        for (unsigned int y=0; y!=size.height; ++y) {
                for (unsigned int x=0; x!=size.width; ++x) {
                        const float fx = x / float(size.width),
                                    fy = y / float(size.height),
                                    u = fx * region.width() + region.left,
                                    v = fy * region.depth() + region.front;
                        target(x, y, source(u,v));
                }
        }
}

HeightmapMaxima minmax (std::function<float(float,float)> fun,
                        HeightmapSize size,
                        HeightmapRegion region)
{
        auto min_h = std::numeric_limits<float>::max(),
             max_h = -min_h;
        map (fun, size, region, [&](int, int, float h) {
                min_h = std::min(min_h, h);
                max_h = std::max(max_h, h);
        });
        return {min_h, max_h};
}


void HeightmapImage::paintEvent (QPaintEvent *)
{
        QPainter painter(this);

        const std::string code = "(* x z)";
        try {
                const quatsch::TemplatePtrList templates;
                const quatsch::ProgramContract ptype (quatsch::Typename::Float, {{"x", quatsch::Typename::Float},
                                                                                 {"z", quatsch::Typename::Float}});

                quatsch::ErrorState errors;
                quatsch::quatsch_function fun = quatsch::compile (
                                code, templates, ptype, errors,
                                quatsch::compiler::phase5::to_callable);

                if (errors) {
                        std::stringstream ss;
                        print_errors (errors, ss);
                        painter.drawText (0,0, QString::fromStdString(ss.str()));
                        // TODO: painter.drawText() doesn'T work yet
                } else {
                        using quatsch::DynamicVariant;

                        QImage image (256, 256, QImage::Format_RGB888);

                        const auto adapter = [&] (float u, float v) -> float {
                                         return fun({DynamicVariant::Floating(u),
                                                     DynamicVariant::Floating(v)})
                                                 .floating();
                                 };
                        const auto mm = minmax (adapter,
                                                {image.width(), image.height()},
                                                {0, 1, 0, 1});
                        std::cerr << mm.min << ":" << mm.max << "\n";

                        map (adapter,  {image.width(),image.height()}, {0,1,0,1},
                             [&](int x, int y, float h){
                                h = h*(mm.max-mm.min) - mm.min;
                                int d = 255 * h;
                                d = d < 0 ? 0 : d > 255 ? 255 : d;
                                image.setPixel (x, y, QColor::fromRgb(d,d,d).rgb());
                        });

                        painter.drawImage (0, 0, image);
                }

        } catch (std::exception &e) {
                QMessageBox::warning (this, "error", QString::fromStdString (e.what()));
        }
        /*QImage image(128, 128, Format::);
        image.setPixel (
        QPainter painter (this);
        painter.drawImage (0, 0, image);*/
}
