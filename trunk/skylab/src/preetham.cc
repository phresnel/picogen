/***************************************************************************
 *            skyshader_Preetham.cc
 *
 *  Mon Aug 20 00:56:04 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
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


#include "../include/preetham.hh"


//using namespace picogen;
//using geometrics::Vector3d;
//using graphics::color::Color;



/// \todo replace orthnormalbasis_from_normal with the version from pbrt (it has no loop inside)
static inline void orthnormalbasis_from_normal (
        redshift::Vector &u,
        redshift::Vector &v,
        redshift::Vector const &n
) {
        using namespace redshift;
        do {
                // look for any vector that is not parallel to n
                const Vector tmp (
                        (real_t) rand() / (real_t) RAND_MAX,
                        (real_t) rand() / (real_t) RAND_MAX,
                        (real_t) rand() / (real_t) RAND_MAX
                );
                u = cross (n, tmp);
        } while (fabs (u*n) > redshift::constants::epsilon);
        u = normalize(u);
        v = cross (n,u);
}

static inline redshift::real_t to_radian (redshift::real_t a) {
        return a* (redshift::constants::pi/180.0);
}
static inline redshift::real_t to_degree (redshift::real_t a) {
        return a* (180.0/redshift::constants::pi);
}

static inline redshift::real_t angle2d (
        redshift::real_t x, redshift::real_t z
) {
        if (z<0) {
                return acos (-x);
        }
        return (2*redshift::constants::pi)-acos (-x);
}


static inline redshift::real_t getAngleBetween (
        redshift::real_t thetav,
        redshift::real_t phiv,
        redshift::real_t theta,
        redshift::real_t phi
) {
        using namespace redshift;
        real_t cospsi = sin (thetav) * sin (theta) * cos (phi-phiv)
                        + cos (thetav) * cos (theta);
        if (cospsi > 1) return 0;
        if (cospsi < -1) return redshift::constants::pi;
        return  acos (cospsi);
}

/*
static void Yxy_RGB (real_t *p_color, real_t Y, real_t x, real_t y) {
    real_t CIE_X = x * (Y / y);
    real_t CIE_Y = Y;
    real_t CIE_Z = (1-x-y) * (Y / y);

    //CIE_X /= 100.0;
    //CIE_Y /= 100.0;
    //CIE_Z /= 100.0;

    p_color[0] =   3.240479 * CIE_X  - 1.537150 * CIE_Y  - 0.498535 * CIE_Z;
    p_color[1] = - 0.969256 * CIE_X  + 1.875991 * CIE_Y  + 0.041556 * CIE_Z;
    p_color[2] =   0.055648 * CIE_X  - 0.204043 * CIE_Y  + 1.057311 * CIE_Z;

    for (int i=0; i<3; i++) {
        if (p_color[i] > 0.0031308) {
            p_color[i] = 1.055 * (pow (p_color[i], (1/2.4))) - 0.055;
        } else {
            p_color[i] = 12.92 * p_color[i];
        }
    }
}
*/




namespace redshift { namespace background {
Preetham::Preetham() :
        m_T (1.8), m_sunSolidAngleFactor (1), m_sunColor (100,100,100), m_colorFilter (1,1,1),
        m_enableSunFalloffHack (false), m_enableFogHack (false) {
}



void Preetham::setSunDirection (Vector3d pw) {
        const Vector3d w = normalize(-pw);
        m_sunDirection = -w;

        m_sunTheta = acos (m_sunDirection.y);
        m_sunPhi   = atan2 (m_sunDirection.z,m_sunDirection.x);
}



Vector Preetham::getSunDirection () const {
        return m_sunDirection;
}



// coming from official Preetham code (http://www.cs.utah.edu/vissim/papers/sunsky/)
// and thus from IES Lighting Handbook p361
void Preetham::setSunDirection (
        real_t latitude,
        real_t longitude,
        int _standardMeridian,          // standardMeridian , time zone number, east to west and zero based
        int julianDay,                 // julianDay
        real_t timeOfDay     // timeOfDay
        ) {
        real_t standardMeridian = (real_t) _standardMeridian*15.0;

        real_t solarTime = timeOfDay +
           (0.170*sin (4.0*redshift::constants::pi* (julianDay - 80.0) /373.0) - 0.129*sin (2.0*redshift::constants::pi* (julianDay - 8.0) /355.0)) +
           (standardMeridian - longitude) /15.0;

        real_t solarDeclination = (0.4093*sin (2*redshift::constants::pi* (julianDay - 81.0) /368.0));

        real_t solarAltitude =
        asin (sin (to_radian (latitude)) * sin (solarDeclination)
              - cos (to_radian (latitude)) * cos (solarDeclination) * cos (redshift::constants::pi*solarTime/12.0))
        ;

        real_t opp, adj;
        opp = -cos (solarDeclination) * sin (redshift::constants::pi*solarTime/12.0);
        adj = - (cos (to_radian (latitude)) * sin (solarDeclination) +
        sin (to_radian (latitude)) * cos (solarDeclination) * cos (redshift::constants::pi*solarTime/12.0)
        );
        real_t solarAzimuth=atan2 (opp,adj);

        m_sunPhi   = (solarAzimuth); // the original implementation takes the negative, but then below polar2cartesian triade is not correct
        m_sunTheta = redshift::constants::pi / 2.0 - solarAltitude;

        m_sunDirection.x = sin (m_sunPhi) * cos (m_sunTheta);
        m_sunDirection.z = sin (m_sunPhi) * sin (m_sunTheta);
        m_sunDirection.y = cos (m_sunPhi);
        m_sunDirection = normalize(m_sunDirection);
}



void Preetham::setSunColor (Color color) {
        m_sunColor = color;
}



Preetham::Color Preetham::getSunColor () const {
        return m_sunColor;
}



void Preetham::setColorFilter (Color color) {
        m_colorFilter = color;
}



void Preetham::setTurbidity (real_t t) {
        m_T = t;
}



void Preetham::setSunSolidAngleFactor (real_t f) {
        m_sunSolidAngleFactor = f;
}



/*void Preetham::setSunFalloffMaxSolidAngleFactor (real_t f) {
        m_sunFalloffHackMaxSolidAngleFactor = f;
}*/



/*void Preetham::setSunFalloffExponent (real_t e) {
        m_falloffHackExponent = e;
}*/



void Preetham::setSunFalloffHackParameters (real_t a, real_t b, real_t c) {
        m_falloffHackFactor [0] = a;
        m_falloffHackFactor [1] = b;
        m_falloffHackFactor [2] = c;
}



void Preetham::enableSunFalloffHack (bool enable) {
        m_enableSunFalloffHack = enable;
}



void Preetham::invalidate() {
        const real_t T2 = m_T*m_T;
        const real_t theta2 = m_sunTheta*m_sunTheta;
        const real_t theta3 = theta2*m_sunTheta;

        m_zenith_x =
        (+0.00165*theta3 - 0.00374*theta2 + 0.00208*m_sunTheta + 0)       * T2 +
        (-0.02902*theta3 + 0.06377*theta2 - 0.03202*m_sunTheta + 0.00394) * m_T +
        (+0.11693*theta3 - 0.21196*theta2 + 0.06052*m_sunTheta + 0.25885);

        m_zenith_y =
        (+0.00275*theta3 - 0.00610*theta2 + 0.00316*m_sunTheta  + 0)       * T2 +
        (-0.04214*theta3 + 0.08970*theta2 - 0.04153*m_sunTheta  + 0.00515) * m_T +
        (+0.15346*theta3 - 0.26756*theta2 + 0.06669*m_sunTheta  + 0.26688);

        const real_t X = (4.0/9.0-m_T/120.0) * (redshift::constants::pi-2*m_sunTheta);
        m_zenith_Y = (4.0453*m_T-4.9710) * tan (X) - 0.2155*m_T+2.4192;
        //m_zenith_Y *= 1000;  // convert kcd/m^2 to cd/m^2

        m_perez_Y[0] =    0.17872 *m_T  - 1.46303;
        m_perez_Y[1] =   -0.35540 *m_T  + 0.42749;
        m_perez_Y[2] =   -0.02266 *m_T  + 5.32505;
        m_perez_Y[3] =    0.12064 *m_T  - 2.57705;
        m_perez_Y[4] =   -0.06696 *m_T  + 0.37027;

        m_perez_x[0] =   -0.01925 *m_T  - 0.25922;
        m_perez_x[1] =   -0.06651 *m_T  + 0.00081;
        m_perez_x[2] =   -0.00041 *m_T  + 0.21247;
        m_perez_x[3] =   -0.06409 *m_T  - 0.89887;
        m_perez_x[4] =   -0.00325 *m_T  + 0.04517;

        m_perez_y[0] =   -0.01669 *m_T  - 0.26078;
        m_perez_y[1] =   -0.09495 *m_T  + 0.00921;
        m_perez_y[2] =   -0.00792 *m_T  + 0.21023;
        m_perez_y[3] =   -0.04405 *m_T  - 1.65369;
        m_perez_y[4] =   -0.01092 *m_T  + 0.05291;


        m_sunSolidAngle =  360.0* (6.7443e-05) *m_sunSolidAngleFactor;//0.25*constants::pi*1.39*1.39/(150*150);  // = 6.7443e-05
        //m_sunFalloffHackMaxSolidAngle = 360.0* (6.7443e-05) * m_sunFalloffHackMaxSolidAngleFactor;//0.25*constants::pi*1.39*1.39/(150*150);  // = 6.7443e-05
        //m_beta = 0.04608365822050 * m_T - 0.04586025928522;
}



real_t Preetham::perez (real_t Theta, real_t gamma, const real_t p[]) {
        return (1.0 + p[0]*exp (p[1]/cos (Theta))) * (1.0 + p[2]*exp (p[3]*gamma) + p[4]*pow (cos (gamma),2.0));
}



Color Preetham::shade (Ray in_ray) const {
        real_t /*rho,*/ theta, phi;

        Ray ray = in_ray;

        if (ray.direction.y<0.001) {
                //color.from_rgb(1,0,0); return;
                ray.direction.y = 0.001;
                ray.direction = normalize (ray.direction);
        }
        theta = acos (ray.direction.y);
        if (fabs (theta) < 1e-5) phi = 0;
        else  phi = atan2 (ray.direction.z,ray.direction.x);

        const real_t gamma = acos (ray.direction * m_sunDirection);//*/getAngleBetween( theta, phi, m_sunTheta, m_sunPhi );
        const real_t x = m_zenith_x * (perez (theta, gamma, m_perez_x) / perez (0.0, m_sunTheta, m_perez_x));
        const real_t y = m_zenith_y * (perez (theta, gamma, m_perez_y) / perez (0.0, m_sunTheta, m_perez_y));
        const real_t Y = m_zenith_Y * (perez (theta, gamma, m_perez_Y) / perez (0.0, m_sunTheta, m_perez_Y));


        /*real_t c[3];
        Yxy_RGB (c, Y, x, y);
        color.from_rgb (c[0],c[1],c[2]);*/
        Color color;
        color.fromYxy (Y, x, y);
        return multiplyComponents (color, m_colorFilter);
}



Color Preetham::sunShade (Ray ray) const {
        const real_t gamma = acos (ray.direction * m_sunDirection);//*/getAngleBetween( theta, phi, m_theta, m_phi );

        if (gamma < m_sunSolidAngle) {
                return m_sunColor;
        } else if (m_enableSunFalloffHack) {
                real_t f = gamma-m_sunSolidAngle;
                const real_t i_ = (1/exp(m_falloffHackFactor [0] * f))+m_falloffHackFactor [1]*(1/exp(m_falloffHackFactor [2]*f));
                const real_t i = i_ < 0.0 ? 0.0 : i_ > 1.0 ? 1.0 : i_;
                return m_sunColor * i;
        }

        return Color (.0f, .0f, .0f);
}



redshift::tuple<Color,Ray,real_t> Preetham::sunSample (Point position) const {
        //> we will use the disk sampler from "Monte Carlo Techniques
        //> for Direct Lighting Calculations" (Shirley,Wang,Zimmerman)
        /// \todo: modify so that the solid angle is projected onto the unit sphere accurately
        const Vector3d c = m_sunDirection;
        const real_t     r = m_sunSolidAngle;
        const Vector3d n = -m_sunDirection;
        const real_t    e1 = (real_t) rand() / (real_t) RAND_MAX;
        const real_t    e2 = (real_t) rand() / (real_t) RAND_MAX;
        Vector3d u,v;
        orthnormalbasis_from_normal (u,v,n);
        const Vector3d R (
                r*sqrt (e1) *cos (2.0*redshift::constants::pi*e2),
                r*sqrt (e1) *sin (2.0*redshift::constants::pi*e2),
                0.0
        );


        const Ray ray (
                position,
                Vector3d (
                        c.x + u.x*R.x + v.x*R.y + n.x*R.z,
                        c.y + u.y*R.x + v.y*R.y + n.y*R.z,
                        c.z + u.z*R.x + v.z*R.y + n.z*R.z
                )
        );

        const Color color =
                m_sunColor * (m_sunSolidAngle/ (4.0*redshift::constants::pi));

        return make_tuple(color,ray,1.0f);
        /*
        //> simple, directional sampler
        color = m_sunColor * (m_sunSolidAngle/(4.0*redshift::constants::pi));
        ray.direction = m_sunDirection;
        ray.x() = position;
        p = 1.0;
        */
}



real_t Preetham::getSunArealFactor () const {
        return (m_sunSolidAngle/ (4.0*redshift::constants::pi));
}



Color Preetham::atmosphereShade (Color src_color, Ray ray, real_t distance) const {
        if (m_enableFogHack) {
                const Color sky_color = shade (ray);
                const real_t distance_ = distance>m_fogHackSatDist?m_fogHackSatDist:distance;
                //const real_t d = 1.0 / (1+m_fogHackFactor*distance_); // 0.00082 was a cool value for '2097'
                const real_t d = 1.0 / exp (pow (m_fogHackFactor*distance_, 1));
                return src_color*d + sky_color* (1-d);
        } else {
                return src_color;
        }
}



void Preetham::enableFogHack (bool enable, real_t f, real_t satDist) {
        m_enableFogHack = enable;
        m_fogHackFactor = f;
        m_fogHackSatDist = satDist;
}



} } // namespace
