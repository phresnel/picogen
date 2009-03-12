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

#ifndef REDSHIFTRENDER_H_INCLUDED_20090303
#define REDSHIFTRENDER_H_INCLUDED_20090303

namespace redshift {
        DefineFinalizer(Renderer);
        class Renderer : DoFinalize(Renderer) {
        public:        
                Renderer(shared_ptr<RenderTarget>, shared_ptr<camera::Camera>);
                void render() const ;
                ~Renderer ();
                
                inline tuple<real_t,Color> computeRadiance(
                        RayDifferential const&, Sample const&) const;
        private:
                Renderer (Renderer const &);
                Renderer & operator= (Renderer const &);
                Renderer();

                //Scene scene;
                shared_ptr<RenderTarget> renderTarget;
                shared_ptr<camera::Camera> camera;
        };
}

#endif // REDSHIFTRENDER_H_INCLUDED_20090303
