#include "quadtree.h"
#include "aabb.h"
#include "color.h"
#include "materials/lambertmaterial.h"

#include <array>
#include <functional>
#include <limits>

#include <QDebug>

namespace picogen { namespace cracker {


namespace detail {

        static const real tri_eps = 0.00000001;
        static int
        raytri_intersect (
            const Ray &ray,
            const Vector &a, const Vector &b, const Vector &c,
            real &t, real &u, real &v,
            Normal &normal_
        ) {
            Vector vect0, vect1, nvect, normal;
            real det, inv_det;


            //SUB(vect0, b,a)
            vect0 = b - a;

            //SUB(vect1, c,a)
            vect1 = c - a;

            //CROSS(normal, vect0, vect1);
            normal = cross (vect0, vect1);

            /* orientation of the ray with respect to the triangle's normal,
               also used to calculate output parameters*/
            //det = - DOT(dir,normal);
            det = -mixed_dot(ray.direction(), normal);//-( p_ray->direction.x*normal->x + p_ray->direction.y*normal->y + p_ray->direction.z*normal->z );

            //---------

            /* if determinant is near zero, ray is parallel to the plane of triangle */
            if (det > -tri_eps && det < tri_eps) return 0;

            /* calculate vector from ray origin to a */
            //SUB(vect0,a,orig);
            vect0 = a - static_cast<Vector>(ray.origin());

            /* normal vector used to calculate u and v parameters */
            //CROSS(nvect,dir,vect0);
            nvect = mixed_cross<Vector> (ray.direction(), vect0);

            inv_det = 1.0 / det;
            /* calculate vector from ray origin to b*/
            //SUB(vect1,b,orig);
            vect1 = b - static_cast<Vector>(ray.origin());

            /* calculate v parameter and test bounds */
            //*v = - DOT(vect1,nvect) * inv_det;
            v = - dot(vect1, nvect) * inv_det;

            if (v < 0.0 || v > 1.0) return 0;

            /* calculate vector from ray origin to c*/
            //SUB(vect1,c,orig);
            vect1 = c - static_cast<Vector>(ray.origin());

            /* calculate v parameter and test bounds */
            //*u = DOT(vect1,nvect) * inv_det;
            u = dot (vect1, nvect) * inv_det;

            if (u < 0.0 || u + v > 1.0) return 0;

            /* calculate t, ray intersects triangle */
            //*t = - DOT(vect0,normal) * inv_det;
            t = - dot(vect0, normal) * inv_det;

            //---------
            // pretty crappy but sometimes useful wireframe mode
            //if ((u>0.1&&u<0.9) && (v>0.1&&v<0.9) && ((u+v)>0.1 && (u+v)<0.9)) return 0;

            if (t < 0)
                return 0;

            normal_ = normalize<Normal>(normal);
            if (mixed_dot (ray.direction(), normal) > 0.0)
                return -1;
            return 1;
        }

}


namespace detail {

        class Patch {
        public:
                Patch ()                         = delete;
                Patch (Patch const &)            = delete;
                Patch& operator= (Patch const &) = delete;


                Patch (real left, real right,
                       real front, real back,
                       unsigned int res_x,
                       unsigned int res_z,
                       std::function<real (real, real)> fun,
                       BoundingBox &exactBB)
                        : left_(left), right_(right), front_(front), back_(back)
                        , res_x_(res_x), res_z_(res_z)
                        , stride_(res_x_+1)
                        , h_(new real [(res_x+1)*(res_z+1)])
                {
                        assert (res_x >= 2);
                        assert (res_z >= 2);
                        assert (left < right);
                        assert (front < back);

                        real y_min = std::numeric_limits<real>::max(),
                             y_max = -y_min;
                        for (unsigned int uz=0; uz<=res_z; ++uz) {
                                for (unsigned int ux=0; ux<=res_x; ++ux) {
                                        const real u = ux/static_cast<real>(res_x);
                                        const real v = uz/static_cast<real>(res_z);

                                        const real x = (1-u)*left + u*right;
                                        const real z = (1-v)*front + v*back;
                                        const real y = fun(x,z);

                                        if (y < y_min) y_min = y;
                                        if (y > y_max) y_max = y;

                                        h(ux,uz) = y;
                                }
                        }

                        exactBB = BoundingBox (Point(left, y_min, front),
                                               Point(right, y_max, back));

                        material_.reset (new LambertMaterial(Color::FromRgb(
                                                                     0.3,0.8,0.4)));
                }

                ~Patch() {
                        delete [] h_;
                }

                Vector vertex(unsigned int ux, unsigned int uz) const {
                        const real u = ux/static_cast<real>(res_x_);
                        const real v = uz/static_cast<real>(res_z_);

                        const real x = (1-u)*left_ + u*right_;
                        const real z = (1-v)*front_ + v*back_;
                        const real y = h(ux,uz);

                        return Vector(x,y,z);
                }


                Intersection::Optional operator() (Ray const &ray) const {
                        const bool pro_x = ray.direction().x() >= 0;
                        const bool pro_z = ray.direction().z() >= 0;
                        if (pro_x & pro_z) {
                                return intersect<1,1> (ray, 0, res_x_,
                                                            0, res_z_);
                        }
                        if (pro_x & !pro_z) {
                                return intersect<1,-1> (ray, 0, res_x_,
                                                            res_z_-1, -1);
                        }
                        if (!pro_x & pro_z) {
                                return intersect<-1,1> (ray, res_x_-1, -1,
                                                             0,        res_z_);
                        }
                        return intersect<-1,-1> (ray, res_x_-1, -1,
                                                      res_z_-1, -1);
                }


        private:
                real left_, right_, front_, back_;
                unsigned int res_x_, res_z_, stride_;
                real *h_;

                std::shared_ptr<Material> material_;


                template <int step_x, int step_z>
                Intersection::Optional intersect (
                        Ray const &ray,
                        int left, int right,
                        int front, int back
                ) const
                {
                        for (int uz=front; uz!=back; uz+=step_z) {
                                for (int ux=left; ux!=right; ux+=step_x) {

                                        Vector a,b,c;
                                        real t, tu, tv;
                                        Normal tn(0,1,0);
                                        if (0 != raytri_intersect(
                                                ray,
                                                (a=vertex(ux, uz)),
                                                (b=vertex(ux, uz+1)),
                                                (c=vertex(ux+1, uz)),
                                                t, tu, tv, tn)
                                        || 0 != raytri_intersect(
                                                ray,
                                                (a=vertex(ux, uz+1)),
                                                (b=vertex(ux+1, uz+1)),
                                                (c=vertex(ux+1, uz)),
                                                t, tu, tv, tn)
                                        )
                                        {
                                                return Intersection (
                                                     t,
                                                     material_,
                                                     DifferentialGeometry(
                                                         tn, tn,
                                                         normalize<Normal>(b-a),
                                                         normalize<Normal>(c-a)));
                                        }
                                }
                        }

                        return Intersection::Optional();
                }

                real h (unsigned int x, unsigned z) const {
                        return h_[x + z*stride_];
                }
                real& h (unsigned int x, unsigned z) {
                        return h_[x + z*stride_];
                }
        };
}

namespace detail {

        inline std::array<BoundingBox,4> child_boxen (BoundingBox const &aabb) {
                const Point c = aabb.center();
                const real oldW = aabb.width(),
                           oldH = aabb.height(),
                           oldD = aabb.depth(),
                           newW = oldW / 2,
                           newH = oldH,
                           newD = oldD / 2
                          ;

                std::array<BoundingBox,4> ret;
                ret[0] = BoundingBox (c + Vector(-newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[1] = BoundingBox (c + Vector(+newW/2, 0, +newD/2),
                                      newW, newH, newD);
                ret[2] = BoundingBox (c + Vector(+newW/2, 0, -newD/2),
                                      newW, newH, newD);
                ret[3] = BoundingBox (c + Vector(-newW/2, 0, -newD/2),
                                      newW, newH, newD);
                return ret;
        }

        class Node {
        public:
                Node& operator= (Node const &) = delete;
                Node(Node const &)             = delete;

                // Creates a root node.
                Node (unsigned int depth,
                      std::function<real (real,real)> const & height)
                {
                        qDebug() << "creating Quadtree";
                        Point center (0, -40, 100);
                        create(depth, center, BoundingBox (center,
                                                           200,
                                                           2,
                                                           200),
                               height);
                        qDebug() << "Quadtree created.";
                }

                ~Node () {
                        delete [] children_;
                        delete patch_;
                }


                Intersection::Optional operator() (Ray const &ray) const {
                        Interval::Optional i = cracker::intersect(ray, aabb_);
                        if (!i) return Intersection::Optional();
                        return intersect (ray, i.interval().min(),
                                               i.interval().max());
                }

        private:

                Intersection::Optional intersect (
                        Ray const &ray,
                        real minT, real maxT
                ) const {

                        if (true) {
                                // We can assume minT and maxT to be a correct interval on the xz plane.
                                // But we got to check for vertical intersection now.
                                const real p_h = ray.origin().y();
                                const real min_h = p_h + minT * ray.direction().y();
                                const real max_h = p_h + maxT * ray.direction().y();


                                // BOTTLENECK when this->aabb.get...?
                                if (((min_h < aabb_.minY()) & (max_h < aabb_.minY()))
                                   |((min_h > aabb_.maxY()) & (max_h > aabb_.maxY())))
                                        return Intersection::Optional();
                        }

                        if (!children_) {
                                return (*patch_)(ray);
                        }


                        // Find out which ones to traverse.
                        const bool d_right = ray.direction().x() >= 0;
                        const bool d_up    = ray.direction().z() >= 0;
                        const real d_x = (aabb_.centerX() - ray.origin().x()) / ray.direction().x();
                        const real d_z = (aabb_.centerZ() - ray.origin().z()) / ray.direction().z();
                        const bool upper_three = d_x > d_z;

                        // +----+----+
                        // | 0  | 1  |
                        // -----+-----
                        // | 3  | 2  |
                        // +----+----+

                        struct C {
                                int child;
                                real t0, t1;
                        } t[3];
                        if (d_right & d_up) {
                                if (upper_three) {
                                        // 0, 2, 3
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 0; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 1; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 0, 1, 3
                                        t[0].child = 3; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 2; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 1; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else if (!d_right & d_up) {
                                if (upper_three) {
                                        // 1, 3, 2
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 1; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 0; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 1, 0, 2
                                        t[0].child = 2; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 3; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 0; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        // TODO: implement other cases
                        else if (d_right & !d_up) {
                                if (upper_three) {
                                        // 2, 0, 1
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 3; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 2; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 2, 3, 1
                                        t[0].child = 0; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 1; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 2; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else if (!d_right & !d_up) {
                                if (upper_three) {
                                        // 3, 1, 0
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_z;
                                        t[1].child = 2; t[1].t0 = d_z;  t[1].t1 = d_x;
                                        t[2].child = 3; t[2].t0 = d_x;  t[2].t1 = maxT;
                                } else {
                                        // 3, 2, 0
                                        t[0].child = 1; t[0].t0 = minT; t[0].t1 = d_x;
                                        t[1].child = 0; t[1].t0 = d_x;  t[1].t1 = d_z;
                                        t[2].child = 3; t[2].t0 = d_z;  t[2].t1 = maxT;
                                }
                        }
                        else {
                                return Intersection::Optional();
                        }

                        for (int i=0; i<3; ++i) {
                                const C& c = t[i];
                                if (c.t0 > c.t1) continue;
                                const Intersection::Optional I =
                                                children_[c.child].intersect(
                                                        ray, c.t0, c.t1);
                                if (I) return I.intersection();
                        }
                        return Intersection::Optional();
                }

                Node() {}

                void create(unsigned int depth,
                            Point const &center,
                            BoundingBox const &aabb,
                            std::function<real (real,real)> const & height
                ) {
                        const bool leaf = (depth==0);
                        //const real distance = length (aabb.center() - center);
                        //qDebug() << distance << " <- " <<
                        //            (13/(1+0.02*distance));

                        if (leaf) makeLeaf(center, aabb, height);
                        else makeInner(center, depth, aabb, height);
                }
                void makeLeaf (Point const &center,
                               BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        children_ = 0;
                        //aabb_ = aabb;
                        patch_ = new Patch (aabb.min().x(), aabb.max().x(),
                                            aabb.min().z(), aabb.max().z(),
                                            4,4,
                                            height,
                                            aabb_);
                        //patch_->exactBoundingBox ();
                        //refineBoundingBox (height);
                }
                void makeInner(Point const &center,
                               unsigned int depth, BoundingBox const &aabb,
                               std::function<real (real,real)> const & height)
                {
                        patch_ = 0;
                        const auto childBoxes = child_boxen(aabb);
                        children_ = new Node[4];
                        children_[0].create (depth-1, center, childBoxes[0], height);
                        children_[1].create (depth-1, center, childBoxes[1], height);
                        children_[2].create (depth-1, center, childBoxes[2], height);
                        children_[3].create (depth-1, center, childBoxes[3], height);
                        refineBoundingBox(aabb);
                }

                // Refinement for inner nodes.
                // Pre-condition: * child nodes are refined
                //                * has child nodes
                void refineBoundingBox (BoundingBox const &aabb) {
                        real min_h = children_[0].aabb_.min().y();
                        min_h = min(min_h, children_[1].aabb_.min().y());
                        min_h = min(min_h, children_[2].aabb_.min().y());
                        min_h = min(min_h, children_[3].aabb_.min().y());
                        real max_h = children_[0].aabb_.max().y();
                        max_h = max(max_h, children_[1].aabb_.max().y());
                        max_h = max(max_h, children_[2].aabb_.max().y());
                        max_h = max(max_h, children_[3].aabb_.max().y());

                        const Point &min = aabb.min(),
                                    &max = aabb.max();
                        aabb_ = BoundingBox (Point(min.x(), min_h, min.z()),
                                             Point(max.x(), max_h, max.z()));
                }

        private:
                Node *children_;
                Patch *patch_;
                BoundingBox aabb_;
        };
}

Quadtree::Quadtree ()
: root_(new detail::Node (4,
                          [](real x,real y) { return -30+15 * cos(y*0.1) * cos(x*0.1); }))
{
}

Intersection::Optional Quadtree::operator() (Ray const &ray) const {
        return (*root_)(ray);
}

} }
