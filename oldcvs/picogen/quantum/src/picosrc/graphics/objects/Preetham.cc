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



#include <picogen/picogen.h>



using namespace picogen;
using geometrics::Vector3d;
using graphics::color::Color;



/// \todo replace orthnormalbasis_from_normal with the version from pbrt (it has no loop inside)
static inline void orthnormalbasis_from_normal (
    param_out (Vector3d,u),
    param_out (Vector3d,v),
    param_in (Vector3d,n)
) {
    do {
        // look for any vector that is not parallel to n
        const Vector3d tmp ( (real) rand() / (real) RAND_MAX, (real) rand() / (real) RAND_MAX, (real) rand() / (real) RAND_MAX);
        u = n.cross (tmp);
    } while (fabs (u*n) > picogen::constants::epsilon);
    u = u.normal();
    v = n.cross (u);
}

static inline real to_radian (real a) {
    return a* (picogen::constants::pi/180.0);
}
static inline real to_degree (real a) {
    return a* (180.0/picogen::constants::pi);
}

static inline real angle2d (real x, real z) {
    if (z<0) {
        return acos (-x);
    }
    return (2*picogen::constants::pi)-acos (-x);
}


static inline real getAngleBetween (real thetav, real phiv, real theta, real phi) {
    real cospsi = sin (thetav) * sin (theta) * cos (phi-phiv) + cos (thetav) * cos (theta);
    if (cospsi > 1) return 0;
    if (cospsi < -1) return picogen::constants::pi;
    return  acos (cospsi);
}

/*
static void Yxy_RGB (real *p_color, real Y, real x, real y) {
    real CIE_X = x * (Y / y);
    real CIE_Y = Y;
    real CIE_Z = (1-x-y) * (Y / y);

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




namespace picogen {
    namespace graphics {
        namespace objects {



            Preetham::Preetham() :
                    m_T (1.8), m_sunSolidAngleFactor (1), m_sunColor (100,100,100), m_colorFilter (1,1,1),
                    m_enableSunFalloffHack (false), m_enableFogHack (false) {
            }



            void Preetham::setSunDirection (param_in (Vector3d,pw)) {
                Vector3d w = -pw.normal();
                m_sunDirection = -w;

                m_sunTheta = acos (m_sunDirection[1]);
                m_sunPhi   = atan2 (m_sunDirection[0],m_sunDirection[2]);
            }



            void Preetham::getSunDirection (param_out (Vector3d,direction)) const {
                direction = m_sunDirection;
            }



            // coming from official Preetham code (http://www.cs.utah.edu/vissim/papers/sunsky/)
            // and thus from IES Lighting Handbook p361
            void Preetham::setSunDirection (
                real latitude,
                real longitude,
                int _standardMeridian,          // standardMeridian , time zone number, east to west and zero based
                int julianDay,                 // julianDay
                real timeOfDay     // timeOfDay
            ) {
                real standardMeridian = (real) _standardMeridian*15.0;

                real solarTime = timeOfDay +
                                 (0.170*sin (4.0*picogen::constants::pi* (julianDay - 80.0) /373.0) - 0.129*sin (2.0*picogen::constants::pi* (julianDay - 8.0) /355.0)) +
                                 (standardMeridian - longitude) /15.0;

                real solarDeclination = (0.4093*sin (2*picogen::constants::pi* (julianDay - 81.0) /368.0));

                real solarAltitude =
                    asin (sin (to_radian (latitude)) * sin (solarDeclination)
                          - cos (to_radian (latitude)) * cos (solarDeclination) * cos (picogen::constants::pi*solarTime/12.0))
                    ;

                real opp, adj;
                opp = -cos (solarDeclination) * sin (picogen::constants::pi*solarTime/12.0);
                adj = - (cos (to_radian (latitude)) * sin (solarDeclination) +
                         sin (to_radian (latitude)) * cos (solarDeclination) * cos (picogen::constants::pi*solarTime/12.0)
                        );
                real solarAzimuth=atan2 (opp,adj);

                m_sunPhi   = (solarAzimuth); // the original implementation takes the negative, but then below polar2cartesian triade is not correct
                m_sunTheta = picogen::constants::pi / 2.0 - solarAltitude;

                m_sunDirection[0] = sin (m_sunPhi) * cos (m_sunTheta);
                m_sunDirection[2] = sin (m_sunPhi) * sin (m_sunTheta);
                m_sunDirection[1] = cos (m_sunPhi);
                m_sunDirection = m_sunDirection.normal();
            }



            void Preetham::setSunColor (Color color) {
                m_sunColor = color;
            }



            void Preetham::getSunColor (param_out (Color,color)) const {
                color = m_sunColor;
            }



            void Preetham::setColorFilter (Color color) {
                m_colorFilter = color;
            }



            void Preetham::setTurbidity (real t) {
                m_T = t;
            }



            void Preetham::setSunSolidAngleFactor (real f) {
                m_sunSolidAngleFactor = f;
            }
            
            
            
            /*void Preetham::setSunFalloffMaxSolidAngleFactor (real f) {
                m_sunFalloffHackMaxSolidAngleFactor = f;
            }*/
            
            
            
            /*void Preetham::setSunFalloffExponent (real e) {
                m_falloffHackExponent = e;
            }*/            
            
            
            
            void Preetham::setSunFalloffHackParameters (real a, real b, real c) {
                m_falloffHackFactor [0] = a;
                m_falloffHackFactor [1] = b;
                m_falloffHackFactor [2] = c;
            }
            
            
            
            void Preetham::enableSunFalloffHack (bool enable) {
                m_enableSunFalloffHack = enable;
            }



            void Preetham::invalidate() {
                const real T2 = m_T*m_T;
                const real theta2 = m_sunTheta*m_sunTheta;
                const real theta3 = theta2*m_sunTheta;

                m_zenith_x =
                    (+0.00165*theta3 - 0.00374*theta2 + 0.00208*m_sunTheta + 0)       * T2 +
                    (-0.02902*theta3 + 0.06377*theta2 - 0.03202*m_sunTheta + 0.00394) * m_T +
                    (+0.11693*theta3 - 0.21196*theta2 + 0.06052*m_sunTheta + 0.25885);

                m_zenith_y =
                    (+0.00275*theta3 - 0.00610*theta2 + 0.00316*m_sunTheta  + 0)       * T2 +
                    (-0.04214*theta3 + 0.08970*theta2 - 0.04153*m_sunTheta  + 0.00515) * m_T +
                    (+0.15346*theta3 - 0.26756*theta2 + 0.06669*m_sunTheta  + 0.26688);

                const real X = (4.0/9.0-m_T/120.0) * (picogen::constants::pi-2*m_sunTheta);
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



            real Preetham::perez (real Theta, real gamma, const real p[]) {
                return (1.0 + p[0]*exp (p[1]/cos (Theta))) * (1.0 + p[2]*exp (p[3]*gamma) + p[4]*pow (cos (gamma),2.0));
            }



            void Preetham::shade (param_out (Color,color), param_in (Ray,in_ray)) const {
                real /*rho,*/ theta, phi;

                Ray ray = in_ray;

                if (ray.w() [1]<0.001) {
                    //color.from_rgb(1,0,0); return;
                    ray.w() [1] = 0.001;
                    ray.w() = ray.w().normal();
                }
                theta = acos (ray.w() [1]);
                if (fabs (theta) < 1e-5) phi = 0;
                else  phi = atan2 (ray.w() [2],ray.w() [0]);

                const real gamma = acos (ray.w() * m_sunDirection);//*/getAngleBetween( theta, phi, m_sunTheta, m_sunPhi );
                const real x = m_zenith_x * (perez (theta, gamma, m_perez_x) / perez (0.0, m_sunTheta, m_perez_x));
                const real y = m_zenith_y * (perez (theta, gamma, m_perez_y) / perez (0.0, m_sunTheta, m_perez_y));
                const real Y = m_zenith_Y * (perez (theta, gamma, m_perez_Y) / perez (0.0, m_sunTheta, m_perez_Y));


                /*real c[3];
                Yxy_RGB (c, Y, x, y);
                color.from_rgb (c[0],c[1],c[2]);*/
                color.from_Yxy (Y, x, y);
                color *= m_colorFilter;
            }



            void Preetham::sunShade (param_out (Color,color), param_in (Ray,ray)) const {
                color = Color (0.0,0.0,0.0);
                const real gamma = acos (ray.w() * m_sunDirection);//*/getAngleBetween( theta, phi, m_theta, m_phi );
                
                if (gamma < m_sunSolidAngle) {
                    color += m_sunColor;
                } else if (m_enableSunFalloffHack) {
                    
                    /*
                    real f = 1.0 - (gamma-m_sunSolidAngle) / (m_sunFalloffHackMaxSolidAngle - m_sunSolidAngle);
                    if (f > 0.0 && f < 1.0) {
                        const real i = f * picogen::constants::pi * 0.5;//1.0 / exp (pow (m_sunFalloffHackMaxSolidAngle * (gamma-m_sunSolidAngle), 2.0));
                        color += m_sunColor * i;
                    }
                    */
                    
                    real f = gamma-m_sunSolidAngle;
                    const real i_ = (1/exp(m_falloffHackFactor [0] * f))+m_falloffHackFactor [1]*(1/exp(m_falloffHackFactor [2]*f));
                    const real i = i_ < 0.0 ? 0.0 : i_ > 1.0 ? 1.0 : i_;
                    color += m_sunColor * i;
                    
                    /*real f = 1.0 - (gamma-m_sunSolidAngle) / ( (m_sunSolidAngle+0.5)-m_sunSolidAngle);
                    //real f = 1.0 - ( gamma-alpha );// / (180.0*constants::pi);
                    */
                    /*if (f > 0.0 && f < 1.0) {
                        //color += m_sunColor * pow (f, m_falloffHackExponent);//+10*ray.w() [1]);
                        const real i = 1.0 / exp ( pow (f*0.33, 2.0));
                        color += m_sunColor * i;
                    }*/
                    //const real i = 1.0 / exp (pow (m_sunFalloffHackMaxSolidAngle * (gamma-m_sunSolidAngle), 2.0));
                    //color += m_sunColor * i;
                }
            }



            void Preetham::sunSample (
                param_out (Color,color),
                param_out (Ray,ray),
                param_out (real,p),
                param_in (Vector3d,position)
            ) const {
                //> we will use the disk sampler from "Monte Carlo Techniques
                //> for Direct Lighting Calculations" (Shirley,Wang,Zimmerman)
                /// \todo: modify so that the solid angle is projected onto the unit sphere accurately
                const Vector3d c = m_sunDirection;
                const real     r = m_sunSolidAngle;
                const Vector3d n = -m_sunDirection;
                const real    e1 = (real) rand() / (real) RAND_MAX;
                const real    e2 = (real) rand() / (real) RAND_MAX;
                Vector3d u,v;
                orthnormalbasis_from_normal (u,v,n);
                const Vector3d R (
                    r*sqrt (e1) *cos (2.0*picogen::constants::pi*e2),
                    r*sqrt (e1) *sin (2.0*picogen::constants::pi*e2),
                    0.0
                );
                ray.w() = Vector3d (
                              c[0] + u[0]*R[0] + v[0]*R[1] + n[0]*R[2],
                              c[1] + u[1]*R[0] + v[1]*R[1] + n[1]*R[2],
                              c[2] + u[2]*R[0] + v[2]*R[1] + n[2]*R[2]
                          );
                ray.x() = position;
                p = 1.0;

                color = m_sunColor * (m_sunSolidAngle/ (4.0*picogen::constants::pi));
                /*
                //> simple, directional sampler
                color = m_sunColor * (m_sunSolidAngle/(4.0*picogen::constants::pi));
                ray.w() = m_sunDirection;
                ray.x() = position;
                p = 1.0;
                */
            }



            real Preetham::getSunArealFactor () const {
                return (m_sunSolidAngle/ (4.0*picogen::constants::pi));
            }



            void Preetham::atmosphereShade (param_out (Color,color), param_in (Color,src_color), param_in (Ray,ray), real distance) const {
                if (m_enableFogHack) {
                    Color sky_color;
                    shade (sky_color, ray);
                    const real distance_ = distance>m_fogHackSatDist?m_fogHackSatDist:distance;
                    //const real d = 1.0 / (1+m_fogHackFactor*distance_); // 0.00082 was a cool value for '2097'
                    const real d = 1.0 / exp (pow (m_fogHackFactor*distance_, 1));
                    color = src_color*d + sky_color* (1-d);
                } else {
                    color = src_color;
                }
            }



            void Preetham::enableFogHack (bool enable, real f, real satDist) {
                m_enableFogHack = enable;
                m_fogHackFactor = f;
                m_fogHackSatDist = satDist;
            }



        }; // namespace objects
    }; // namespace graphics
}; // namespace picogen
