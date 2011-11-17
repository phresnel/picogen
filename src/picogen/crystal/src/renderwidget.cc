#include "renderwidget.h"
#include "ui_renderwidget.h"

#include <QImage>

// Some features we want:
// * renderer with pause+continue capabilities
// * flexible, yet performant
// * clean, well-engineered code
namespace crystal {

        class RenderControl {
        public:
                // pause is expected to have low latency,
                // so canPause would return false e.g. in case an acceleration
                // structure must be built
                bool pause();
                bool canPause() const;

                void proceed();

                int activeThreadCount() const;
        };
}


#include "crystal_math.h"
#include "film.h"
#include "cameras/pinhole.h"
#include "geoblocks/ray_tri_intersect.h"


#include <boost/optional.hpp>
#include <memory>
#include <functional>

namespace crystal {
        using std::shared_ptr;
        using boost::optional;

        class SurfaceIntegrator;
        class VolumeIntegrator;

        struct Intersection
        {
                real distance;
                Normal normal;

                Intersection() = delete;

                Intersection (real distance, Normal const &normal)
                        : distance(distance), normal(normal)
                {}
        };
        typedef optional<Intersection> PIntersection;


        class Geometry
        {
        public:
                virtual ~Geometry() {}
                PIntersection intersect (Ray const &ray) const {
                        return this->intersect_(ray);
                }
        private:
                virtual PIntersection intersect_ (Ray const &ray) const = 0;
        };

        class LessIntelligentTerrain : public Geometry {
        public:
                LessIntelligentTerrain (std::function<real(real,real)> fun)
                        : fun_(fun)
                {
                }
        private:
                PIntersection intersect_ (Ray const &ray) const {
                        const Point a (-1, 1, 3),
                                    b (1,  1, 3),
                                    c (0, 0, 3);
                        real t_, u_, v_;
                        Normal normal_(0,1,0);
                        if (geoblocks::raytri_intersect (ray,
                                                         a, b, c,
                                                         t_, u_, v_,
                                                         normal_))
                        {
                                return Intersection (t_, normal_);
                        }
                        return PIntersection();
                }

                std::function<real(real,real)> fun_;
        };

        class Volume;




        class Scene {
        public:
                Scene() = delete;

                Scene (shared_ptr<const Geometry> geometry)
                        : geometry_(geometry)
                {}

                Geometry const& geometry() const {
                        return *geometry_;
                }
        private:
                shared_ptr<const Geometry>          geometry_;
                shared_ptr<const Volume>            volume_;
        };





        class Renderer {
        public:
                virtual ~Renderer() {}

                void render() const { return this->render_(); }
            /*virtual void Render(const Scene *scene) = 0;
            virtual Spectrum Li(const Scene *scene, const RayDifferential &ray,
                const Sample *sample, RNG &rng, MemoryArena &arena,
                Intersection *isect = NULL, Spectrum *T = NULL) const = 0;
            virtual Spectrum Transmittance(const Scene *scene,
                const RayDifferential &ray, const Sample *sample,
                RNG &rng, MemoryArena &arena) const = 0;
                */

        private:
                virtual void render_() const = 0;

        };

        class FlatRenderer : public Renderer {
        public:
                FlatRenderer (shared_ptr<Film> film,
                              shared_ptr<const Scene> scene,
                              shared_ptr<const Camera> camera,
                              shared_ptr<const SurfaceIntegrator> surfaceIntegrator,
                              shared_ptr<const VolumeIntegrator>  volumeIntegrator)
                        : film_(film)
                        , scene_(scene)
                        , camera_(camera)
                        , surfaceIntegrator_(surfaceIntegrator)
                        , volumeIntegrator_(volumeIntegrator)
                {
                }
        private:
                void render_ () const {
                        const int     height = film_->height();
                        const int     width  = film_->width();
                        const Camera& camera = *camera_;
                        const Geometry &geo  = scene_->geometry();
                        Film&         film   = *film_;

                        for (int y=0; y<height; ++y) {
                                for (int x=0; x<width; ++x) {
                                        const CameraSample sample(x, y,
                                                                  x/real(width),
                                                                  y/real(height));
                                        const Ray ray = camera(sample);

                                        const PIntersection pinter = geo.intersect(ray);

                                        if (pinter) {
                                                film.addSample (sample,
                                                                Radiance::FromRgb(1,0,0));
                                        } else {
                                                const Vector dir = ray.direction*1;
                                                film.addSample(sample, Radiance::FromRgb(
                                                                        0.5+dir.x,
                                                                        0.5+dir.y,
                                                                        0.5+dir.z
                                                                       ));
                                        }
                                }
                        }
                }

        private:
                shared_ptr<Film>                    film_;
                shared_ptr<const Scene>             scene_;
                shared_ptr<const Camera>            camera_;
                shared_ptr<const SurfaceIntegrator> surfaceIntegrator_;
                shared_ptr<const VolumeIntegrator>  volumeIntegrator_;
        };
}





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
        updateDisplay();
}

void RenderWidget::updateDisplay () {

        using namespace crystal;

        shared_ptr<Film>           film     (new Film(320, 240));
        shared_ptr<const Camera>   camera   (new cameras::Pinhole(1));

        shared_ptr<const Geometry> geometry (new LessIntelligentTerrain(
                                        [](real x, real z) { return 0; }));
        shared_ptr<const Scene>    scene    (new Scene(geometry));

        shared_ptr<const Renderer> renderer (new FlatRenderer(
                                                film,
                                                scene,
                                                camera,
                                                shared_ptr<const SurfaceIntegrator>(),
                                                shared_ptr<const VolumeIntegrator>()
                                            ));

        renderer->render();

        const unsigned int width = film->width(),
                           height = film->height();
        QImage image (width, height, QImage::Format_RGB32);

        for (unsigned int y=0; y<height; ++y) {
                for (unsigned int x=0; x<width; ++x) {
                        const rgb_t rgb = static_cast<rgb_t> ((*film)(x,y));
                        const int r_ = rgb.r * 255,
                                  g_ = rgb.g * 255,
                                  b_ = rgb.b * 255;
                        const int r = r_<0?0:r_>255?255:r_,
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
        on_pushButton_clicked();
}

#include "renderwidget.moc"
