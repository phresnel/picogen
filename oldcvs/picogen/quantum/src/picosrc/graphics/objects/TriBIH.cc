/***************************************************************************
 *            object_bih.cc
 *
 *  Fri Sep 28 10:56:49 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


/// \todo: Intersect() fails if bih has not been invalidated
/// \todo: Intersect() fails if no brdf has been set

#include "picogen.h"

// quick and dirty aliases due to namespace re-ordering
typedef picogen::misc::prim::real real;
typedef picogen::misc::geometrics::Vector3d Vector3d;
typedef picogen::misc::geometrics::Ray Ray;
typedef picogen::misc::geometrics::BoundingBox BoundingBox;
typedef picogen::graphics::material::abstract::IBRDF IBRDF;
typedef picogen::graphics::structs::intersection_t intersection_t;

static const real tri_eps = 0.00000001;
static int
raytri_intersect (
    const Ray &ray,
    const Vector3d &a, const Vector3d &b, const Vector3d &c,
    real &t, real &u, real &v,
    Vector3d &normal) {
    Vector3d vect0, vect1, nvect;
    real det, inv_det;


    //SUB(vect0, b,a)
    vect0 = b - a;

    //SUB(vect1, c,a)
    vect1 = c - a;

    //CROSS(normal, vect0, vect1);
    normal = vect0.cross (vect1);

    /* orientation of the ray with respect to the triangle's normal,
       also used to calculate output parameters*/
    //det = - DOT(dir,normal);
    det = -ray.w() * normal;//-( p_ray->direction.x*normal->x + p_ray->direction.y*normal->y + p_ray->direction.z*normal->z );

    //---------

    /* if determinant is near zero, ray is parallel to the plane of triangle */
    if (det > -tri_eps && det < tri_eps) return 0;

    /* calculate vector from ray origin to a */
    //SUB(vect0,a,orig);
    vect0 = a - ray.x();

    /* normal vector used to calculate u and v parameters */
    //CROSS(nvect,dir,vect0);
    nvect = ray.w().cross (vect0);

    inv_det = 1.0 / det;
    /* calculate vector from ray origin to b*/
    //SUB(vect1,b,orig);
    vect1 = b - ray.x();

    /* calculate v parameter and test bounds */
    //*v = - DOT(vect1,nvect) * inv_det;
    v = - (vect1*nvect*inv_det);

    if (v < 0.0 || v > 1.0) return 0;

    /* calculate vector from ray origin to c*/
    //SUB(vect1,c,orig);
    vect1 = c - ray.x();

    /* calculate v parameter and test bounds */
    //*u = DOT(vect1,nvect) * inv_det;
    u = vect1*nvect*inv_det;

    if (u < 0.0 || u + v > 1.0) return 0;

    /* calculate t, ray intersects triangle */
    //*t = - DOT(vect0,normal) * inv_det;
    t = - (vect0* normal * inv_det);

    //---------

    if (t < 0)
        return 0;
    normal = normal.normal();
    if (ray.w() * normal > 0.0)
        return -1;
    return 1;
}

namespace picogen {
    namespace graphics {
        namespace objects {



            TriBIH::TriBIH() : AABB(), BIHBuild(), BIH (NULL), BRDF (NULL), shader (NULL) {
            }



            TriBIH::~TriBIH() {
                BRDF = NULL;
                shader = NULL;
                flush();
            }



            void TriBIH::setBRDF (const IBRDF *BRDF) {
                this->BRDF = BRDF;
            }



            void TriBIH::setShader (const material::abstract::IShader *shader) {
                this->shader = shader;
            }



            bool TriBIH::intersect (
                param_out (intersection_t,intersection),
                const bih_node *node,
                real t_min,
                real t_max,
                param_in (Ray,ray)
            ) const {
                using namespace misc::constants;

                /*if( t_min > t_max ){
                 return false;
                }*/

                if (node->is_leaf) {
#if TriBIH_VisualizeLeafs
                    if (rand() %4 == 0) {
                        unsigned int r = rand();
                        srand ( (unsigned int) node);
                        if (node->aabb.intersect (t_min, t_max, ray)) {
                            intersection.side = outside;
                            intersection.t = t_min;
                            intersection.normal = -ray.w();
                            intersection.color = image::color::Color (
                                                     (double) rand() / (double) RAND_MAX,
                                                     (double) rand() / (double) RAND_MAX,
                                                     (double) rand() / (double) RAND_MAX
                                                 );
                            intersection.brdf = BRDF;
                            intersection.L_e = 1.0;
                            srand (r);
                            return true;
                        }
                        srand (r);
                        return false;
                    }
#endif
                    unsigned int T;
                    bool any = false;
                    for (T=node->triangles_begin; T<node->triangles_end; T++) {
                        real t,u,v;
                        Vector3d N;
                        const int tmp = raytri_intersect (ray, Triangles[T].A, Triangles[T].B, Triangles[T].C, t,u,v,N);
                        if (0 != tmp && t>=t_min-epsilon && t<=t_max+epsilon && t<intersection.t) {
                            intersection.side = tmp<0?inside:outside;
                            if (intersection.side == inside) {
                                intersection.normal = -N;
                            } else {
                                intersection.normal = N;
                            }
                            intersection.t = t;
                            intersection.color = image::color::Color (1,1,1);
                            intersection.brdf = BRDF;
                            intersection.L_e = 0.0;
                            any = true;
                        }
                    }
                    return any;
                } else {

                    //++
                    // i had  a  (near)  branchless traversal (except for the calls),
                    // but it was, say, sh**. so for debugging, i tried out this one
                    // by Michael77 on ompf:
                    //       [http://ompf.org/forum/viewtopic.php?p=2613]
                    //--
                    const real x  = ray.x() [node->axis];
                    const real w  = ray.w() [node->axis];

                    const real         iw         = 1.0 / w;
                    const bool         w_positive = (iw >= 0.0);
                    const unsigned int near       = w_positive ? 0 : 1;
                    const unsigned int far        = w_positive ? 1 : 0;

                    const real         dist_near  = (node->clip[near] - x) * iw;
                    const real         dist_far   = (node->clip[far]  - x) * iw;

                    const bih_node    *node_near  = &BIH[node->children+near];
                    const bih_node    *node_far   = &BIH[node->children+far];

                    /*const real far_t_min   = t_min>dist_far ? t_min : dist_far;
                    const real near_t_max  = t_max<dist_near ? t_max : dist_near;

                    const bool i1 = t_min<=near_t_max && Intersect( intersection, node_near, t_min, near_t_max, ray );
                    const bool i2 = far_t_min<=t_max && Intersect( intersection, node_far,  far_t_min, t_max, ray );*/

                    //bool i1 = false, i2 = false;
                    if (t_min >= dist_near) {
                        return intersect (intersection, node_far, t_min>dist_far ? t_min : dist_far, t_max, ray);
                    } else if (t_max < dist_far) {
                        return intersect (intersection, node_near, t_min, t_max<dist_near ? t_max : dist_near, ray);
                    } else {
                        const bool i1 = intersect (intersection, node_near, t_min, (t_max<dist_near) ? (t_max) : (dist_near), ray);
                        const bool i2 = intersect (intersection, node_far, (t_min>dist_far) ? (t_min) : (dist_far), t_max, ray);
                        return i1 || i2;
                    }

                    /*
                    const real x  = ray.x()[node->axis];
                    const real w = ray.w()[node->axis];

                    const real         iw         = 1.0 / w;
                    const bool         w_positive = iw >= 0.0;
                    const unsigned int near       = w_positive ? 0 : 1;
                    const unsigned int far        = w_positive ? 1 : 0;
                    const bih_node    *node_near  = &BIH[node->children+near];
                    const bih_node    *node_far   = &BIH[node->children+far];
                    const real         dist_near  = ( node->clip[near] - x ) * iw;
                    const real         dist_far   = ( node->clip[far]  - x ) * iw;

                    const real a   = t_min;
                    const real b   = t_max<dist_near ? t_max : dist_near;
                    const bool i1  = (a<=b) ? Intersect( intersection, node_near, a, b, ray ) : false;

                    const real c   = t_min>dist_far ? t_min : dist_far;
                    const real d   = ( !i1 || (t_max<intersection.t) ) ? t_max : intersection.t;
                    const bool i2  = (c<=d) ? Intersect( intersection, node_far, c, d, ray ) : false;
                    return i1 || i2;
                    */
                    /*
                    const bool i1 = Intersect( intersection, node_near, t_min, t_max, ray );
                    const bool i2 = Intersect( intersection, node_far,  t_min, t_max, ray );
                    */
                }
                return false;
            }


            bool TriBIH::intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
                using namespace misc::constants;
                real t_min, t_max;
                const bool bbhit = AABB.intersect (t_min, t_max, ray);
                if (!bbhit)
                    return false;
                t_min = t_min<0.0?0.0:t_min;
                if (t_min>t_max)
                    return false;
                intersection.t = real_max;
                if (NULL != shader)
                    shader->shade (intersection.color,intersection.normal,ray.x() +ray.w() *intersection.t);// = image::color::Color(1.0,0.9,0.8);//image::color::Color(1.0,0.0,0.0)*f + image::color::Color(0.0,0.0,1.0)*(1.0-f);
                else
                    intersection.color = image::color::Color (1.0,1.0,1.0);
                return intersect (intersection, BIH, t_min, t_max, ray);
            }

            void TriBIH::insert (param_in (Vector3d,A), param_in (Vector3d,B), param_in (Vector3d,C)) {
                AABB.update (A);
                AABB.update (B);
                AABB.update (C);
                BIHBuild.insert (t_triangle (A,B,C));
            }

            void TriBIH::flush() {
                //TempList.clear();
                if (NULL != BIH)
                    delete [] BIH;
                BIH = NULL;
                if (NULL != Triangles)
                    delete [] Triangles;
                Triangles = NULL;
                BIHBuild.free();
            }

            void TriBIH::invalidate() {
                BIHBuild.build (&BIH, &Triangles);
                BIHBuild.free();
            }



            void TriBIH::bih_build_node::insert (param_in (t_triangle,tri)) {
                aabb.update (tri.A);
                aabb.update (tri.B);
                aabb.update (tri.C);
                triangles.insert (triangles.end(), tri);
            }

            void TriBIH::bih_build_node::free() {
                aabb.reset();
                triangles.clear();
                if (NULL != p_left)
                    delete p_left;
                p_left = NULL;
                if (NULL != p_right)
                    delete p_right;
                p_right = NULL;
            }



            bool TriBIH::bih_build_node::rec_build (const BoundingBox &build_aabb, const unsigned int maxRec=32) {

#if TriBIH_DebugBIHBuild
                if (maxRec == 0)
                    fprintf (stderr, "#forced leaf\n");
                vector3d size = build_aabb.max() - build_aabb.min();
                fprintf (stderr, /*"{{%.2f,%.2f,%.2f},{%.2f,%.2f,%.2f}}:"*/
                         "{%.2f,%.2f,%.2f}\n",
                         /*build_aabb.min()[0], build_aabb.min()[1], build_aabb.min()[2],
                         build_aabb.max()[0], build_aabb.max()[1], build_aabb.max()[2],*/
                         size[0], size[1], size[2]
                        );
#endif

                if (maxRec>0 && triangles.size() > 4) {   // then split

                    //> alloc children + evacuate if anything goes wrong
                    p_left = new bih_build_node;
                    if (p_left == NULL)
                        return false;
                    p_right = new bih_build_node;
                    if (p_right == NULL) {
                        delete p_left;
                        p_left = NULL;
                        return false;
                    }

                    const Vector3d size = build_aabb.getMax() - build_aabb.getMin();
                    unsigned int axes[3];
                    /*axes[0] = build_aabb.get_axis_of_max_extent();
                    axes[1] = (axes[0]+1)%3;
                    axes[2] = (axes[0]+2)%3;*/
                    // 'sort' by axis-size
                    //fprintf( stderr, ">>(%f,%f,%f)\n", (float)size[axes[0]], (float)size[axes[1]], (float)size[axes[2]] );
                    if (size[0] >= size[1] && size[0] > size[2])      axes[0] = 0;
                    else if (size[1] >= size[0] && size[1] > size[2]) axes[0] = 1;
                    else                                               axes[0] = 2;

                    if (size[2] <= size[0] && size[2] < size[1] && axes[0] != 0)      axes[2] = 2;
                    else if (size[1] <= size[0] && size[1] < size[2] && axes[0] != 1) axes[2] = 1;
                    else                                                               axes[2] = 0;

                    if (axes[0] == 1 && axes[2] == 2)      axes[1] = 0;
                    else if (axes[0] == 2 && axes[2] == 1) axes[1] = 0;
                    else if (axes[0] == 0 && axes[2] == 2) axes[1] = 1;
                    else if (axes[0] == 2 && axes[2] == 0) axes[1] = 1;
                    else if (axes[0] == 0 && axes[2] == 1) axes[1] = 2;
                    else                                    axes[1] = 2;

                    //> get median + splitting center
                    unsigned int axis_ofs = 0;    //> used for retry, that is, if the split plane is crap
                    unsigned int count_L, count_R;//> ditto
                    bool found;
                    do {
                        axis  = axes[axis_ofs];//(aabb.get_axis_of_max_extent()+axis_ofs)%3;
                        split = 0.5*build_aabb.getMin() [axis] + 0.5*build_aabb.getMax() [axis];

                        //> insert triangles into children
                        p_left->free();
                        p_right->free();
                        t_trianglelist::const_iterator it;
                        for (it=triangles.begin(),count_L=count_R=0; it!=triangles.end(); it++) {   // for each triangle
                            const real center = (1.0/3.0) * (it->A[axis] + it->B[axis] + it->C[axis]);
                            if (center < split) {   // then insert into left child
                                p_left->insert (*it);
                                ++count_L;
                            } else if (center > split) {   // insert into right child
                                p_right->insert (*it);
                                ++count_R;
                            } else {
#if TriBIH_DebugBIHBuild
                                fprintf (stderr, "#fuzzy\n");
#endif
                                //> a little bit of fuzzyness
                                if ((rand()&1) == 1) {
                                    ++count_L;
                                    p_left->insert (*it);
                                } else {
                                    ++count_R;
                                    p_right->insert (*it);
                                }
                            }
                        }
                        found = true;//count_L!=0 && count_R!=0;
                        //> increment axis for next try
                        ++axis_ofs;
                    } while (!found && axis_ofs<3);

                    if (!found) {   //> no good split found, so this becomes leaf node
                        delete p_left;
                        delete p_right;
                        p_left = p_right = NULL;

#if TriBIH_DebugBIHBuild
                        fprintf (stderr, "#found no good split...\n");
#endif
                    } else {

                        triangles.clear(); // free memory asap
                        //> now build children
                        // BoundingBox aabb_left = build_aabb, aabb_right = build_aabb;
                        // aabb_left.max() [axis] = split;
                        // aabb_right.min() [axis] = split;

                        /// \todo validate below 5 lines
                        BoundingBox aabb_left=build_aabb, aabb_right=build_aabb;
                        Vector3d vl = build_aabb.getMax();
                        Vector3d vr = build_aabb.getMin();
                        vl[axis] = split;
                        vr[axis] = split;

                        aabb_left.setMax( vl );
                        aabb_right.setMin( vr );

                        if (!p_left->rec_build (aabb_left, maxRec-1))
                            return false;
                        if (!p_right->rec_build (aabb_right, maxRec-1))
                            return false;
                    }
                    return true;
                } else { // reached a leaf
                    //> really nothing to do here since we already have our triangle-list
                    /*fprintf( stderr,
                     "\n#actual{ {%.3f,%.3f,%.3f}, {%.3f,%.3f,%.3f} }\n"
                       " split { {%.3f,%.3f,%.3f}, {%.3f,%.3f,%.3f} }\n",
                     aabb.min()[0], aabb.min()[1], aabb.min()[2],
                     aabb.max()[0], aabb.max()[1], aabb.max()[2],
                     build_aabb.min()[0], build_aabb.min()[1], build_aabb.min()[2],
                     build_aabb.max()[0], build_aabb.max()[1], build_aabb.max()[2]
                    );*/
                    return true;
                }
            }


            unsigned int TriBIH::bih_build_node::num_nodes() const {
                /// \todo: debug this piece!
                if (NULL != p_left && NULL != p_right) {
                    return 1 + p_left->num_nodes() + p_right->num_nodes();
                }
                return 1;
            }


            unsigned int TriBIH::bih_build_node::num_triangles() const {
                if (NULL != p_left && NULL != p_right) {
                    //> assume that at this point, we don't have any triangles left
                    return p_left->num_triangles() + p_right->num_triangles();
                }
                return triangles.size();
            }


            void TriBIH::bih_build_node::rec_convert (
                //>
                bih_node *bih_array, t_triangle *tri_array,

                //>
                unsigned int index_curr,

                //> alloc-ptr's
                unsigned int &bih_free, unsigned int &tri_free,

                //> stats
                unsigned int depth,
                unsigned int &max_tris_in_node, unsigned int &max_depth
            ) const {
                //> analyzing stats
                if (depth > max_depth)
                    max_depth = depth;

                bih_node* curr = &bih_array[index_curr];

#if TriBIH_VisualizeLeafs
                curr->aabb = aabb;
#endif

#if TriBIH_DebugBIHRecConvert
                {
                    unsigned int i;
                    for (i=0; i<depth; i++)
                        fprintf (stderr, ".");
                    if ( (p_left==NULL || p_right==NULL)  && p_left!=p_right)
                        fprintf (stderr, "{misbuild}");
                }
#endif

                //> actual conversion
                if (p_left != NULL && p_right != NULL) {   //> is inner node

                    curr->clip[0] = p_left->aabb.getMax() [axis];
                    curr->clip[1] = p_right->aabb.getMin() [axis];
                    curr->is_leaf = false;
                    curr->axis  = axis;
                    curr->children = bih_free;

#if TriBIH_DebugBIHRecConvert
                    {
                        fprintf (stderr, "[%d]axis=%d,clip={%.2f,%.2f}-[%d,%d]\n",
                        index_curr,
                        curr->axis,
                        (float) curr->clip[0], (float) curr->clip[1],
                        curr->children, curr->children+1
                                );
                    }
#endif

                    //> alloc children
                    bih_free+=2;
                    p_left ->rec_convert (bih_array, tri_array, curr->children,   bih_free, tri_free, depth+1, max_tris_in_node, max_depth);
                    p_right->rec_convert (bih_array, tri_array, curr->children+1, bih_free, tri_free, depth+1, max_tris_in_node, max_depth);
                } else { //> is leaf

                    //> analyzing stats
                    if (triangles.size() > max_tris_in_node)
                        max_tris_in_node = triangles.size();

                    //> actual conversion
                    curr->is_leaf          = true;
                    curr->triangles_begin  = tri_free;
                    curr->triangles_end    = tri_free+triangles.size();

                    //fprintf( stderr, "[%d..%d)", (int)curr->triangles_begin, (int)curr->triangles_end );

                    //> insert triangles
                    t_trianglelist::const_iterator it;
                    for (it=triangles.begin(); it!=triangles.end(); it++, tri_free++) {
                        tri_array[tri_free] = *it;
                    }

#if TriBIH_DebugBIHRecConvert
                    fprintf (stderr, "[%d]leaf, T[W=%d,%d..%d)\n", index_curr, curr->triangles_end-curr->triangles_begin, curr->triangles_begin, curr->triangles_end);
#endif
                }
            }



            bool TriBIH::bih_build_node::build (bih_node **bih, t_triangle **triangles) {
                using namespace misc::constants;

                fprintf (stderr, "building bih-structure %xP{\n", (unsigned int) this);

                if (NULL == bih) {
                    fprintf (stderr, "  parameter 'bih' is null\n}\n");
                    return false;
                }
                if (NULL == triangles) {
                    fprintf (stderr, "  parameter 'triangles' is null\n}\n");
                    return false;
                }


                fprintf (stderr, "  building recursively...");
                if (!rec_build (aabb)) {
                    fprintf (stderr, "  error on rec_build(), probably out of memory\n}\n");
                    return false;
                }
                fprintf (stderr, "done\n");

                fprintf (stderr, "  counting...");
                const unsigned int numNodes = num_nodes();
                fprintf (stderr, "done\n");
                fprintf (stderr, "  allocating linear array for bih-structure [%u nodes; %.3fMiB]...", numNodes, (float) (sizeof (bih_node) *numNodes) / (1024.0f*1024.0f));
                *bih       = new bih_node[ numNodes ];
                if (NULL == *bih) {
                    fprintf (stderr, "  error on 'new bih_node[...]', probably out of memory\n}\n");
                    return false;
                }
                fprintf (stderr, "done\n");

                const int numTris = num_triangles();
                fprintf (stderr, "  allocating linear triangle-array [%u tris; %.3fMiB]...", numTris, (float) (sizeof (t_triangle) *numTris) / (1024.0f*1024.0f));
                *triangles = new t_triangle[ numTris ];
                if (NULL == *triangles) {
                    delete [] *bih;
                    fprintf (stderr, "  error on 'new t_trianngle[...]', probably out of memory\n}\n");
                    return false;
                }
                fprintf (stderr, "done\n");

                fprintf (stderr, "  now converting to linear array...");
                unsigned int bih_free = 1, tri_free = 0;
                unsigned int max_tris_in_node=0, max_depth=0;
                rec_convert (*bih, *triangles, 0, bih_free, tri_free, 0, max_tris_in_node, max_depth);
                fprintf (stderr,
                         "done\n"
                         "   > free-bih-ptr points to %u\n"
                         "   > free-tri-ptr points to %u\n"
                         "   > max depth %u\n"
                         "   > max tri count %u\n"
                         "}\n",
                         bih_free,
                         tri_free,
                         max_depth,
                         max_tris_in_node
                        );
                return true;
            }


        }; // namespace objects
    }; // namespace graphics
}; // namespace picogen
