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

#ifndef RENDERTARGET_HH_INCLUDED_20090306
#define RENDERTARGET_HH_INCLUDED_20090306

#include <stdexcept>

namespace redshift {
        class RenderTarget {
        public:
                // TODO add a getFaceCount(), add parameter to lock()<-int face
                virtual int getWidth () const = 0;
                virtual int getHeight () const = 0;
                virtual shared_ptr<RenderTargetLock> lock () = 0;
                virtual shared_ptr<RenderTargetLock const> lock () const = 0;
                virtual void flip () = 0;
                virtual ~RenderTarget() {}
                
                typedef shared_ptr<RenderTarget> Ptr;
                typedef shared_ptr<RenderTarget const> ConstPtr;

                typedef shared_ptr<RenderTargetLock> LockPtr;
                typedef shared_ptr<RenderTargetLock const> ReadLockPtr;
                typedef shared_ptr<RenderTargetLock> WriteLockPtr;
        };
        
        template <typename lhs_t>
        inline shared_ptr<RenderTarget>
        convert (RenderTarget::ConstPtr rhs) {
                shared_ptr<RenderTarget> lhs (
                        new lhs_t (rhs->getWidth(), rhs->getHeight()));
                shared_ptr<RenderTargetLock> lockr (rhs->lock());
                shared_ptr<RenderTargetLock> lockl (lhs->lock());
                for (int y=0; y<lhs->getHeight(); ++y)
                 for (int x=0; x<lhs->getWidth(); ++x) {
                        lockl->setPixel (x, y, lockr->getPixel (x, y));
                }
                return lhs;
        }
        
        inline void
        copy (RenderTarget::ConstPtr source, RenderTarget::Ptr target) {
                int const width = source->getWidth() < target->getWidth()
                                ? source->getWidth()
                                : target->getWidth();
                int const height = source->getHeight() < target->getHeight()
                                ? source->getHeight()
                                : target->getHeight();

                RenderTarget::ReadLockPtr sourcel (source->lock());
                RenderTarget::WriteLockPtr targetl (target->lock());

                for (int y=0; y<height; ++y)
                 for (int x=0; x<width; ++x) {
                        targetl->setPixel (x, y, sourcel->getPixel (x, y));
                }
        }
        
        inline void
        copy (RenderTarget::ConstPtr source, RenderTarget::ReadLockPtr sourcel, 
                                                    RenderTarget::Ptr target) {
                int const width = source->getWidth() < target->getWidth()
                                ? source->getWidth()
                                : target->getWidth();
                int const height = source->getHeight() < target->getHeight()
                                ? source->getHeight()
                                : target->getHeight();

                RenderTarget::WriteLockPtr targetl (target->lock());

                for (int y=0; y<height; ++y)
                 for (int x=0; x<width; ++x) {
                        targetl->setPixel (x, y, sourcel->getPixel (x, y));
                }
        }
}

#endif // RENDERTARGET_HH_INCLUDED_20090306
