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


#ifndef TERRAIN_HH_20100902
#define TERRAIN_HH_20100902

#include "stash.hh"
#include <string>
#include <actuarius/bits/enum.hh>

namespace cosyscene {
class QuatschSource;
class Terrain;

class QuatschSource {
public:
        QuatschSource() {}
        QuatschSource(std::string const &code) : code_(code) {}

        void setCode (std::string const &code) { code_ = code; }
        std::string code () const { return code_; }

        template<typename Arch>
        void serialize (Arch &arch);
private:
        std::string code_;
};


class Terrain
{
public:
        enum Kind {
                QuatschSource
        };

        Terrain();

        Kind kind() const;
        void toQuatschSource (cosyscene::QuatschSource const &qs);
        cosyscene::QuatschSource quatschSource() const;

        const Stash<Terrain>& getStash() const;
        Stash<Terrain>& getStash();

        void setStash(Stash<Terrain> const &);

        void stash();
        void clearStash();

        template<typename Arch>
        void serialize (Arch &arch);
        static const actuarius::Enum<Kind> Typenames;
private:
        Stash<Terrain> stash_;
        Kind kind_;

        cosyscene::QuatschSource quatschSource_;
};

} // namespace cosyscene

#endif // TERRAIN_HH_20100902
