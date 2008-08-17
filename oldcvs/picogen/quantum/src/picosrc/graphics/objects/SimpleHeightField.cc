/***************************************************************************
 *            SimpleHeightField.cc
 *
 *  Sun Oct 28 12:07:43 2007
 *  Copyright  2007  Sebastian Mach
 *  seb@greenhybrid.net
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


#include <picogen/picogen.h>

typedef picogen::misc::prim::real real;
typedef picogen::misc::geometrics::Vector3d Vector3d;
typedef picogen::misc::geometrics::Ray Ray;
typedef picogen::misc::geometrics::BoundingBox BoundingBox;
typedef picogen::graphics::material::abstract::IBRDF IBRDF;
typedef picogen::graphics::structs::intersection_t intersection_t;



/// \todo bring the following triangle intersector in some intersector-repository or something
static const real tri_eps = 0.00000001;
static int
raytri_intersect (
    const Ray &ray,
    const real a[], const real b[], const real c[],
    real &t, real &u, real &v,
    real normal[]) {
    real vect0[3], vect1[3], nvect[3];
    real det, inv_det;


    //SUB(vect0, b,a)
    vect0[0] = b[0] - a[0];
    vect0[1] = b[1] - a[1];
    vect0[2] = b[2] - a[2];

    //SUB(vect1, c,a)
    vect1[0] = c[0] - a[0];
    vect1[1] = c[1] - a[1];
    vect1[2] = c[2] - a[2];

    //CROSS(normal, vect0, vect1);
    normal[0] = vect0[1] * vect1[2] - vect0[2] * vect1[1];
    normal[1] = vect0[2] * vect1[0] - vect0[0] * vect1[2];
    normal[2] = vect0[0] * vect1[1] - vect0[1] * vect1[0];

    /* orientation of the ray with respect to the triangle's normal,
       also used to calculate output parameters*/
    //det = - DOT(dir,normal);
    det = - (ray.w() [0] * normal[0] + ray.w() [1] * normal[1] + ray.w() [2] * normal[2]);

    //---------

    /* if determinant is near zero, ray is parallel to the plane of triangle */
    if (det > -tri_eps && det < tri_eps) return 0;

    /* calculate vector from ray origin to a */
    //SUB(vect0,a,orig);
    vect0[0] = a[0] - ray.x() [0];
    vect0[1] = a[1] - ray.x() [1];
    vect0[2] = a[2] - ray.x() [2];

    /* normal vector used to calculate u and v parameters */
    //CROSS(nvect,dir,vect0);
    //nvect = ray.w().cross( vect0 );
    nvect[0] = ray.w() [1] * vect0[2] - ray.w() [2] * vect0[1];
    nvect[1] = ray.w() [2] * vect0[0] - ray.w() [0] * vect0[2];
    nvect[2] = ray.w() [0] * vect0[1] - ray.w() [1] * vect0[0];

    inv_det = 1.0 / det;
    /* calculate vector from ray origin to b*/
    //SUB(vect1,b,orig);
    //vect1 = b - ray.x();
    vect1[0] = b[0] - ray.x() [0];
    vect1[1] = b[1] - ray.x() [1];
    vect1[2] = b[2] - ray.x() [2];

    /* calculate v parameter and test bounds */
    //*v = - DOT(vect1,nvect) * inv_det;
    //v = -(vect1*nvect*inv_det);
    v = -inv_det * (vect1[0] * nvect[0] + vect1[1] * nvect[1] + vect1[2] * nvect[2]);

    if (v < 0.0 || v > 1.0) return 0;

    /* calculate vector from ray origin to c*/
    //SUB(vect1,c,orig);
    vect1[0] = c[0] - ray.x() [0];
    vect1[1] = c[1] - ray.x() [1];
    vect1[2] = c[2] - ray.x() [2];

    /* calculate v parameter and test bounds */
    //*u = DOT(vect1,nvect) * inv_det;
    //u = vect1*nvect*inv_det;
    u = (vect1[0] * nvect[0] + vect1[1] * nvect[1] + vect1[2] * nvect[2]) * inv_det;

    if (u < 0.0 || u + v > 1.0) return 0;

    /* calculate t, ray intersects triangle */
    //*t = - DOT(vect0,normal) * inv_det;
    //t = -( vect0* normal * inv_det );
    t = -inv_det * (vect0[0] * normal[0] + vect0[1] * normal[1] + vect0[2] * normal[2]);

    //---------

    if (t < 0)
        return 0;
    const real tmp = 1.0 / sqrt (normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    normal[0] *= tmp;
    normal[1] *= tmp;
    normal[2] *= tmp;
    if (ray.w() [0]*normal[0] + ray.w() [1]*normal[1] + ray.w() [2]*normal[2] > 0.0)
        return -1;
    return 1;
}

namespace picogen {
    namespace graphics {
        namespace objects {



            SimpleHeightField::SimpleHeightField() :
                    m_scaleBox()
                    , m_vScale (0.0)
                    , m_pBRDF (0)
                    , m_pShader (0)
                    , m_pField (0)
                    , m_size (0) {
            }



            SimpleHeightField::~SimpleHeightField() {
                /*
                fprintf( stderr, "destructing SimpleHeightField %xP{\n", (unsigned int)this );
                fprintf( stderr, "  total isec-calls.....: %u\n", (unsigned int)numIntersectQueries );
                fprintf( stderr, "  marched on heightmap.: %u\n", (unsigned int)numTraversals );
                fprintf( stderr, "}\n" );*/

                if (m_pField != 0)
                    delete [] m_pField;
                m_pField  = 0;
                m_pBRDF   = 0;
                m_size    = 0;
                m_vScale  = 0.0;
                m_scaleBox.reset();
            }



            void SimpleHeightField::setBox (param_in (Vector3d, min), param_in (Vector3d, max)) {
                m_scaleBox.setMin( min );
                m_scaleBox.setMax( max );
                m_vScale = max[2] - min[2];

                const Vector3d tmp = max - min;
                m_scaleSize[0] = tmp[0];
                m_scaleSize[1] = tmp[1];
                m_scaleSize[2] = tmp[2];
                m_scaleBoxMin[0] = min[0];
                m_scaleBoxMin[1] = min[1];
                m_scaleBoxMin[2] = min[2];
                updateOptimizerVars();
            }



            void SimpleHeightField::updateOptimizerVars() {
                const Vector3d tmp = m_scaleBox.getMax() - m_scaleBox.getMin();
                m_scaleSize[0] = tmp[0];
                m_scaleSize[1] = tmp[1];
                m_scaleSize[2] = tmp[2];
                m_vScale = m_scaleSize[1];
                m_invertScaleSize[0] = 1.0 / m_scaleSize[0];
                m_invertScaleSize[1] = 1.0 / m_scaleSize[1];
                m_invertScaleSize[2] = 1.0 / m_scaleSize[2];
                m_scaleBoxMin[0] = m_scaleBox.getMin() [0];
                m_scaleBoxMin[1] = m_scaleBox.getMin() [1];
                m_scaleBoxMin[2] = m_scaleBox.getMin() [2];

                m_invertHMapSize = 1.0 / (real) (m_size - 1);
                m_scaleSize_mul_invertHMapSize[0] = m_scaleSize[0] * m_invertHMapSize;
                m_scaleSize_mul_invertHMapSize[1] = m_scaleSize[1] * m_invertHMapSize;
                m_scaleSize_mul_invertHMapSize[2] = m_scaleSize[2] * m_invertHMapSize;
            }



            ::picogen::misc::geometrics::BoundingBox SimpleHeightField::getBoundingBox() const {
                return m_scaleBox;
            }



            SimpleHeightField::t_heightVal SimpleHeightField::getHeight (unsigned int u, unsigned int v) const {
                return m_pField [u + v*m_size];
            }



            unsigned int SimpleHeightField::getSize() const {
                return m_size;
            }



            void SimpleHeightField::setBRDF (const material::abstract::IBRDF*  brdf) {
                m_pBRDF = brdf;
            }



            void SimpleHeightField::setShader (const material::abstract::IShader* shader) {
                m_pShader = shader;
            }



            real SimpleHeightField::smoothedHeightFunc (const misc::functions::abstract::uv_to_scalar *heightFunc, real fu, real fv, real cellSizeU, real cellSizeV) {
                const real su = cellSizeU * 0.5;
                const real sv = cellSizeV * 0.5;
                return
                    0.25*
                    (heightFunc->f (fu,   fv)
                     + heightFunc->f (fu + su, fv)
                     + heightFunc->f (fu,   fv + sv)
                     + heightFunc->f (fu + su, fv + sv)
                    );
            }



            real SimpleHeightField::smoothedHeightFunc (const ::picogen::misc::functional::Function_R2_R1 *heightFunc, real fu, real fv, real cellSizeU, real cellSizeV) {
                const real su = cellSizeU * 0.5;
                const real sv = cellSizeV * 0.5;
                return
                    0.25*
                    ((*heightFunc) (fu,   fv)
                     + (*heightFunc) (fu + su, fv)
                     + (*heightFunc) (fu,   fv + sv)
                     + (*heightFunc) (fu + su, fv + sv)
                    );
            }



            // TODO: make template in terms of heightFunc of below function
            void SimpleHeightField::init (
                unsigned int size,
                const ::picogen::misc::functional::Function_R2_R1 *heightFunc,
                real boundsGuessAccuracy,
                bool smooth
            ) {
                fprintf (stderr, "building SimpleHeightField %xP{\n", (unsigned int) this);


                if (m_pField != 0) {
                    fprintf (stderr, "  deleting previously allocated heightfield...");
                    delete [] m_pField;
                    fprintf (stderr, "done\n");
                }
                fprintf (stderr, "  allocating %ux%u-heightmap, claiming %.2fMiB (%.3fMTris)...", size, size, float (size*size*sizeof (t_heightVal)) / (1024.0*1024.0), float (size*size*2) / (1e6f));
                m_pField = new t_heightVal[size*size];
                fprintf (stderr, "done\n");
                m_size = size;
                unsigned int u, v;

                const real cellSize = 1.0 / (real) size;

                // get min and max value of function
                int updateDisplay = 99999;
                fprintf (stderr, "  determining bounds of height function %xP\n", (unsigned int) heightFunc);
                real tmin = misc::constants::real_max;
                real tmax = -misc::constants::real_max;
                if (boundsGuessAccuracy>0.9f) {
                    fprintf (stderr, "   doing this at full accuracy (requested was %.1f%%)\n", boundsGuessAccuracy*100.0f);
                    for (v = 0; v < size; v++) {
                        const real fv = v / (real) size;
                        for (u = 0; u < size; u++) {
                            const real fu = u / (real) size;
                            const real t = smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize) : (*heightFunc) (fu, fv);
                            tmin = t < tmin ? t : tmin;
                            tmax = t > tmax ? t : tmax;
                        }
                        /*if( ++updateDisplay > 10000 )*/ {
                            updateDisplay = 0;
                            fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (u + v*size) / (float) (size*size), v, size);
                            fflush (stderr);
                        }
                        /*if( v%(size/10)==0 )
                         fprintf( stderr, "." );*/
                    }
                } else {
                    fprintf (stderr, "   using random sampling (requested %.1f%% of accuracy)\n", boundsGuessAccuracy*100.0f);
                    int numSamples = static_cast<int> (boundsGuessAccuracy*static_cast<float> (size*size));
                    for (int i=0; i<numSamples; ++i) {
                        const real fu = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
                        const real fv = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
                        const real t = smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize) : (*heightFunc) (fu, fv);
                        tmin = t < tmin ? t : tmin;
                        tmax = t > tmax ? t : tmax;
                        if (++updateDisplay > 1000) {
                            updateDisplay = 0;
                            fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (i) / (float) (numSamples), i, numSamples);
                            fflush (stderr);
                        }
                    }
                }
                fprintf (stderr, " done\n");
                const real min = tmin;
                const real irange = 1.0 / (tmax - tmin);

                fprintf (stderr, "  filling heightmap\n");
                updateDisplay = 0;
                for (v = 0; v < size; v++) {
                    const real fv = v / (real) size;
                    for (u = 0; u < size; u++) {
                        const real fu = u / (real) size;
                        m_pField[u+v*size] = rtoh (
                                                 ( (smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize)
                                                    : (*heightFunc) (fu, fv)
                                                   ) - min
                                                 ) * irange
                                             );
                    }
                    /*if( ++updateDisplay > 10000 )*/ {
                        updateDisplay = 0;
                        fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (u + v*size) / (float) (size*size), v, size);
                        fflush (stderr);
                    }
                    /*if( v%(size/10)==0 )
                     fprintf( stderr, "." );*/
                }
                fprintf (stderr, " done\n");
                fprintf (stderr, "}\n");

                updateOptimizerVars();
            }



            // Hack.
            void SimpleHeightField::feedTriScene (::picogen::graphics::objects::abstract::ITriScene *target) const {
                for (unsigned int v=0; v<m_size; ++v) {
                    for (unsigned int u=0; u<m_size; ++u) {
                        Vector3d tris[2][3];
                        getVoxelTriangles (u, v, tris[0], tris[1]);
                        target->insert (tris[0][0], tris[0][1], tris[0][2]);
                        target->insert (tris[1][0], tris[1][1], tris[1][2]);
                    }
                }
            }



            // Below function DEPRECATED
            void SimpleHeightField::init (
                unsigned int size,
                const misc::functions::abstract::uv_to_scalar *heightFunc,
                real boundsGuessAccuracy,
                bool smooth
            ) {
                fprintf (stderr, "building SimpleHeightField %xP{\n", (unsigned int) this);


                if (m_pField != 0) {
                    fprintf (stderr, "  deleting previously allocated heightfield...");
                    delete [] m_pField;
                    fprintf (stderr, "done\n");
                }
                fprintf (stderr, "  allocating %ux%u-heightmap, claiming %.2fMiB (%.3fMTris)...", size, size, float (size*size*sizeof (t_heightVal)) / (1024.0*1024.0), float (size*size*2) / (1e6f));
                m_pField = new t_heightVal[size*size];
                fprintf (stderr, "done\n");
                m_size = size;
                unsigned int u, v;

                const real cellSize = 1.0 / (real) size;

                // get min and max value of function
                int updateDisplay = 99999;
                fprintf (stderr, "  determining bounds of height function %xP\n", (unsigned int) heightFunc);
                real tmin = misc::constants::real_max;
                real tmax = -misc::constants::real_max;
                if (boundsGuessAccuracy>0.9f) {
                    fprintf (stderr, "   doing this at full accuracy (requested was %.1f%%)\n", boundsGuessAccuracy*100.0f);
                    for (v = 0; v < size; v++) {
                        const real fv = v / (real) size;
                        for (u = 0; u < size; u++) {
                            const real fu = u / (real) size;
                            const real t = smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize) : heightFunc->f (fu, fv);
                            tmin = t < tmin ? t : tmin;
                            tmax = t > tmax ? t : tmax;
                        }
                        /*if( ++updateDisplay > 10000 )*/ {
                            updateDisplay = 0;
                            fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (u + v*size) / (float) (size*size), v, size);
                            fflush (stderr);
                        }
                        /*if( v%(size/10)==0 )
                         fprintf( stderr, "." );*/
                    }
                } else {
                    fprintf (stderr, "   using random sampling (requested %.1f%% of accuracy)\n", boundsGuessAccuracy*100.0f);
                    int numSamples = static_cast<int> (boundsGuessAccuracy*static_cast<float> (size*size));
                    for (int i=0; i<numSamples; ++i) {
                        const real fu = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
                        const real fv = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
                        const real t = smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize) : heightFunc->f (fu, fv);
                        tmin = t < tmin ? t : tmin;
                        tmax = t > tmax ? t : tmax;
                        if (++updateDisplay > 1000) {
                            updateDisplay = 0;
                            fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (i) / (float) (numSamples), i, numSamples);
                            fflush (stderr);
                        }
                    }
                }
                fprintf (stderr, " done\n");
                const real min = tmin;
                const real irange = 1.0 / (tmax - tmin);

                fprintf (stderr, "  filling heightmap\n");
                updateDisplay = 0;
                for (v = 0; v < size; v++) {
                    const real fv = v / (real) size;
                    for (u = 0; u < size; u++) {
                        const real fu = u / (real) size;
                        m_pField[u+v*size] = rtoh (
                                                 ( (smooth ? smoothedHeightFunc (heightFunc, fu, fv, cellSize, cellSize)
                                                    : heightFunc->f (fu, fv)
                                                   ) - min
                                                 ) * irange
                                             );
                    }
                    /*if( ++updateDisplay > 10000 )*/ {
                        updateDisplay = 0;
                        fprintf (stderr, "\r   %.4f%% [%u/%u]", 100.0f* (float) (u + v*size) / (float) (size*size), v, size);
                        fflush (stderr);
                    }
                    /*if( v%(size/10)==0 )
                     fprintf( stderr, "." );*/
                }
                fprintf (stderr, " done\n");
                fprintf (stderr, "}\n");

                updateOptimizerVars();
            }



            inline void SimpleHeightField::getVoxelTriangles (int u, int v, Vector3d tA[], Vector3d tB[]) const {
                const bool odd = (u & 1) == (v & 1);
                const int va = (v + 1) * m_size;
                const int vb = (v + 0) * m_size;
                const real a_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[   u + va ]);
                const real b_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[ 1+u + va ]);
                const real c_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[   u + vb ]);
                const real d_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[ 1+u + vb ]);

                const real fu1 = static_cast<real>(u) / static_cast<real>(m_size-1);
                const real fu2 = static_cast<real>(u+1) / static_cast<real>(m_size-1);
                const real fv1 = static_cast<real>(v) / static_cast<real>(m_size-1);
                const real fv2 = static_cast<real>(v+1) / static_cast<real>(m_size-1);

                const real x1 = fu1 * m_scaleSize[0] + m_scaleBoxMin[0];
                const real x2 = fu2 * m_scaleSize[0] + m_scaleBoxMin[0];
                const real z1 = fv2 * m_scaleSize[2] + m_scaleBoxMin[2];
                const real z2 = fv1 * m_scaleSize[2] + m_scaleBoxMin[2];

                const Vector3d A (x1, a_y, z1);
                const Vector3d B (x2, b_y, z1);
                const Vector3d C (x1, c_y, z2);
                const Vector3d D (x2, d_y, z2);

                tA[0] = A;
                tA[1] = B;
                tA[2] = odd ? C : D;

                tB[0] = C;
                tB[1] = odd ? B : A;
                tB[2] = D;
            }



            inline int SimpleHeightField::intersectVoxel (
                param_out (structs::intersection_t, intersection),
                int u, int v,
                real x1, real z1, real x2, real z2,
                param_in (misc::geometrics::Ray, ray)
            ) const {
                const bool odd = (u & 1) == (v & 1);
                const int va = (v + 1) * m_size;
                const int vb = (v + 0) * m_size;
                const real a_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[   u + va ]);
                const real b_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[ 1+u + va ]);
                const real c_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[   u + vb ]);
                const real d_y = m_scaleBoxMin[1] + m_scaleSize[1] * htor (m_pField[ 1+u + vb ]);

                const real A[3] = { x1, a_y, z1 };
                const real B[3] = { x2, b_y, z1 };
                const real C[3] = { x1, c_y, z2 };
                const real D[3] = { x2, d_y, z2 };

                real t, tu, tv;
                real normal[3];

                //> if an intersection is found, than it is necessarily the nearest,
                //> thus we're not going to optimize the if-bodies below
                int ic;
                if (0 != (ic=raytri_intersect (ray, A, B, odd ? C : D, t, tu, tv, normal))) {
                    intersection.side = misc::constants::outside;
                    intersection.t = t;
                    intersection.normal = Vector3d (normal[0], normal[1], normal[2]) * (ic>0?1:-1);
                    intersection.brdf = m_pBRDF;
                    intersection.L_e = 0.0;
                    return 1;
                }


                if (0 != (ic=raytri_intersect (ray, C, odd ? B : A, D, t, tu, tv, normal))) {
                    intersection.side = misc::constants::outside;
                    intersection.t = t;
                    intersection.normal = Vector3d (normal[0], normal[1], normal[2]) * (ic>0?1:-1);
                    intersection.brdf = m_pBRDF;
                    intersection.L_e = 0.0;
                    return 1;
                }//*/

                return 0;
            }



            inline int SimpleHeightField::lineIntersection (
                param_out (structs::intersection_t, intersection),
                real au, real av,
                real bu, real bv,
                param_in (misc::geometrics::Ray, ray)
            ) const {
                using namespace misc::constants;

                int icode = 0;


                int x, z;
                x = (int) au;
                z = (int) av;
                const int stepX = au > bu ? -1 : + 1;
                const int stepZ = av > bv ? -1 : + 1;

                const int kx = (int) bu + stepX;
                /*const int _kx  = __kx>=m_size ? m_size-1 : __kx;
                const int kx   = _kx<0 ? 0 : _kx;*/

                //const int kz = (int)bv+stepZ;
                const int kz = (int) bv + stepZ;
                /*const int _kz  = __kz>=m_size ? m_size-1 : __kz;
                const int kz   = _kz<0 ? 0 : _kz;*/

                /// \todo TODO TODO avoid teh kludge
                //if( x==kx || z==kz )

                /*const int kx = _kx < -1 ? -1 : _kx > (int)m_size ? (int)m_size : _kx;
                const int kz = _kz < -1 ? -1 : _kz > (int)m_size ? (int)m_size : _kz;*/
                const real sx = bu - au;
                const real sz = bv - av;
                const real len = sqrt (sx * sx + sz * sz);
                const real ilen = 1.0 / len;
                const real nx = sx * ilen;
                const real nz = sz * ilen;
                const real deltaX = fabs (1.0 / nx);
                const real deltaZ = fabs (1.0 / nz);
                real tMaxX = stepX > 0 ? fabs ( ( (real) (x + 1) - au) / nx)
                             : fabs ( ( (real) (x) - au)   / nx);
                real tMaxZ = stepZ > 0 ? fabs ( ( (real) (z + 1) - av) / nz)
                             : fabs ( ( (real) (z) - av)   / nz);

                const real ssx = m_scaleSize_mul_invertHMapSize[0];
                const real ssz = m_scaleSize_mul_invertHMapSize[2];


                const real speedX = stepX > 0 ? ssx : -ssx;
                const real speedZ = stepZ > 0 ? ssz : -ssz;
                real x1 = m_scaleBoxMin[0] + (real) (0 + x) * ssx;
                real x2 = x1 + ssx;
                real z2 = m_scaleBoxMin[2] + (real) (0 + z) * ssz;
                real z1 = z2 + ssz;

                while (1) {
                    icode = intersectVoxel (intersection, x, z, x1, z1, x2, z2, ray);
                    if (0 != icode) break;
                    if (tMaxX < tMaxZ) {
                        x += stepX;
                        if (x == kx || x < 0 || x >= (int) m_size) return false;   /// \todo todo
                        tMaxX += deltaX;
                        x1 += speedX;
                        x2 += speedX;
                    } else {
                        z += stepZ;
                        if (z == kz || z < 0 || z >= (int) m_size) return false;   /// \todo todo
                        tMaxZ += deltaZ;
                        z2 += speedZ;
                        z1 += speedZ;
                    }
                }

                if (0 != m_pShader) {
                    m_pShader->shade (
                        intersection.color,
                        intersection.normal,
                        ray.x() + ray.w() * intersection.t
                    );
                } else {
                    intersection.color = image::color::Color (1.0, 1.0, 1.0);
                }


                //const static real isqrt_2 = 1.0/sqrt(2.0);
                /*
                const real diff_x = bu-au;
                const real diff_y = bv-av;
                const real len = sqrt(diff_x*diff_x + diff_y*diff_y);
                int prev_u=-1, prev_v=-1;
                real f;
                for( f=0.0; f<=1.0+epsilon; f+=0.25/len ){
                 const int u = (int)( (1.0-f)*au + f*bu );
                 const int v = (int)( (1.0-f)*av + f*bv );
                 if( ( u!=prev_u || v!=prev_v ) && 0!=(icode=intersectVoxel( intersection, u, v, ray ) ) )
                  break;
                 prev_u = u;
                 prev_v = v;
                 ++ (const_cast<unsigned int&>(numTraversals));
                }//*/
                return icode;
            }



            bool SimpleHeightField::intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const {
                using namespace misc::constants;

                //>> intersect bounding box; eventually return
                real t_min, t_max;
                if (!m_scaleBox.intersect (t_min, t_max, ray))
                    return false;
                //>> saturate t_min to 0; eventually return
                t_min = t_min < epsilon ? epsilon : t_min;
                if (t_min > t_max)
                    return false;

                //++ (const_cast<unsigned int&>(numIntersectQueries));

                const Vector3d gbegin = ray (t_min);
                const Vector3d gend = ray (t_max);
                Vector3d gt = (gend - gbegin);
                gt[1] = 0.0;
                gt = gt.normal();
                //int numSteps = (int)( ( gt.length() / m_scaleSize[0] ) * (real)(m_size-1) );

                Vector3d ibegin = gbegin - m_scaleBox.getMin();
                ibegin[0] /= m_scaleSize[0];
                //ibegin[1] /= m_scaleSize[1];
                ibegin[2] /= m_scaleSize[2];
                ibegin = ibegin * ( (real) (m_size - 1));

                Vector3d iend = gend - m_scaleBox.getMin();
                iend[0] /= m_scaleSize[0];
                //iend[1] /= m_scaleSize[1];
                iend[2] /= m_scaleSize[2];
                iend = iend * ( (real) (m_size - 1));

                return lineIntersection (
                           intersection,
                           ibegin[0], ibegin[2],
                           iend[0],   iend[2],
                           ray
                       ) == 1;

            }



            bool SimpleHeightField::intersect (param_out (intersection_t, intersection), param_in (Ray, ray), real t_min, real t_max) const {
                using namespace misc::constants;

                t_min = t_min < epsilon ? epsilon : t_min;
                if (t_min > t_max)
                    return false;

                //++ (const_cast<unsigned int&>(numIntersectQueries));

                const Vector3d gbegin = ray (t_min);
                const Vector3d gend = ray (t_max);
                Vector3d gt = (gend - gbegin);
                gt[1] = 0.0;
                gt = gt.normal();
                //int numSteps = (int)( ( gt.length() / m_scaleSize[0] ) * (real)(m_size-1) );

                Vector3d ibegin = gbegin - m_scaleBox.getMin();
                ibegin[0] /= m_scaleSize[0];
                //ibegin[1] /= m_scaleSize[1];
                ibegin[2] /= m_scaleSize[2];
                ibegin = ibegin * ((real) (m_size - 1));

                Vector3d iend = gend - m_scaleBox.getMin();
                iend[0] /= m_scaleSize[0];
                //iend[1] /= m_scaleSize[1];
                iend[2] /= m_scaleSize[2];
                iend = iend * ((real) (m_size - 1));

                return lineIntersection (
                           intersection,
                           ibegin[0], ibegin[2],
                           iend[0],   iend[2],
                           ray
                       ) == 1;

            }

            /*inline int SimpleHeightField::intersectVoxel (
                param_out (structs::intersection_t, intersection),
                int u, int v,
                real x1, real z1, real x2, real z2,
                param_in (misc::geometrics::Ray, ray)
            )*/

            bool SimpleHeightField::intersectRectangle (
                param_out (intersection_t, intersection),
                param_in (Ray, ray),
                unsigned int Ax, unsigned int Az, unsigned int Bx, unsigned int Bz
            ) const {

                const int startX = Ax;
                const int startZ = Az;
                const int endX = Bx;
                const int endZ = Bz;

                const real ssx = m_scaleSize_mul_invertHMapSize[0];
                const real ssz = m_scaleSize_mul_invertHMapSize[2];

                const int stepU = 1;
                const int stepV = 1;

                for (int v=startZ; v<=endZ; v+=stepV) {
                    for (int u=startX; u<=endX; u+=stepU) {

                        const real x1 = m_scaleBoxMin[0] + (real) (u) * ssx;
                        const real x2 = x1 + ssx;
                        const real z2 = m_scaleBoxMin[2] + (real) (v) * ssz;
                        const real z1 = z2 + ssz;

                        if (0 != intersectVoxel (intersection, u, v, x1, z1, x2, z2, ray)) {
                            if (0 != m_pShader) {
                                m_pShader->shade (
                                    intersection.color,
                                    intersection.normal,
                                    ray.x() + ray.w() * intersection.t
                                );
                            } else {
                                intersection.color = image::color::Color (1.0, 1.0, 1.0);
                            }
                            return true;
                        }
                    }
                }
                return false;
            }

        }; // namespace objects
    }; // namespace graphics
}; // namespace picogen
