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

#ifndef ORTHONORMAL_BASIS_H_INCLUDED_20090223
#define ORTHONORMAL_BASIS_H_INCLUDED_20090223

namespace kallisto {
        template <typename vector_t> struct orthonormal_basis_t {
                vector_t X, Y, Z;
        };
}


namespace kallisto {
        template <typename vector_t> class OrthonormalBasis {
        public:
                vector_t X, Y, Z;

                vector_t
                operator () (
                   typename traits::get_scalar_type<vector_t>::type const &x,
                   typename traits::get_scalar_type<vector_t>::type const &y,
                   typename traits::get_scalar_type<vector_t>::type const &z
                ) const {
                        return X*x + Y*y + Z*z;
                }
        };
}


namespace kallisto { namespace unit_test {
        class test_orthonormal_basis_t {
        private:
                const static test_orthonormal_basis_t test;
                test_orthonormal_basis_t () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // ORTHONORMAL_BASIS_H_INCLUDED_20090223
