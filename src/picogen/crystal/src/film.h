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

#ifndef FILM_H
#define FILM_H

#include <vector>
#include "crystal_math.h"

namespace crystal {

        class Radiance;
        class CameraSample;

        class Film {
        public:
                Film (int width, int height /*real crop[4]*/);

                void addPostProcessor ();
                void addSample (CameraSample const &cs, Radiance const &rad);

                Radiance operator() (int x, int y) const ;

                int width () const;
                int height() const;

        private:
                class Average {
                public:
                        Average ();
                        void operator+= (Radiance const &rad);
                        Radiance radiance() const;
                private:
                        Radiance radiance_;
                        int count_;
                };

                std::vector<Average> surface_;
                int width_, height_;
        };

}

#endif // FILM_H
