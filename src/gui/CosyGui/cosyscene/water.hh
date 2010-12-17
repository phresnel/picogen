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


#ifndef WATER_HH_20101217
#define WATER_HH_20101217

#include "stash.hh"
#include "terrain.hh"
#include <string>
#include <actuarius/bits/enum.hh>

namespace cosyscene {
class QuatschSource;
} // namespace cosyscene



#include "vector3d.hh"
namespace cosyscene {
class WaterFitting
{
public:
        WaterFitting();

        void setSizeFactor (Vector3d);
        Vector3d sizeFactor() const;

        void setPositionOffset (Vector3d);
        Vector3d positionOffset() const;

        void setLazyQuadtreeMaxRecursion (unsigned int);
        unsigned int lazyQuadtreeMaxRecursion() const;

        void setLazyQuadtreeVisibleExtent (double);
        double lazyQuadtreeVisibleExtent() const;

        template <typename Arch> void serialize (Arch &arch);
private:
        Vector3d sizeFactor_, positionOffset_;
        unsigned int lazyQuadtreeMaxRecursion_;
        double lazyQuadtreeVisibleExtent_;
};
} // namespace cosyscene


#include "redshift/include/smart_ptr.hh"
namespace cosyscene {

class Water
{
public:
        Water() ;

        redshift::shared_ptr<TerrainFormation> formation() const;
        redshift::shared_ptr<Material>  material() const;
        redshift::shared_ptr<WaterFitting> fitting() const;

        template<typename Arch>
        void serialize (Arch &arch);

private:
        redshift::shared_ptr<TerrainFormation> formation_;
        redshift::shared_ptr<Material> material_;
        redshift::shared_ptr<WaterFitting> fitting_;
};

} // namespace cosyscene

#endif // WATER_HH_20101217
