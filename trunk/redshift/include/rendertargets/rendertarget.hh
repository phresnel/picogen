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

namespace redshift {
        class RenderTarget {
        public:
                // TODO add a getFaceCount(), add parameter to lock()<-int face
                virtual int getWidth () const = 0;
                virtual int getHeight () const = 0;
                virtual shared_ptr<RenderTargetLock> lock () = 0;
                virtual void flip () = 0;
                virtual ~RenderTarget() {}
        };
        
        template <typename lhs_t>
        inline shared_ptr<RenderTarget> convert (shared_ptr<RenderTarget> rhs){
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
}

#endif // RENDERTARGET_HH_INCLUDED_20090306
