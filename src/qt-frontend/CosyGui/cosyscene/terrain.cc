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

#include "terrain.hh"

namespace cosyscene {


const actuarius::Enum<Terrain::Kind> Terrain::Typenames =
( actuarius::Nvp<Terrain::Kind>(Terrain::QuatschSource, "quatsch-source")
);



Terrain::Terrain() {
}



Terrain::Kind Terrain::kind() const {
        return kind_;
}



void Terrain::toQuatschSource (cosyscene::QuatschSource const &qs) {
        quatschSource_ = qs;
        kind_ = QuatschSource;
}



cosyscene::QuatschSource Terrain::quatschSource() const {
        return quatschSource_;
}



void Terrain::stash() {
        stash_.stash(*this);
        stash_.back().clearStash();
}



void Terrain::clearStash() {
        stash_.clear();
}



const Stash<Terrain>& Terrain::getStash() const {
        return stash_;
}



Stash<Terrain>& Terrain::getStash() {
        return stash_;
}



void Terrain::setStash(Stash<Terrain> const & stash) {
        stash_ = stash;
}



}
