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

#ifndef RENDERSETTINGS_HH_20101019
#define RENDERSETTINGS_HH_20101019

#include "redshift/include/smart_ptr.hh"
#include <actuarius/bits/enum.hh>

#include "stash.hh"
#include "surfaceintegrator.hh"



namespace picogen { namespace cosyscene {



class RenderSettings : public StashableMixin<RenderSettings>
{
public:
        RenderSettings();

        unsigned int width() const;
        unsigned int height() const;
        unsigned int samplesPerPixel() const;
        unsigned int randomSeed() const;
        unsigned int maxLazyQuadtreeDepth() const;

        void setWidth(unsigned int);
        void setHeight(unsigned int);
        void setSamplesPerPixel(unsigned int);
        void setRandomSeed(unsigned int);
        void setMaxLazyQuadtreeDepth (unsigned int);

        SurfaceIntegrator surfaceIntegrator() const;
        void setSurfaceIntegrator(SurfaceIntegrator const &);

        template<typename Arch> void serialize (Arch &arch);

        bool data_equals(RenderSettings const &rhs) const;
private:

        unsigned int width_, height_;
        unsigned int randomSeed_;
        unsigned int samplesPerPixel_;

        unsigned int maxLazyQuadtreeDepth_;

        SurfaceIntegrator surfaceIntegrator_;

        void reset();
};



class TwinRenderSettings {
public:
        TwinRenderSettings ();

        redshift::shared_ptr<RenderSettings> preview() const;
        redshift::shared_ptr<RenderSettings> production() const;

        template<typename Arch> void serialize (Arch &arch);

private:
        redshift::shared_ptr<RenderSettings> preview_;
        redshift::shared_ptr<RenderSettings> production_;
};

} }

#endif // RENDERSETTINGS_HH_20101019
