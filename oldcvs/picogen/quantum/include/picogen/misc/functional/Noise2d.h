/***************************************************************************
 *            Noise2d.h
 *
 *  2008-10-31
 *  Copyright 2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <map>
#include <picogen/picogen.h>

namespace picogen {
    namespace misc {
        namespace functional {

            class Noise2d : public BasicFunction {

                private:
                    typedef ::picogen::real real;

                    BasicFunction *ufun, *vfun;

                    enum filter_t {
                        nearest,
                        bilinear,
                        cosine
                    };
                    filter_t filter;

                    //unsigned int noiseDepth;
                    unsigned int *offsetLut;
                    real *rngLut;
                    unsigned int offsetLutMask;
                    unsigned int offsetLutSize;
                    real_t frequency;

                    real_t operator () (real_t x, real_t y, real_t domainScale, real_t rangeScale, unsigned int depth ) const;

                public:
                    Noise2d (std::map<std::string,std::string> &parameters, BasicFunction *ufun, BasicFunction *vfun);
                    virtual ~Noise2d();
                    virtual real_t operator () (const real_t * const parameters) const;

            };
        } // namespace functional {
    } // namespace misc {
} // namespace picogen
