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

#ifndef COLORRENDERTARGET_HH_INCLUDED_20090307
#define COLORRENDERTARGET_HH_INCLUDED_20090307

namespace redshift {

        DefineFinalizer(ColorRenderTarget);
        class ColorRenderTarget
                : public redshift::RenderTarget
                , DoFinalize(ColorRenderTarget) {
        public:
                ColorRenderTarget (int width_, int height_);
                ~ColorRenderTarget();
                shared_ptr<RenderTargetLock> lock ();
                void flip();        
                int getWidth() const;        
                int getHeight() const;

        private:         
                int width, height;
                shared_array<Color> display;

                ColorRenderTarget();
                ColorRenderTarget(ColorRenderTarget const&);
                ColorRenderTarget & operator = (ColorRenderTarget const &);
                
                class ColorRenderTargetLock;
                friend class ColorRenderTargetLock;
        };

} // namespace redshift

#endif // COLORRENDERTARGET_HH_INCLUDED_20090307
