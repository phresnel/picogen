//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#ifndef COMPUTATIONTIME_HH_INCLUDED_20100620
#define COMPUTATIONTIME_HH_INCLUDED_20100620

#include <ctime>
namespace picogen { namespace redshift {

	// Should give the total CPU time, i.e. if you have a quadcore box,
	// then you get normal time * 4 (roughly, that is)
        class ComputationTime {
                double begin, end;
                bool stopped;
        public:
                ComputationTime ()
                : begin (clock() / (double)CLOCKS_PER_SEC)
                , stopped(false)
                {}

                void restart () {
                        stopped = false;
                        begin = clock() / (double)CLOCKS_PER_SEC;
                }

                double stop () {
                        stopped = true;
                        end = clock() / (double)CLOCKS_PER_SEC;
                        return end-begin;
                }

                double operator () () const volatile {
                        if (stopped) {
                                return end - begin;
                        } else {
                                return (clock() / (double)CLOCKS_PER_SEC) - begin;
                        }
                }

        };
} }

#endif // COMPUTATIONTIME_HH_INCLUDED_20100620
