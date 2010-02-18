//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef TRANSFORM_HH_INCLUDED_20100216
#define TRANSFORM_HH_INCLUDED_20100216

#include <stdexcept>

namespace kallisto {
        template <typename T>
        class Transform {
        public: // Construct.

                Transform ()
                : m(identity()), i(m)
                {}


                Transform (
                        T _00, T _01, T _02, T _03,
                        T _10, T _11, T _12, T _13,
                        T _20, T _21, T _22, T _23,
                        T _30, T _31, T _32, T _33
                )
                : m(Transform::set(
                        _00, _01, _02, _03,
                        _10, _11, _12, _13,
                        _20, _21, _22, _23,
                        _30, _31, _32, _33
                  ))
                , i(inverse (m))
                {}

                Transform (
                        T _00, T _01, T _02, T _03,
                        T _10, T _11, T _12, T _13,
                        T _20, T _21, T _22, T _23,
                        T _30, T _31, T _32, T _33,
                        T _i00, T _i01, T _i02, T _i03,
                        T _i10, T _i11, T _i12, T _i13,
                        T _i20, T _i21, T _i22, T _i23,
                        T _i30, T _i31, T _i32, T _i33
                )
                : m(Transform::set(
                        _00, _01, _02, _03,
                        _10, _11, _12, _13,
                        _20, _21, _22, _23,
                        _30, _31, _32, _33
                  ))
                , i(Transform::set(
                        _i00, _i01, _i02, _i03,
                        _i10, _i11, _i12, _i13,
                        _i20, _i21, _i22, _i23,
                        _i30, _i31, _i32, _i33
                  ))
                {}

        private:
                Transform (shared_array<const T> const &m, shared_array<const T> const &i)
                : m(m), i(i)
                {}
        public:

                Transform (Transform const &rhs)
                : m(rhs.m), i(rhs.i)
                {}

                Transform &operator = (Transform const &rhs) {
                        m = rhs.m;
                        i = rhs.i;
                        return *this;
                }

                Transform inverse () const {
                        return Transform (i,m);
                }

                Point<CARTESIAN,T> position () const {
                        if (1.f != (*this)(0,0)
                                || 1.f != (*this)(1,1)
                                || 1.f != (*this)(2,2)
                                || 1.f != (*this)(3,3)
                        ) {
                                std::cerr << "Warning: It has been tried to "
                                "get the position of a scaled Transform, "
                                " which is currently not explitly supported.";
                                std::cerr << std::endl;
                        }
                        return Point<CARTESIAN,T>(
                                (*this)(0,3), (*this)(1,3), (*this)(2,3)
                        );

                }

                Vector<CARTESIAN,T> operator * (Vector<CARTESIAN,T> const &v) const {
                        const T x = v.x, y = v.y, z = v.z;
                        return Vector<CARTESIAN,T>(
                                get(0,0)*x + get(0,1)*y + get(0,2)*z,
                                get(1,0)*x + get(1,1)*y + get(1,2)*z,
                                get(2,0)*x + get(2,1)*y + get(2,2)*z
                        );
                }

                Point<CARTESIAN,T> operator * (Point<CARTESIAN,T> const &v) const {
                        const T x = v.x, y = v.y, z = v.z;
                        const T w   = get(3,0)*x + get(3,1)*y + get(3,2)*z + get(3,3);
                        // w will be 1 for most matrices
                        return Point<CARTESIAN,T>(
                                (get(0,0)*x + get(0,1)*y + get(0,2)*z + get(0,3))/w,
                                (get(1,0)*x + get(1,1)*y + get(1,2)*z + get(1,3))/w,
                                (get(2,0)*x + get(2,1)*y + get(2,2)*z + get(2,3))/w
                        );
                }

                Normal<CARTESIAN,T> operator * (Normal<CARTESIAN,T> const &n) const {
                        const T x = n.x, y = n.y, z = n.z;
                        return Normal<CARTESIAN,T>(
                                geti(0,0)*x + geti(1,0)*y + geti(2,0)*z,
                                geti(0,1)*x + geti(1,1)*y + geti(2,1)*z,
                                geti(0,2)*x + geti(1,2)*y + geti(2,2)*z
                        );
                }

                template <typename R> R operator * (R const & rhs) const {
                        const Point<CARTESIAN,T>  p = vector_cast<Point<CARTESIAN,T> >(rhs.position);
                        const Vector<CARTESIAN,T> w = rhs.direction;
                        const Point<CARTESIAN,T>  p_ = (*this) * p;
                        const Vector<CARTESIAN,T> w_ = (*this) * w;
                        return R (
                                vector_cast<typename R::position_type>(p_),
                                w_
                        );
                }

                Transform operator * (Transform const &rhs) const {
                        return Transform (
                                mul(m,rhs.m),
                                mul(i,rhs.i));
                }

                T operator () (unsigned int u, unsigned int v) const {
                        return m[u+v*4];
                }

        public: // Named constructors.

                // Translation.
                static Transform translation (T x, T y, T z) {
                        return Transform (
                                1, 0, 0, x,
                                0, 1, 0, y,
                                0, 0, 1, z,
                                0, 0, 0, 1,

                                1, 0, 0, -x,
                                0, 1, 0, -y,
                                0, 0, 1, -z,
                                0, 0, 0, 1
                        );
                }
                static Transform translation (const Vector<CARTESIAN,T> &rhs) {
                        return translation (rhs.x, rhs.y, rhs.z);
                }

                // Scaling.
                static Transform scaling (T x, T y, T z) {
                        return Transform (
                                x, 0, 0, 0,
                                0, y, 0, 0,
                                0, 0, z, 0,
                                0, 0, 0, 1,

                                1.f/x, 0, 0, 0,
                                0, 1.f/y, 0, 0,
                                0, 0, 1.f/z, 0,
                                0, 0, 0, 1
                        );
                }
                static Transform scaling (const Vector<CARTESIAN,T> &rhs) {
                        return scaling (rhs.x, rhs.y, rhs.z);
                }

                // Rotation.
                static Transform rotationX (T angle) {
                        const T sin_ = std::sin(angle);
                        const T cos_ = std::cos(angle);
                        return Transform (
                                1,    0,     0,  0,
                                0, cos_, -sin_,  0,
                                0, sin_,  cos_,  0,
                                0,    0,     0,  1,

                                1,    0,     0,  0,
                                0, cos_,  sin_,  0,
                                0, -sin_, cos_,  0,
                                0,    0,     0,  1
                        );
                }

                static Transform rotationY (T angle) {
                        const T sin_ = std::sin(angle);
                        const T cos_ = std::cos(angle);
                        return Transform (
                                cos_,   0, sin_, 0,
                                   0,   1,    0, 0,
                               -sin_,   0, cos_, 0,
                                   0,   0,    0, 1,

                                cos_,   0,-sin_, 0,
                                   0,   1,    0, 0,
                                sin_,   0, cos_, 0,
                                   0,   0,    0, 1
                        );
                }

                static Transform rotationZ (T angle) {
                        const T sin_ = std::sin(angle);
                        const T cos_ = std::cos(angle);
                        return Transform (
                                cos_, -sin_, 0, 0,
                                sin_,  cos_, 0, 0,
                                    0,    0, 1, 0,
                                    0,    0, 0, 1,

                                cos_,  sin_, 0, 0,
                                -sin_, cos_, 0, 0,
                                 0,       0, 1, 0,
                                 0,       0, 0, 1
                        );
                }


        private:
                shared_array<const T> m, i;

                T get (unsigned int u, unsigned int v) const {
                        return m[u+v*4];
                }
                T geti (unsigned int u, unsigned int v) const {
                        return m[u+v*4];
                }

                static T* set (
                        T _00, T _01, T _02, T _03,
                        T _10, T _11, T _12, T _13,
                        T _20, T _21, T _22, T _23,
                        T _30, T _31, T _32, T _33
                ) {
                        T *m = new T[4*4];
                        m[0+4*0] = _00; m[0+4*1] = _01; m[0+4*2] = _02; m[0+4*3] = _03;
                        m[1+4*0] = _10; m[1+4*1] = _11; m[1+4*2] = _12; m[1+4*3] = _13;
                        m[2+4*0] = _20; m[2+4*1] = _21; m[2+4*2] = _22; m[2+4*3] = _23;
                        m[3+4*0] = _30; m[3+4*1] = _31; m[3+4*2] = _32; m[3+4*3] = _33;
                        return m;
                }

                static T* identity() {
                        return set(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
                }

                static shared_array<const T> mul (
                        shared_array<const T> const &lhs,
                        shared_array<const T> const &rhs
                ) {
                        T* r = new T[4*4];
                        for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) {
				r[i+j*4] =
                                        lhs[i + 0*4] * rhs[0 + 4*j] +
				        lhs[i + 1*4] * rhs[1 + 4*j] +
				        lhs[i + 2*4] * rhs[2 + 4*j] +
				        lhs[i + 3*4] * rhs[3 + 4*j]
                                ;
                        }
                        return shared_array<const T>(r);
                }

                // Lifted directly from PBRT, original comments left in.
                static shared_array<const T> inverse (shared_array<const T> const &m) {
                        int indxc[4], indxr[4];
                        int ipiv[4] = { 0, 0, 0, 0 };
                        T *minv = new T [4*4];
                        //memcpy(minv, m, 4*4*sizeof(float));
                        for (int u=0; u<4; ++u) for (int v=0; v<4; ++v) {
                                minv[u+v*4] = m[u+v*4];
                        }

                        for (int i = 0; i < 4; i++) {
                                int irow = -1, icol = -1;
                                T big = T(0);
                                // Choose pivot
                                for (int j = 0; j < 4; j++) {
                                        if (ipiv[j] != 1) {
                                                for (int k = 0; k < 4; k++) {
                                                        if (ipiv[k] == 0) {
                                                                if (fabsf(minv[j+k*4]) >= big) {
                                                                        big = T(std::fabs(minv[j+k*4]));
                                                                        irow = j;
                                                                        icol = k;
                                                                }
                                                        } else if (ipiv[k] > 1) {
                                                                //Error("Singular matrix in MatrixInvert");
                                                        }
                                                }
                                        }
                                }
                                ++ipiv[icol];
                                // Swap rows _irow_ and _icol_ for pivot
                                if (irow != icol) {
                                        for (int k = 0; k < 4; ++k)
                                                swap(minv[irow+k*4], minv[icol+k*4]);
                                }
                                indxr[i] = irow;
                                indxc[i] = icol;
                                if (minv[icol+icol*4] == T(0)) {
                                        //Error("Singular matrix in MatrixInvert");
                                }
                                // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
                                T pivinv = T(1) / minv[icol+icol*4];
                                minv[icol+icol*4] = T(1);
                                for (int j = 0; j < 4; j++)
                                        minv[icol+j*4] *= pivinv;
                                // Subtract this row from others to zero out their columns
                                for (int j = 0; j < 4; j++) {
                                        if (j != icol) {
                                                T save = minv[j+icol*4];
                                                minv[j+icol*4] = 0;
                                                for (int k = 0; k < 4; k++)
                                                        minv[j+k*4] -= minv[icol+k*4]*save;
                                        }
                                }
                        }
                        // Swap columns to reflect permutation
                        for (int j = 3; j >= 0; j--) {
                                if (indxr[j] != indxc[j]) {
                                        for (int k = 0; k < 4; k++)
                                                swap(minv[k+indxr[j]*4], minv[k+indxc[j]*4]);
                                }
                        }
                        return shared_array<const T>(minv);
                }
        };
}


namespace kallisto { namespace unit_test {
        class test_transform_t {
        private:
                const static test_transform_t test;
                test_transform_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // TRANSFORM_HH_INCLUDED_20100216
