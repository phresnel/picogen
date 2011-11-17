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



#include <memory>
namespace crystal {
        using std::shared_ptr;

        class SurfaceIntegrator;
        class VolumeIntegrator;
        class Geometry;
        class Volume;

        namespace geobits {
                static const real tri_eps = 0.00000001;
                int
                raytri_intersect (
                    const Ray &ray,
                    const Point &a, const Point &b, const Point &c,
                    real &t, real &u, real &v,
                    Normal &normal_
                ) {
                    Vector vect0, vect1, nvect, normal;
                    real det, inv_det;

                    vect0 = b - a;
                    vect1 = c - a;

                    normal = cross (vect0, vect1);
                    det = -dot((Vector)ray.direction, normal);

                    //---------

                    /* if determinant is near zero, ray is parallel to the plane of triangle */
                    if (det > -tri_eps && det < tri_eps) return 0;

                    /* calculate vector from ray origin to a */
                    //SUB(vect0,a,orig);
                    vect0 = a - ray.origin;

                    /* normal vector used to calculate u and v parameters */
                    //CROSS(nvect,dir,vect0);
                    nvect = cross ((Vector)ray.direction, vect0);

                    inv_det = 1.0 / det;
                    /* calculate vector from ray origin to b*/
                    //SUB(vect1,b,orig);
                    vect1 = b - ray.origin;

                    /* calculate v parameter and test bounds */
                    //*v = - DOT(vect1,nvect) * inv_det;
                    v = -dot(vect1, nvect) * inv_det;

                    if (v < 0.0 || v > 1.0) return 0;

                    /* calculate vector from ray origin to c*/
                    //SUB(vect1,c,orig);
                    vect1 = c - ray.origin;

                    /* calculate v parameter and test bounds */
                    //*u = DOT(vect1,nvect) * inv_det;
                    u = dot(vect1,nvect) * inv_det;

                    if (u < 0.0 || u + v > 1.0) return 0;

                    /* calculate t, ray intersects triangle */
                    //*t = - DOT(vect0,normal) * inv_det;
                    t = -dot (vect0, normal) * inv_det;

                    //---------
                    // pretty crappy but sometimes useful wireframe mode
                    //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

                    if (t < 0)
                        return 0;
                    normal_ = Normal (normal.x, normal.y, normal.z);
                    if (dot ((Vector)ray.direction, normal) > 0.0)
                        return -1;
                    return 1;
                }

        }


        class Scene {
                shared_ptr<Geometry>          geometry_;
                shared_ptr<Volume>            volume_;
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
                        Film&         film   = *film_;

                        for (int y=0; y<height; ++y) {
                                for (int x=0; x<width; ++x) {
                                        const CameraSample sample(x, y,
                                                                  x/real(width),
                                                                  y/real(height));
                                        const Ray ray = camera(sample);

                                        const Vector dir = ray.direction*1;
                                        film.addSample(sample, Radiance::FromRgb(
                                                                0.5+dir.x,
                                                                0.5+dir.y,
                                                                0.5+dir.z
                                                               ));
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
        shared_ptr<const Renderer> renderer (new FlatRenderer(
                                                film,
                                                shared_ptr<const Scene>(),
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
