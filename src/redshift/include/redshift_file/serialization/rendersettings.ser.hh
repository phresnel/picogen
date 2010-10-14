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

#ifndef RENDERSETTINGS_SER_HH_20101013
#define RENDERSETTINGS_SER_HH_20101013

namespace redshift_file {
        template<typename Arch>
        void RenderSettings::serialize (Arch &arch) {
                using actuarius::pack;

                //arch & pack ("title", title);
                arch & pack ("width", width);
                arch & pack ("height", height);
                arch & actuarius::push_optional(true);
                arch & pack ("min-y", min_y);
                arch & pack ("max-y", max_y);
                arch & pack ("seed", userSeed);
                arch & pack ("samples-per-pixel", samplesPerPixel);
                arch & pack ("surface-integrator", surfaceIntegrator);
                arch & actuarius::pop_optional;
                if (Arch::deserialize || volumeIntegrator.type != VolumeIntegrator::none) {
                        arch & actuarius::push_optional(true)
                             & pack ("volume-integrator", volumeIntegrator)
                             & actuarius::pop_optional
                        ;
                }
        }
}

#endif // RENDERSETTINGS_SER_HH_20101013
