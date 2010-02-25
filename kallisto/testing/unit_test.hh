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

#ifndef UNIT_TEST_H_INCLUDED_20090215
#define UNIT_TEST_H_INCLUDED_20090215

namespace kallisto { namespace unit_test {
#if 0
        //  \[\033[1;34m\] $\[\033[0m\]
# define UNIT_TEST_REPORT_BEGIN(x) do {                           \
        ::std::cout << "\\[\\033[1;34m\\]"                        \
                    << __FILE__ << ":"                            \
                    << __LINE__ << ":Beginning Unit-Test! "       \
                    << (x)                                        \
                    << "\\[\\033[0m\\]"                           \
                    << ::std::endl; }while(0)
# define UNIT_TEST_REPORT_FAIL(x) do {\
        ::std::cout << __FILE__ << ":" << __LINE__ << ":Test failed! " << (x) << ::std::endl;}while(0)
#else
# define UNIT_TEST_REPORT_BEGIN(x) do { ::std::cout << __FILE__ << ":" << __LINE__ << ":Beginning Unit-Test." << (x) << ::std::endl; } while(false)
# define UNIT_TEST_PRINT(x) do { ::std::cout << __FILE__ << ":" << __LINE__ << ":" << (x) << ::std::endl; } while(false)
# define UNIT_TEST_WARN(x) do { ::std::cout << __FILE__ << ":" << __LINE__ << ":warning:" << (x) << ::std::endl; } while(false)
# define UNIT_TEST_REPORT_FAIL(x) do { ::std::cout << "\t" << __FILE__ << ":" << __LINE__ << ":Test failed! " << (x) << ::std::endl; } while(false)
#endif

        template <typename T> bool cmpz_epsilon(T const &subject, T const &epsilon) {
                return subject >= -epsilon && subject <= epsilon;
        }

        template <typename T> bool cmp_epsilon(T const &subject, T const &comp, T const &epsilon) {
                return (subject-comp) >= -epsilon && (subject-comp) <= epsilon;
        }
} }

#endif // UNIT_TEST_H_INCLUDED_20090215
