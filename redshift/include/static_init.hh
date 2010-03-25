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

#ifndef STATIC_INIT_HH_INCLUDED_20100317
#define STATIC_INIT_HH_INCLUDED_20100317

#include "setup.hh"
namespace redshift {
        inline void static_init() {
                redshift::SpectrumBase<float,spectrum_samples>::static_init();
                redshift::SpectrumBase<float,longspectrum_samples>::static_init();
                redshift::SpectrumBase<float,referencespectrum_samples>::static_init();
                redshift::SpectrumBase<float,halfreferencespectrum_samples>::static_init();

                redshift::SpectrumBase<double,spectrum_samples>::static_init();
                redshift::SpectrumBase<double,longspectrum_samples>::static_init();
                redshift::SpectrumBase<double,referencespectrum_samples>::static_init();
                redshift::SpectrumBase<double,halfreferencespectrum_samples>::static_init();

                redshift::SpectrumBase<long double,spectrum_samples>::static_init();
                redshift::SpectrumBase<long double,longspectrum_samples>::static_init();
                redshift::SpectrumBase<long double,referencespectrum_samples>::static_init();
                redshift::SpectrumBase<long double,halfreferencespectrum_samples>::static_init();
        }
}

#endif // STATIC_INIT_HH_INCLUDED_20100317
