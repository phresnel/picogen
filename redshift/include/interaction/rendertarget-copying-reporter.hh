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

#ifndef RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100227
#define RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100227

#include "progressreporter.hh"
#include "../auxiliary/stopwatch.hh"

namespace redshift { namespace interaction {
class RenderTargetCopyingReporter : public ProgressReporter {
public:

        RenderTargetCopyingReporter (
                RenderTarget::Ptr src,
                RenderTarget::Ptr target_
        )
        : source (src), target (target_)
        {}



        void report (RenderTarget::ReadLockPtr sourcel,
                                              int completed, int total) const {
                if (lastTime() < 0.5)
                        return;
                lastTime.restart();

                /*real_t const finished = static_cast<real_t>(completed)
                                          / static_cast<real_t>(total);*/
                /*
                if (total>0.0) {
                        cout << real_t(100)*(finished) << "%"
                             << endl;
                } else {
                        cout << completed << endl;
                }
                */
                copy (source, sourcel, target);
                target->flip();
        }



        void reportDone () const {
                redshift::copy (source, target);
                target->flip();
        }

private:
        RenderTargetCopyingReporter();
        RenderTargetCopyingReporter(RenderTargetCopyingReporter const &);
        RenderTargetCopyingReporter &
                        operator = (RenderTargetCopyingReporter const &);

        RenderTarget::ConstPtr source;
        RenderTarget::Ptr target;

        mutable StopWatch lastTime;
};

} }

#endif // RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100227
