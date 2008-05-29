/***************************************************************************
 *            preetham.h
 *
 *  Mon Oct 15 18:15:23 2007
 *  Copyright  2007  Sebastian Mach
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


#ifndef _PREETHAM_H
#define _PREETHAM_H

namespace picogen {
    namespace graphics {
        namespace objects {

            class Preetham : public ::picogen::graphics::objects::abstract::ISky {

                private:
                    typedef ::picogen::misc::prim::real              real;
                    typedef ::picogen::misc::geometrics::Vector3d    Vector3d;
                    typedef ::picogen::misc::geometrics::Ray         Ray;
                    typedef ::picogen::graphics::image::color::Color Color;

                private:
                    Vector3d m_sunDirection;
                    real m_T;
                    real m_sunTheta, m_sunPhi;
                    real m_zenith_x, m_zenith_y, m_zenith_Y;
                    real m_perez_x[5], m_perez_y[5], m_perez_Y[5];
                    real m_sunSolidAngle, m_sunSolidAngleFactor;
                    Color m_sunColor, m_colorFilter;
                    real m_beta;

                    bool m_enableFogHack;
                    real m_fogHackFactor, m_fogHackSatDist;

                private:
                    static real perez (real Theta, real gamma, const real ABCDE[]);

                public:

                    // -- misc ------------------------------------------------
                    Preetham();
                    void invalidate();
                    // --------------------------------------------------------



                    // -- shading / sampling ----------------------------------
                    virtual void shade (param_out (Color,color), param_in (Ray,ray)) const;
                    virtual void sunShade (param_out (Color,color), param_in (Ray,ray)) const;
                    virtual void sunSample (param_out (Color,color), param_out (Ray,ray), param_out (real,p), param_in (Vector3d,position)) const;
                    virtual void atmosphereShade (param_out (Color,color), param_in (Color,src_color), param_in (Ray,ray), real distance) const;
                    // --------------------------------------------------------



                    // -- sun direction / solid angle -------------------------
                    Vector3d getSunDirection() const;
                    void setSunDirection (param_in (Vector3d,ray));
                    void setSunDirection (real lat, real longi, int sm, int jd, real tOfDay);
                    void setSunSolidAngleFactor (real f);
                    // --------------------------------------------------------



                    // -- sun color -------------------------------------------
                    void setSunColor (Color col);
                    Color getSunColor() const;
                    void setColorFilter (Color col);
                    // --------------------------------------------------------



                    // -- atmosphere ------------------------------------------
                    void setTurbidity (real t);
                    void enableFogHack (bool enable, real f, real satDist);
                    // --------------------------------------------------------



            };

        }//namespace picogen {
    }// namespace graphics {
}//  namespace objects {

#endif /* _PREETHAM_H */
