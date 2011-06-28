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

#ifndef FILM_RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100713
#define FILM_RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100713

#include "progressreporter.hh"
#include "../auxiliary/stopwatch.hh"

namespace picogen { namespace redshift {
        class Film;
} }

namespace picogen { namespace redshift { namespace interaction {
class FilmToRenderTargetCopyingReporter : public ProgressReporter {
public:

        FilmToRenderTargetCopyingReporter (
                shared_ptr<const Film> src,
                RenderTarget::Ptr target_
        )
        : source (src), target (target_)
        {}



        void report (int /*completed*/, int /*total*/) {
                if (lastTime() < 2.5)
                        return;

                StopWatch f;
                doCopy();
                lastTime.restart();
        }



        void reportDone () {
                doCopy();
        }

private:

        void doCopy() {
                {
                        RenderTarget::WriteLockPtr writeLock = target->lock();
                        for (unsigned int y=0; y<source->height(); ++y) {
                                for (unsigned int x=0; x<source->width(); ++x) {
                                        writeLock->setPixel(x, y,
                                                            source->average_or_zero(x, y));
                                }
                        }
                }
                target->flip();
        }

        FilmToRenderTargetCopyingReporter();
        FilmToRenderTargetCopyingReporter(FilmToRenderTargetCopyingReporter const &);
        FilmToRenderTargetCopyingReporter &
                        operator = (FilmToRenderTargetCopyingReporter const &);

        shared_ptr<const Film> source;
        RenderTarget::Ptr target;

        mutable StopWatch lastTime;
};

} } }

#endif // FILM_RENDERTARGET_COPYING_REPORTER_HH_INCLUDED_20100713
