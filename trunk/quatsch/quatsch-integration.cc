//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include "kallisto/common.hh"

#include <iostream>
#include <vector>
#include <string>
#include <map>

#ifdef AMALGAM
#include "../../../quatsch/quatsch.hh"
#include "../../../quatsch/frontend/jux.hh"
#include "../../../quatsch/frontend/juxdef.hh"
#include "../../../quatsch/backend/est/backend.hh"
#include "../../../quatsch/backend/est/backenddef.hh"
#include "../../../quatsch/configurable-functions/noise2ddef.hh"
#else
#include "quatsch/quatsch.hh"
#include "quatsch/frontend/jux.hh"
#include "quatsch/frontend/juxdef.hh"
#include "quatsch/backend/est/backend.hh"
#include "quatsch/backend/est/backenddef.hh"
#include "quatsch/configurable-functions/noise2ddef.hh"
#endif

template class quatsch::backend::est::Backend <double, const double *> ;
template class quatsch::backend::est::Backend <float, const float *> ;

template class quatsch::frontend::jux::Compiler <
    quatsch::backend::est::Backend <float, const float *>
>;

template class quatsch::frontend::jux::Compiler <
    quatsch::backend::est::Backend <double, const double *>
>;
