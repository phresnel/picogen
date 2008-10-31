/***************************************************************************
 *            CloudAdapter.h
 *
 *  2008-10-24 21:32
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


#ifndef CLOUDADAPTER_H
#define CLOUDADAPTER_H

namespace picogen {
    namespace graphics {
        namespace objects {

            class CloudAdapter : public ::picogen::graphics::objects::abstract::ISky {

                    typedef ::picogen::real real;
                    typedef ::picogen::geometrics::Vector3d Vector3d;
                    typedef ::picogen::geometrics::Ray Ray;
                    typedef ::picogen::graphics::color::Color Color;
                    typedef ::picogen::graphics::objects::Preetham Preetham;
                    typedef ::picogen::misc::functional::Function_R2_R1 Function_R2_R1;
                    typedef ::picogen::misc::functional::Function_R2_R1_Refcounted Function_R2_R1_Refcounted;

                private:
                    const Preetham &preetham;
                    boost::intrusive_ptr<Function_R2_R1_Refcounted> fun;

                public:
                    CloudAdapter (const Preetham &preetham, boost::intrusive_ptr<Function_R2_R1_Refcounted> fun);
                    CloudAdapter (const Preetham &preetham);
                    void setCloudFunction (boost::intrusive_ptr<Function_R2_R1_Refcounted> fun);
                    Color operator () (const Ray &ray) const;

                    void shade (param_out (Color,color), param_in (Ray,ray)) const;
                    void sunShade (param_out (Color,color), param_in (Ray,ray)) const;
                    void sunSample (param_out (Color,color), param_out (Ray,ray), param_out (real,p), param_in (Vector3d,position)) const;
                    void atmosphereShade (param_out (Color,color), param_in (Color,src_color), param_in (Ray,ray), real distance) const;
                    void getSunColor (param_out (Color,color)) const;
                    void getSunDirection (param_out (Vector3d,direction)) const;
                    real getSunArealFactor () const;
            };

        } // namespace objects {
    } // namespace graphics {
}//namespace picogen {


#endif /* CLOUDADAPTER_H */
