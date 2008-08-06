/***************************************************************************
 *            ImplicitHeightField.h
 *
 *  Wed Aug 06 2008
 *  Copyright  2008  Sebastian Mach
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


#ifndef _IMPLICITHEIGHTFIELD_H
#define _IMPLICITHEIGHTFIELD_H

#include <picogen/graphics/objects.h>
#include <picogen/misc/functional.h>

namespace picogen {
    namespace graphics {
        namespace objects {



            class ImplicitHeightField : public ::picogen::graphics::objects::abstract::IIntersectable {
                    /// \todo de-uglify typenames
                private:
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::misc::functional::Function_R2_R1 Function_R2_R1;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;

                private:

                    const IBRDF *brdf;
                    const IShader *shader;
                    const Function_R2_R1 *function;
                    Vector3d boxMin, boxMax, boxSize, boxSizeInv;

                public:
                    ImplicitHeightField();
                    virtual ~ImplicitHeightField();
                    void setBox (param_in (Vector3d, min), param_in (Vector3d, max));
                    void setBRDF (const IBRDF* brdf);
                    void setShader (const IShader* shader);
                    void setFunction (const Function_R2_R1* function);

                    virtual bool intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const;
            };

        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {



#endif /* _IMPLICITHEIGHTFIELD_H */
