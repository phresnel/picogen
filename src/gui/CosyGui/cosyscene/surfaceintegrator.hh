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

#ifndef SURFACEINTEGRATOR_HH_20101215
#define SURFACEINTEGRATOR_HH_20101215

#include "stash.hh"
#include "redshift/include/smart_ptr.hh"
#include <actuarius/bits/enum.hh>

namespace cosyscene {

class NoneIntegrator {
public: template<typename Arch> void serialize (Arch &) {}
        bool operator == (NoneIntegrator) {return true;}
};

class WhittedIntegrator {
public: template<typename Arch> void serialize (Arch &) {}
        bool operator == (WhittedIntegrator) {return true;}
};

class WhittedAmbientIntegrator {
public:
        explicit WhittedAmbientIntegrator(unsigned int numAmbientSamples=16);
        void setNumAmbientSamples (unsigned int);
        unsigned int numAmbientSamples() const;
        template<typename Arch> void serialize (Arch &arch);
private:
        unsigned int numAmbientSamples_;
};
bool operator ==(WhittedAmbientIntegrator const&, WhittedAmbientIntegrator const&);

class PathIntegrator {
public: template<typename Arch> void serialize (Arch &) {}
        bool operator == (PathIntegrator) {return true;}
};

class DebugDistanceIntegrator {
public: template<typename Arch> void serialize (Arch &) {}
        bool operator == (DebugDistanceIntegrator) {return true;}
};

class DebugNormalsIntegrator {
public: template<typename Arch> void serialize (Arch &) {}
        bool operator == (DebugNormalsIntegrator) {return true;}
};

class SurfaceIntegrator {
public:
        enum Kind {
                none,
                whitted,
                whitted_ambient,
                path,
                debug_distance,
                debug_normals
        };
        static const actuarius::Enum<Kind> Typenames;

        SurfaceIntegrator();

        Kind kind() const;

        void to (Kind);
        void toNoneIntegrator          (NoneIntegrator           const& = NoneIntegrator());
        void toWhittedIntegrator       (WhittedIntegrator        const& = WhittedIntegrator());
        void toWhittedAmbientIntegrator(WhittedAmbientIntegrator const& = WhittedAmbientIntegrator());
        void toPathIntegrator          (PathIntegrator           const& = PathIntegrator());
        void toDebugDistanceIntegrator (DebugDistanceIntegrator  const& = DebugDistanceIntegrator());
        void toDebugNormalsIntegrator  (DebugNormalsIntegrator   const& = DebugNormalsIntegrator());

        NoneIntegrator           noneIntegrator ()          const;
        WhittedIntegrator        whittedIntegrator ()       const;
        WhittedAmbientIntegrator whittedAmbientIntegrator() const;
        PathIntegrator           pathIntegrator()           const;
        DebugDistanceIntegrator  debugDistanceIntegrator()  const;
        DebugNormalsIntegrator   debugNormalsIntegrator()   const;

        template<typename Arch> void serialize (Arch &arch);

private:
        NoneIntegrator none_;
        WhittedIntegrator whitted_;
        WhittedAmbientIntegrator whittedAmbient_;
        PathIntegrator path_;
        DebugDistanceIntegrator debugDistance_;
        DebugNormalsIntegrator debugNormals_;

        Kind kind_;
};
bool operator == (SurfaceIntegrator const &lhs, SurfaceIntegrator const &rhs);

} // namespace cosyscene

#endif // SURFACEINTEGRATOR_HH_20101215
