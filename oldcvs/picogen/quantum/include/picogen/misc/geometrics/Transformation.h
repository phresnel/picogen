/***************************************************************************
 *            Transformation.h
 *
 *  Tue Nov 13 11:33:24 2007
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


#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H

namespace picogen {
        namespace geometrics {

            /// \todo rewrite whole class. it is crap.
            class Transformation {

                    typedef ::picogen::real real;
                    typedef union _t_matrix {
                        struct {
                            real _11,_12,_13,_14;
                            real _21,_22,_23,_24;
                            real _31,_32,_33,_34;
                            real _41,_42,_43,_44;
                        };
                        real m[4][4];
                    } t_matrix;
                    t_matrix matrix;

                public:
                    Transformation (
                        real _11, real _12, real _13, real _14,
                        real _21, real _22, real _23, real _24,
                        real _31, real _32, real _33, real _34,
                        real _41, real _42, real _43, real _44) {
                        matrix._11 = _11; matrix._12 = _12; matrix._13 = _13; matrix._14 = _14;
                        matrix._21 = _21; matrix._22 = _22; matrix._23 = _23; matrix._24 = _24;
                        matrix._31 = _31; matrix._32 = _32; matrix._33 = _33; matrix._34 = _34;
                        matrix._41 = _41; matrix._42 = _42; matrix._43 = _43; matrix._44 = _44;
                    }

                    Transformation (const Transformation &t) {
                        matrix._11 = t.matrix._11; matrix._12 = t.matrix._12; matrix._13 = t.matrix._13; matrix._14 = t.matrix._14;
                        matrix._21 = t.matrix._21; matrix._22 = t.matrix._22; matrix._23 = t.matrix._23; matrix._24 = t.matrix._24;
                        matrix._31 = t.matrix._31; matrix._32 = t.matrix._32; matrix._33 = t.matrix._33; matrix._34 = t.matrix._34;
                        matrix._41 = t.matrix._41; matrix._42 = t.matrix._42; matrix._43 = t.matrix._43; matrix._44 = t.matrix._44;
                    }

                    Transformation setTo (const Transformation &t) {
                        matrix._11 = t.matrix._11; matrix._12 = t.matrix._12; matrix._13 = t.matrix._13; matrix._14 = t.matrix._14;
                        matrix._21 = t.matrix._21; matrix._22 = t.matrix._22; matrix._23 = t.matrix._23; matrix._24 = t.matrix._24;
                        matrix._31 = t.matrix._31; matrix._32 = t.matrix._32; matrix._33 = t.matrix._33; matrix._34 = t.matrix._34;
                        matrix._41 = t.matrix._41; matrix._42 = t.matrix._42; matrix._43 = t.matrix._43; matrix._44 = t.matrix._44;
                        return *this;
                    }
                    Transformation operator = (const Transformation &t) {
                        matrix._11 = t.matrix._11; matrix._12 = t.matrix._12; matrix._13 = t.matrix._13; matrix._14 = t.matrix._14;
                        matrix._21 = t.matrix._21; matrix._22 = t.matrix._22; matrix._23 = t.matrix._23; matrix._24 = t.matrix._24;
                        matrix._31 = t.matrix._31; matrix._32 = t.matrix._32; matrix._33 = t.matrix._33; matrix._34 = t.matrix._34;
                        matrix._41 = t.matrix._41; matrix._42 = t.matrix._42; matrix._43 = t.matrix._43; matrix._44 = t.matrix._44;
                        return *this;
                    }

                    Transformation() {
                        matrix._11 = 1.0; matrix._12 = 0.0; matrix._13 = 0.0; matrix._14 = 0.0;
                        matrix._21 = 0.0; matrix._22 = 1.0; matrix._23 = 0.0; matrix._24 = 0.0;
                        matrix._31 = 0.0; matrix._32 = 0.0; matrix._33 = 1.0; matrix._34 = 0.0;
                        matrix._41 = 0.0; matrix._42 = 0.0; matrix._43 = 0.0; matrix._44 = 1.0;
                    }


                    inline Transformation setToIdentity() {
                        matrix._11 = 1.0; matrix._12 = 0.0; matrix._13 = 0.0; matrix._14 = 0.0;
                        matrix._21 = 0.0; matrix._22 = 1.0; matrix._23 = 0.0; matrix._24 = 0.0;
                        matrix._31 = 0.0; matrix._32 = 0.0; matrix._33 = 1.0; matrix._34 = 0.0;
                        matrix._41 = 0.0; matrix._42 = 0.0; matrix._43 = 0.0; matrix._44 = 1.0;
                        return *this;
                    }

                    inline Transformation setToScaling (real s) {
                        matrix._11 =   s; matrix._12 = 0.0; matrix._13 = 0.0; matrix._14 = 0.0;
                        matrix._21 = 0.0; matrix._22 =   s; matrix._23 = 0.0; matrix._24 = 0.0;
                        matrix._31 = 0.0; matrix._32 = 0.0; matrix._33 =   s; matrix._34 = 0.0;
                        matrix._41 = 0.0; matrix._42 = 0.0; matrix._43 = 0.0; matrix._44 = 1.0;
                        return *this;
                    }

                    inline Transformation  setToScaling (real x, real y, real z) {
                        matrix._11 =   x; matrix._12 = 0.0; matrix._13 = 0.0; matrix._14 = 0.0;
                        matrix._21 = 0.0; matrix._22 =   y; matrix._23 = 0.0; matrix._24 = 0.0;
                        matrix._31 = 0.0; matrix._32 = 0.0; matrix._33 =   z; matrix._34 = 0.0;
                        matrix._41 = 0.0; matrix._42 = 0.0; matrix._43 = 0.0; matrix._44 = 1.0;
                        return *this;
                    }

                    inline Transformation setToTranslation (real x, real y, real z) {
                        matrix._11 =   1; matrix._12 =   0; matrix._13 =   0; matrix._14 =   0;
                        matrix._21 =   0; matrix._22 =   1; matrix._23 =   0; matrix._24 =   0;
                        matrix._31 =   0; matrix._32 =   0; matrix._33 =   1; matrix._34 =   0;
                        matrix._41 =   x; matrix._42 =   y; matrix._43 =   z; matrix._44 =   1;
                        return *this;
                    }

                    inline Transformation setToTranslation (const Vector3d &v) {
                        matrix._11 =   1; matrix._12 =   0; matrix._13 =   0; matrix._14 =   0;
                        matrix._21 =   0; matrix._22 =   1; matrix._23 =   0; matrix._24 =   0;
                        matrix._31 =   0; matrix._32 =   0; matrix._33 =   1; matrix._34 =   0;
                        matrix._41 =v[0]; matrix._42 =v[1]; matrix._43 =v[2]; matrix._44 =   1;
                        return *this;
                    }

                    inline Transformation setToRotationX (real angle) {
                        register const real sina = sin (angle);
                        register const real cosa = cos (angle);
                        matrix._11 =    1; matrix._12 =    0; matrix._13 =    0; matrix._14 =    0;
                        matrix._21 =    0; matrix._22 = cosa; matrix._23 = sina; matrix._24 =    0;
                        matrix._31 =    0; matrix._32 =-sina; matrix._33 = cosa; matrix._34 =    0;
                        matrix._41 =    0; matrix._42 =    0; matrix._43 =    0; matrix._44 =    1;
                        return *this;
                    }

                    inline Transformation setToRotationY (real angle) {
                        register const real sina = sin (angle);
                        register const real cosa = cos (angle);
                        matrix._11 = cosa; matrix._12 =    0; matrix._13 =-sina; matrix._14 =    0;
                        matrix._21 =    0; matrix._22 =    1; matrix._23 =    0; matrix._24 =    0;
                        matrix._31 = sina; matrix._32 =    0; matrix._33 = cosa; matrix._34 =    0;
                        matrix._41 =    0; matrix._42 =    0; matrix._43 =    0; matrix._44 =    1;
                        return *this;
                    }

                    inline Transformation setToRotationZ (real angle) {
                        register const real sina = sin (angle);
                        register const real cosa = cos (angle);
                        matrix._11 = cosa; matrix._12 = sina; matrix._13 =    0; matrix._14 =    0;
                        matrix._21 =-sina; matrix._22 = cosa; matrix._23 =    0; matrix._24 =    0;
                        matrix._31 =    0; matrix._32 =    0; matrix._33 =    1; matrix._34 =    0;
                        matrix._41 =    0; matrix._42 =    0; matrix._43 =    0; matrix._44 =    1;
                        return *this;
                    }


                    /*inline void Transform( vector *pOut, const matrix *mat, const vector *vec )
                    {
                     pOut->x = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31 + mat->_41;
                     pOut->y = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32 + mat->_42;
                     pOut->z = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33 + mat->_43;
                    }*/
                    inline Vector3d operator * (const Vector3d &vec) const {
                        return Vector3d (
                                   vec[0]*matrix._11 + vec[1]*matrix._21 + vec[2]*matrix._31 + matrix._41,
                                   vec[0]*matrix._12 + vec[1]*matrix._22 + vec[2]*matrix._32 + matrix._42,
                                   vec[0]*matrix._13 + vec[1]*matrix._23 + vec[2]*matrix._33 + matrix._43
                               );
                    }

                    inline Transformation operator * (const Transformation &b) const {
                        Transformation res;

                        register int u,v;
                        for (v=0; v<4; v++) for (u=0; u<4; u++)
                                res.matrix.m[u][v] =
                                    matrix.m[u][0]*b.matrix.m[0][v] +
                                    matrix.m[u][1]*b.matrix.m[1][v] +
                                    matrix.m[u][2]*b.matrix.m[2][v] +
                                    matrix.m[u][3]*b.matrix.m[3][v];

                        return res;
                    }

                    inline Transformation setToPerspectiveFovLH (real fov, real aspectRatio, real zn, real zf) {
                        real h = 1.0 / tan (fov*0.5);
                        real w = h / aspectRatio;
                        matrix._11=w;
                        matrix._12=0;
                        matrix._13=0;
                        matrix._14=0;
                        matrix._21=0;
                        matrix._22=h;
                        matrix._23=0;
                        matrix._24=0;
                        matrix._31=0;
                        matrix._32=0;
                        matrix._33=zf/ (zf-zn);
                        matrix._34=1;
                        matrix._41=0;
                        matrix._42=0;
                        matrix._43=-zn*zf/ (zf-zn);
                        matrix._44=0;
                        return *this;
                    }

                    inline Transformation setToLookAtLH (const Vector3d &eye, const Vector3d &at, const Vector3d &up) {

                        Vector3d X,Y,Z;
                        Z = (at - eye).normal();
                        X = Z.cross (up).normal();
                        Y = Z.cross (X).normal();

                        matrix._11 =  X[0];
                        matrix._12 =  Y[0];
                        matrix._13 =  Z[0];
                        matrix._14 =    0;
                        matrix._21 =  X[1];
                        matrix._22 =  Y[1];
                        matrix._23 =  Z[1];
                        matrix._24 =    0;
                        matrix._31 =  X[2];
                        matrix._32 =  Y[2];
                        matrix._33 =  Z[2];
                        matrix._34 =    0;

                        matrix._41 = - (X*eye);
                        matrix._42 = - (Y*eye);
                        matrix._43 = - (Z*eye);
                        matrix._44 = 1;
                        return *this;
                    }



                private:
                    inline real sarrus (
                        real _11, real _12, real _13,
                        real _21, real _22, real _23,
                        real _31, real _32, real _33
                    ) const {
                        return  _11*_22*_33 - _11*_23*_32
                                + _12*_23*_31 - _12*_21*_33
                                + _13*_21*_32 - _13*_22*_31;
                    }


                    inline real subdet33 (int v, int u) const {
                        int u1, u2, u3, v1, v2, v3;

                        switch (u) {
                            case 0:
                                u1=1;
                                u2=2;
                                u3=3;
                                break;
                            case 1:
                                u1=0;
                                u2=2;
                                u3=3;
                                break;
                            case 2:
                                u1=0;
                                u2=1;
                                u3=3;
                                break;
                            default:
                                u1=0;
                                u2=1;
                                u3=2;
                                break;
                        };
                        switch (v) {
                            case 0:
                                v1=1;
                                v2=2;
                                v3=3;
                                break;
                            case 1:
                                v1=0;
                                v2=2;
                                v3=3;
                                break;
                            case 2:
                                v1=0;
                                v2=1;
                                v3=3;
                                break;
                            default:
                                v1=0;
                                v2=1;
                                v3=2;
                                break;
                        };


                        return sarrus (matrix.m[v1][u1], matrix.m[v1][u2], matrix.m[v1][u3],
                                       matrix.m[v2][u1], matrix.m[v2][u2], matrix.m[v2][u3],
                                       matrix.m[v3][u1], matrix.m[v3][u2], matrix.m[v3][u3]);
                    }


                public:
                    inline real getDeterminant() const {
                        const
                        real det11 = matrix._11 * sarrus (matrix._22, matrix._23, matrix._24,
                                                          matrix._32, matrix._33, matrix._34,
                                                          matrix._42, matrix._43, matrix._44
                                                         );
                        const
                        real det12 = matrix._12 * sarrus (matrix._21, matrix._23, matrix._24,
                                                          matrix._31, matrix._33, matrix._34,
                                                          matrix._41, matrix._43, matrix._44
                                                         );
                        const
                        real det13 = matrix._13 * sarrus (matrix._21, matrix._22, matrix._24,
                                                          matrix._31, matrix._32, matrix._34,
                                                          matrix._41, matrix._42, matrix._44
                                                         );
                        const
                        real det14 = matrix._14 * sarrus (matrix._21, matrix._22, matrix._23,
                                                          matrix._31, matrix._33, matrix._33,
                                                          matrix._41, matrix._42, matrix._43
                                                         );

                        return (+det11 -det12 +det13 -det14);
                    }


                    inline Transformation getInverse (real *pDet = 0) const {
                        const real det = getDeterminant();
                        if (0 != pDet)
                            *pDet = det;
                        const real detinv = 1.0 / det;


                        Transformation out;
                        int u,v;
                        for (v=0; v<4; v++)
                            for (u=0; u<4; u++)
                                out.matrix.m[v][u] = detinv * powf (-1, (real) (u+v)) * subdet33 (u, v);

                        return out;
                    }



                    inline Transformation getTranspose() const {
                        Transformation out;
                        int y,x;
                        for (y=0; y<4; y++)
                            for (x=0; x<4; x++)
                                out.matrix.m[x][y] = matrix.m[y][x];
                        return out;
                    }
            };

        } // namespace geometrics {
} //namespace picogen {



#endif /* _TRANSFORMATION_H */
