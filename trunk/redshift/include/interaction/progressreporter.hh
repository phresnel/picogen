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

#ifndef PROGRESSREPORTER_HH_INCLUDED_20090317
#define PROGRESSREPORTER_HH_INCLUDED_20090317

namespace redshift { class RenderTargetLock; }

namespace redshift { namespace interaction {
        class ProgressReporter {
        public:
                virtual void report (shared_ptr<RenderTargetLock const> rlock,
                                        int completed, int total) const = 0;
                virtual void reportDone () const = 0;
                
                typedef shared_ptr<ProgressReporter> Ptr;
                typedef shared_ptr<ProgressReporter const> ConstPtr;
        };
} }

#endif // PROGRESSREPORTER_HH_INCLUDED_20090317
