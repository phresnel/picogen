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

#ifndef SCENE_HH_INCLUDED_20090303
#define SCENE_HH_INCLUDED_20090303
        
namespace redshift {
        
        DefineFinalizer(Scene);
        
        class Scene : DoFinalize(Scene) {
        public:        
                
                Scene(shared_ptr<RenderTarget>, shared_ptr<camera::Camera>,
                                             shared_ptr<primitive::Primitive>);
                ~Scene ();
                
                void render(shared_ptr<ProgressReporter const>) const ;                
        private:
                // non copyable
                // TODO use NonCopyable base class instead
                Scene (Scene const &);
                Scene & operator= (Scene const &);
                Scene();
                
                
                inline tuple<real_t,Color> Li(
                                  RayDifferential const&, Sample const&) const;
                        
                inline bool doesIntersect (RayDifferential const &ray) const;
                
                inline tuple<bool,Intersection> intersect(
                                             RayDifferential const &ray) const;
                        
                

                //Scene scene;
                shared_ptr<RenderTarget>         renderTarget;
                shared_ptr<camera::Camera>       camera;
                shared_ptr<primitive::Primitive> aggregate;
        };
}

#endif // SCENE_HH_INCLUDED_20090303
