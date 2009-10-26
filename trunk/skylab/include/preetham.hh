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

#include "redshift.hh"

namespace redshift { namespace background {
class Preetham {
private:
        typedef ::redshift::real_t    real;
        typedef ::redshift::Vector    Vector3d;
        typedef ::redshift::Point     Point3d;
        typedef ::redshift::Ray       Ray;
        typedef ::redshift::Color     Color;
private:
        Vector3d m_sunDirection;
        real m_T;
        real m_sunTheta, m_sunPhi;
        real m_zenith_x, m_zenith_y, m_zenith_Y;
        real m_perez_x[5], m_perez_y[5], m_perez_Y[5];
        real m_sunSolidAngle, m_sunSolidAngleFactor;
        Color m_sunColor, m_colorFilter;
        real m_beta;

        bool m_enableSunFalloffHack;
        //real m_sunFalloffHackMaxSolidAngleFactor;
        //real m_falloffHackExponent;
        real m_falloffHackFactor [3];


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
        Color shade (Ray ray) const;
        Color sunShade (Ray ray) const;
        redshift::tuple<Color,Ray,real> sunSample (Point3d position) const;
        Color atmosphereShade (Color src_color, Ray ray, real distance) const;
        // --------------------------------------------------------



        // -- sun direction / solid angle -------------------------
        void setSunDirection (Vector3d ray);
        void setSunDirection (real lat, real longi, int sm, int jd, real tOfDay);
        void setSunSolidAngleFactor (real f);
        //Vector3d getSunDirection() const;
        Vector3d getSunDirection () const;
        real getSunArealFactor () const;
        // --------------------------------------------------------



        // -- falloff hack ----------------------------------------
        //void setSunFalloffMaxSolidAngleFactor (real f);
        //void setSunFalloffExponent (real e);
        void setSunFalloffHackParameters (real a, real b, real c);
        void enableSunFalloffHack (bool enable);
        // --------------------------------------------------------



        // -- sun color -------------------------------------------
        void setSunColor (Color col);
        void setColorFilter (Color col);
        //Color getSunColor() const;
        Color getSunColor () const;
        // --------------------------------------------------------



        // -- atmosphere ------------------------------------------
        void setTurbidity (real t);
        void enableFogHack (bool enable, real f, real satDist);
        // --------------------------------------------------------



};

} }// namespace

#endif /* _PREETHAM_H */
