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

#include "rendersettings.hh"

namespace cosyscene {


RenderSettings::RenderSettings() {
        reset();
}

unsigned int RenderSettings::width() const {
        return width_;
}
unsigned int RenderSettings::height() const {
        return height_;
}
unsigned int RenderSettings::samplesPerPixel() const {
        return samplesPerPixel_;
}
unsigned int RenderSettings::randomSeed() const {
        return randomSeed_;
}
unsigned int RenderSettings::maxLazyQuadtreeDepth() const {
        return maxLazyQuadtreeDepth_;
}

void RenderSettings::setWidth(unsigned int val) {
        width_ = val;
}
void RenderSettings::setHeight(unsigned int val) {
        height_ = val;
}
void RenderSettings::setSamplesPerPixel(unsigned int val) {
        samplesPerPixel_ = val;
}
void RenderSettings::setRandomSeed(unsigned int val) {
        randomSeed_ = val;
}
void RenderSettings::setMaxLazyQuadtreeDepth(unsigned int val) {
        maxLazyQuadtreeDepth_ = val;
}

void RenderSettings::reset() {
        width_ = 640;
        height_ = 480;
        samplesPerPixel_ = 1;
        randomSeed_ = 0;
        maxLazyQuadtreeDepth_ = 0;
}

bool RenderSettings::data_equals(RenderSettings const &rhs) const {
        return width_ == rhs.width_
            && height_ == rhs.height_
            && samplesPerPixel_ == rhs.samplesPerPixel_
            && randomSeed_ == rhs.randomSeed_
            && maxLazyQuadtreeDepth_ == rhs.maxLazyQuadtreeDepth_
        ;
}




TwinRenderSettings::TwinRenderSettings()
: preview_(new RenderSettings())
, production_(new RenderSettings())
{}

redshift::shared_ptr<RenderSettings> TwinRenderSettings::production() const {
        return production_;
}
redshift::shared_ptr<RenderSettings> TwinRenderSettings::preview() const {
        return preview_;
}

}
