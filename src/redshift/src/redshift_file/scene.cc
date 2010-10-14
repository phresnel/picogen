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

#include "redshift_file/scene.hh"
#include "redshift_file/scene.impl.hh"

namespace redshift_file {

Scene() : impl(new SceneImpl)
{
}

Scene::Scene (Scene const &rhs) : impl (new SceneImpl(*rhs.impl))
{
}

Scene& operator= (Scene const &rhs) {
        impl.reset (new SceneImpl(*rhs.impl));
}
                

void Scene::addRenderSettings (RenderSettings const &rs) {
        impl->renderSettings_.push_back (rs);
}
unsigned int Scene::renderSettingsCount() const {
        return impl->renderSettings_.size();
}
RenderSettings Scene::renderSettings(unsigned int index) const {
        return impl->renderSettings_[index];
}
RenderSettings & Scene::renderSettings(unsigned int index) {
        return impl->renderSettings_[index];
}
void Scene::pruneRenderSettings () {
        impl->renderSettings_.clear();
}

void Scene::addObject (Object const &o) {
        impl->objects_.push_back (o);
}
unsigned int Scene::objectCount() const {
        return impl->objects_.size();
}
Object const & Scene::object(unsigned int index) const {
        return impl->objects_[index];
}


void Scene::addVolume (Volume const &o) {
        impl->volumes_.push_back (o);
}
unsigned int Scene::volumeCount() const {
        return impl->volumes_.size();
}
Volume const & Scene::volume(unsigned int index) const {
        return impl->volumes_[index];
}

void Scene::addBackground (Background const &o) {
        impl->backgrounds_.push_back (o);
}
unsigned int Scene::backgroundCount() const {
        return impl->backgrounds_.size();
}
Background const & Scene::background(unsigned int index) const {
        return impl->backgrounds_[index];
}


void Scene::addCamera (Camera const &o) {
        impl->cameras_.push_back (o);
}
unsigned int Scene::cameraCount() const {
        return impl->cameras_.size();
}
Camera const & Scene::camera(unsigned int index) const {
        return impl->cameras_[index];
}
void Scene::pruneCameras () {
        impl->cameras_.clear();
}

FilmSettings Scene::filmSettings () const {
        return impl->filmSettings_;
}
void Scene::setFilmSettings (FilmSettings const &fs) {
        impl->filmSettings_ = fs;
}

}
