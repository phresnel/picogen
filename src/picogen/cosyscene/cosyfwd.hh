//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#ifndef COSYFWD_HH
#define COSYFWD_HH

namespace picogen { namespace cosyscene {
        class Camera;
        class PinholeCamera;
        class CylindricalCamera;
        class CubemapFaceCamera;

        class Color;
        class WavelengthAmplitudePair;
        class Spectrum;
        class Rgb;

        class CosyFloat;
        class Direction3d;
        class Point3d;
        class Vector3d;

        class UtahSky;
        class SunSky;

        class SurfaceIntegrator;
        class NoneIntegrator;
        class WhittedIntegrator;
        class WhittedAmbientIntegrator;
        class PathIntegrator;
        class DebugDistanceIntegrator;
        class DebugNormalsIntegrator;

        class QuatschSource;
        class QuatschPreset;
        class TerrainFormation;
        class TerrainFitting;
        class Terrain;

        class WaterFitting;
        class Water;

        class StringKeyValue;

        class Material;

        class FilmSettings;
        class RenderSettings;
        class TwinRenderSettings;

        class Scene;

        class YawPitchRoll;
        class Navigation;
} }

#endif // COSYFWD_HH
