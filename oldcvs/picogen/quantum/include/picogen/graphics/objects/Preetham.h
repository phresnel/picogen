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


class Preetham : public abstract::ISky {
private:
    typedef misc::prim::real              real;
    typedef misc::geometrics::Vector3d    Vector3d;
    typedef misc::geometrics::Ray         Ray;
    typedef graphics::image::color::Color Color;

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

    static real Perez( real Theta, real gamma, const real ABCDE[] );
public:
    Preetham();
    virtual void Shade( param_out(Color,color), param_in(Ray,ray) ) const;
    virtual void SunShade( param_out(Color,color), param_in(Ray,ray) ) const;
    virtual void SunSample( param_out(Color,color), param_out(Ray,ray), param_out(real,p), param_in(Vector3d,position) ) const;
    virtual void AtmosphereShade( param_out(Color,color), param_in(Color,src_color), param_in(Ray,ray), real distance ) const;
    Vector3d GetSunDirection() const;
    void SetSunDirection( param_in(Vector3d,ray) );
    void SetSunDirection( real lat, real longi, int sm, int jd, real tOfDay );
    void SetTurbidity( real t );
    void SetSunSolidAngleFactor( real f );
    void SetSunColor( Color col );
    Color GetSunColor() const;
    void SetColorFilter( Color col );
    void Invalidate();
    void EnableFogHack( bool enable, real f, real satDist );
};

#endif /* _PREETHAM_H */
