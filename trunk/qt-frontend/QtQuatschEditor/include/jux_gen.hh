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

#ifndef JUX_GEN_HH
#define JUX_GEN_HH

#include "kallisto/common.hh"
#include "quatsch/quatsch.hh"
#include "quatsch/frontend/jux.hh"
#include "quatsch/backend/est/backend.hh"

#include "quatsch/configurable-functions/noise2ddef.hh"
#include "quatsch/configurable-functions/layerednoise2ddef.hh"

#include <QString>

//-----
typedef quatsch::backend::est::Backend <float, const float *> backend_t;

typedef backend_t::Function Function;
typedef backend_t::FunctionPtr FunctionPtr;
typedef backend_t::scalar_t scalar_t;
typedef backend_t::parameters_t parameters_t;

typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;
//-----


class JuxGeneratorState {
public:
        JuxGeneratorState () : indendation(0) {}

        QString getIndendationString () const {
                QString ret;
                for (int i=0; i<indendation; ++i)
                        ret += "    ";
                return ret;
        }


        friend class Frame;
        class Frame {
        public:
                ~Frame () { --*indent; }
                Frame (Frame const &f) : indent(f.indent) {}
                Frame & operator = (Frame const &f) {
                        indent = f.indent;
                        return *this;
                }
        private:
                friend class JuxGeneratorState;
                int *indent;
                Frame (int *indent_) : indent(indent_) { ++*indent; }
                Frame () ;
        };
        Frame getFrame () { return Frame (&indendation); }
private:
        int indendation;
};


#endif // JUX_GEN_HH
