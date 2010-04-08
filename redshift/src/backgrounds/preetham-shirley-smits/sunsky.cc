//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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


// -*- C++ -*-
/* Copyright 1999
 * Wed Apr 14 10:07:50 1999  Brian Smits  (bes@phoenix.cs.utah.edu)
 *
 * PssSunSky.C
 *
 *
 *
 * $Id: PssSunSky.C,v 1.2 1999/07/15 00:07:34 bes Exp $
 *
 */

/**********************************************************
//  TODO
// Fix Indexing (make it zero based, not 1 based...)
// ********************************************************/

/*#ifndef RICOMMON_H
#include <RiCommon.H>
#endif

#ifndef PssSunSky_H
#include <PssSunSky.H>
#endif

#ifndef RICOLORXYZV_H
#include <RiColorXYZV.H>
#endif

#ifndef RISPECTRALCURVE_H
#include <RiSpectralCurve.H>
#endif*/

#include "../../../include/backgrounds/preetham-shirley-smits/sunsky.hh"

#include <iostream>
#include <cmath>


namespace redshift { namespace background {
/**********************************************************
// Atmospheric Perspective Constants
//
// ********************************************************/


// density decreses as exp(-alpha*h) alpha in m^-1.
// _1 refers to Haze, _2 referes to molecules.

static PssSunSky::real_t const Alpha_1 = 0.83331810215394e-3;
static PssSunSky::real_t const Alpha_2 = 0.11360016092149e-3;

#define Nt 20 // Number of bins for theta
#define Np 20 // Number of bins for phi

#define radians(x) ((x)/180.0*constants::pi)

static PssSunSky::Spectrum AT0_1[Nt+1][Np+1];
static PssSunSky::Spectrum AT0_2[Nt+1][Np+1];



/* All angles in radians, theta angles measured from normal */
inline PssSunSky::real_t RiAngleBetween(
        PssSunSky::real_t thetav,
        PssSunSky::real_t phiv,
        PssSunSky::real_t theta,
        PssSunSky::real_t phi)
{
        using std::sin;
        using std::cos;
        using std::acos;
        real_t cospsi = sin(thetav) * sin(theta) * cos(phi-phiv) + cos(thetav) * cos(theta);
        if (cospsi > 1) return 0;
        if (cospsi < -1) return M_PI;
        return  acos(cospsi);
}




PssSunSky::PssSunSky(
        real_t lat,
        real_t longi,
        int sm,       // standardMeridian
        int jd,       // julianDay
        real_t tOfDay,   // timeOfDay
        real_t turb,
        real_t overcast,
        bool initAtmEffects // turbidity
) : turbidity (turb), overcast (overcast)
{
        /*latitude = lat;
        longitude = longi;
        julianDay = jd;
        timeOfDay = tOfDay;
        standardMeridian = sm * 15.0;  // sm is actually timezone number (east to west, zero based...)
        */

        InitSunThetaPhi(lat, longi, jd, tOfDay, sm * 15.0);
        InitRest(initAtmEffects);
}

PssSunSky::PssSunSky(
        const Vector &direction,
        real_t turbidity,
        real_t overcast,
        bool initAtmEffects
) : turbidity (turbidity), overcast (overcast)
{
        /*latitude = lat;
        longitude = longi;
        julianDay = jd;
        timeOfDay = tOfDay;
        standardMeridian = sm * 15.0;  // sm is actually timezone number (east to west, zero based...)
        */

        InitSunThetaPhi(direction);
        InitRest(initAtmEffects);
}

void PssSunSky::InitRest (bool initAtmEffects) {
        using std::tan;
        using std::sin;
        using std::cos;

        V = 4.0; // Junge's exponent.

        toSun = Vector(cos(phiS)*sin(thetaS), cos(thetaS), sin(phiS)*sin(thetaS));

        sunSpectralRad =  (1-overcast)*ComputeAttenuatedSunlight(thetaS, turbidity);

        sunSolidAngle =  0.25*constants::pi*1.39*1.39/(150*150);  // = 6.7443e-05

        const real_t
                theta2 = thetaS*thetaS,
                theta3 = theta2*thetaS,
                T = turbidity,
                T2 = turbidity*turbidity
        ;

        const real_t chi = (4.0/9.0 - T / 120.0) * (constants::pi - 2 * thetaS);
        zenith_Y = (4.0453 * T - 4.9710) * tan(chi) - .2155 * T + 2.4192;
        zenith_Y *= 1000;  // conversion from kcd/m^2 to cd/m^2

        zenith_x =
                (+0.00165*theta3 - 0.00374*theta2 + 0.00208*thetaS + 0)          * T2 +
                (-0.02902*theta3 + 0.06377*theta2 - 0.03202*thetaS + 0.00394) * T +
                (+0.11693*theta3 - 0.21196*theta2 + 0.06052*thetaS + 0.25885);

        zenith_y =
                (+0.00275*theta3 - 0.00610*theta2 + 0.00316*thetaS  + 0)          * T2 +
                (-0.04214*theta3 + 0.08970*theta2 - 0.04153*thetaS  + 0.00515) * T +
                (+0.15346*theta3 - 0.26756*theta2 + 0.06669*thetaS  + 0.26688);

        perez_Y[1] =    0.17872 *T  - 1.46303;
        perez_Y[2] =   -0.35540 *T  + 0.42749;
        perez_Y[3] =   -0.02266 *T  + 5.32505;
        perez_Y[4] =    0.12064 *T  - 2.57705;
        perez_Y[5] =   -0.06696 *T  + 0.37027;

        perez_x[1] =   -0.01925 *T  - 0.25922;
        perez_x[2] =   -0.06651 *T  + 0.00081;
        perez_x[3] =   -0.00041 *T  + 0.21247;
        perez_x[4] =   -0.06409 *T  - 0.89887;
        perez_x[5] =   -0.00325 *T  + 0.04517;

        perez_y[1] =   -0.01669 *T  - 0.26078;
        perez_y[2] =   -0.09495 *T  + 0.00921;
        perez_y[3] =   -0.00792 *T  + 0.21023;
        perez_y[4] =   -0.04405 *T  - 1.65369;
        perez_y[5] =   -0.01092 *T  + 0.05291;

        if (initAtmEffects) {
                // Atmospheric Perspective initialization
                CreateConstants();
                InitA0();
        }
        atmInited = initAtmEffects;
}

/**********************************************************
// South = x,  East = y, up = z
// All times in decimal form (6.25 = 6:15 AM)
// All angles in Radians
// ********************************************************/

void PssSunSky::SunThetaPhi(real_t &stheta, real_t &sphi) const
{
        sphi = phiS;
        stheta = thetaS;
}

/**********************************************************
// South = x,  East = y, up = z
// All times in decimal form (6.25 = 6:15 AM)
// All angles in Radians
// From IES Lighting Handbook pg 361
// ********************************************************/

void PssSunSky::InitSunThetaPhi(
        const real_t latitude,
        const real_t longitude,
        const int    julianDay,
        const real_t timeOfDay,
        const int standardMeridian
) {
        const real_t
                solarTime = timeOfDay +
                           (0.170*sin(4*constants::pi*(julianDay - 80)/373) - 0.129*sin(2*constants::pi*(julianDay - 8)/355)) +
                           (standardMeridian - longitude)/15.0,
                solarDeclination = (0.4093*sin(2*constants::pi*(julianDay - 81)/368)),
                solarAltitude = asin(sin(radians(latitude)) * sin(solarDeclination) -
                                   cos(radians(latitude)) * cos(solarDeclination) * cos(constants::pi*solarTime/12)),
                opp = -cos(solarDeclination) * sin(constants::pi*solarTime/12),
                adj = -(cos(radians(latitude)) * sin(solarDeclination) +
                        sin(radians(latitude)) * cos(solarDeclination) *  cos(constants::pi*solarTime/12)),
                solarAzimuth = atan2(opp,adj)
        ;

        phiS = -solarAzimuth;
        thetaS = constants::pi / 2.0 - solarAltitude;
}

void PssSunSky::InitSunThetaPhi(Vector const &sunDirection) {
        using std::atan2; using std::acos;
        thetaS = acos (sunDirection.up() / length (sunDirection));
        phiS = atan2 (sunDirection.ahead(), sunDirection.right());
}

Vector PssSunSky::GetSunPosition() const
{
        return toSun;
}

/**********************************************************
// Solar Radiance
//
// ********************************************************/

PssSunSky::Spectrum PssSunSky::GetSunSpectralRadiance() const
{
        return sunSpectralRad;
}

PssSunSky::real_t PssSunSky::GetSunSolidAngle() const
{
        return sunSolidAngle;
}

/**********************************************************
// Sky Radiance
//
// ********************************************************/

PssSunSky::Spectrum PssSunSky::GetSkySpectralRadiance(const Vector &varg) const
{
        real_t theta, phi;
        Vector v = normalize(varg);
        if (v.up() < 0) return Spectrum(0.000000);
        if (v.up() < 0.001)
                v = normalize(Vector(v.right(),0.001, v.ahead()));

        theta = acos(v.up());
        if (fabs(theta)< 1e-5) phi = 0;
        else  phi = atan2(v.ahead(),v.right());
        return GetSkySpectralRadiance(theta,phi);
}



inline PssSunSky::real_t PssSunSky::PerezFunction(
        const PssSunSky::real_t *lam,
        PssSunSky::real_t theta,
        PssSunSky::real_t gamma,
        PssSunSky::real_t lvz
) const
{
        using std::exp;
        using std::sin;
        using std::cos;
        PssSunSky::real_t den = ((1 + lam[1]*exp(lam[2])) *
                      (1 + lam[3]*exp(lam[4]*thetaS) + lam[5]*cos(thetaS)*cos(thetaS)));

        PssSunSky::real_t num = ((1 + lam[1]*exp(lam[2]/cos(theta))) *
                      (1 + lam[3]*exp(lam[4]*gamma)  + lam[5]*cos(gamma)*cos(gamma)));

        return lvz* num/den;
}



PssSunSky::Spectrum
PssSunSky::overcastSkySpectralRadiance(PssSunSky::real_t theta,
                                       PssSunSky::real_t /*phi*/
) const {
        /*const real_t gamma = RiAngleBetween(theta,phi,thetaS,phiS);
        const real_t Y = PerezFunction(perez_Y, theta, gamma, zenith_Y);*/
        const real_t o = zenith_Y*((1+2.*cos(theta))/3);
        //return Spectrum(o);//::FromRGB(o,o,o,IlluminantSpectrum);
        return Spectrum::FromRGB(o,o,o,IlluminantSpectrum);
}



PssSunSky::Spectrum
PssSunSky::clearSkySpectralRadiance (PssSunSky::real_t theta,
                                     PssSunSky::real_t phi
) const {
        const real_t gamma = RiAngleBetween(theta,phi,thetaS,phiS);
        // Compute xyY values
        const real_t x = PerezFunction(perez_x, theta, gamma, zenith_x);
        const real_t y = PerezFunction(perez_y, theta, gamma, zenith_y);
        const real_t Y = PerezFunction(perez_Y, theta, gamma, zenith_Y);
        const Spectrum spect = ChromaticityToSpectrum(x,y);
        // A simple luminance function would be more efficient.
        const Spectrum ret = Y * spect / spect.y();
        return ret;
}



PssSunSky::Spectrum
PssSunSky::GetSkySpectralRadiance(
        PssSunSky::real_t theta,
        PssSunSky::real_t phi) const
{
        if (overcast >= 0.9999) {
                return overcastSkySpectralRadiance(theta, phi);
        } else if (overcast <= 0.0001) {
                return clearSkySpectralRadiance(theta, phi);
        } else {
                return clearSkySpectralRadiance(theta,phi) * (1-overcast)
                      + overcastSkySpectralRadiance(theta,phi) * overcast;
        }
}



/**********************************************************
// Atmospheric perspective functions
//
// ********************************************************/


/**********************************************************
// Initialization
//
// ********************************************************/





void PssSunSky::CalculateA0(
        PssSunSky::real_t thetav,
        PssSunSky::real_t phiv,
        PssSunSky::Spectrum &A0_1,
        PssSunSky::Spectrum &A0_2
) const {
        using std::sin;
        using std::cos;
        real_t psi;
        Spectrum skyRad;
        Spectrum beta_ang_1, beta_ang_2;
        real_t theta, phi, phiDelta = M_PI/20;
        real_t thetaDelta = (M_PI/2)/20;
        real_t thetaUpper;

        Spectrum skyAmb_1 (Spectrum::real_t(0));
        Spectrum skyAmb_2 (Spectrum::real_t(0));

        thetaUpper = M_PI / 2;

        for (theta = 0; theta < thetaUpper; theta += thetaDelta)
                for (phi = 0; phi < 2 * M_PI; phi += phiDelta)  {
                        skyRad = GetSkySpectralRadiance(theta, phi);
                        psi = RiAngleBetween(thetav, phiv, theta, phi);

                        beta_ang_1 = beta_p_ang_prefix * GetNeta(psi, V);
                        beta_ang_2 = beta_m_ang_prefix * Spectrum::real_t (
                                             1+0.9324*cos(psi)*cos(psi));

                        skyAmb_1 += skyRad * beta_ang_1 * sin(theta) * thetaDelta * phiDelta;
                        skyAmb_2 += skyRad * beta_ang_2 * sin(theta) * thetaDelta * phiDelta;
                }

        /* Sun's ambience term*/

        psi = RiAngleBetween(thetav, phiv, thetaS, phiS);

        beta_ang_1 = beta_p_ang_prefix * GetNeta(psi, V);
        beta_ang_2 = beta_m_ang_prefix * Spectrum::real_t(1+0.9324*cos(psi)*cos(psi));

        Spectrum sunAmb_1 = sunSpectralRad * beta_ang_1 * sunSolidAngle;
        Spectrum sunAmb_2 = sunSpectralRad * beta_ang_2 * sunSolidAngle;

        // Sum of sun and sky  (should probably add a ground ambient)
        A0_1 =  (sunAmb_1 + skyAmb_1);
        A0_2 =  (sunAmb_2 + skyAmb_2);
}



void  PssSunSky::InitA0() const {
        // This loop was buggy in the original code, causing the last entries to be black. /phresnel

        /* // original code
        int i, j;
        real_t theta, phi;

        real_t delTheta = M_PI/Nt;
        real_t delPhi = 2*M_PI/Np;

        std::cerr << "ggPssSunSky::Preprocessing: 0%\r";
        for (i=0,theta = 0; theta<=  M_PI; theta+= delTheta,i++) {
            for (j=0,phi=0; phi <= 2*M_PI; phi+= delPhi,j++)
              CalculateA0(theta, phi,  AT0_1[i][j], AT0_2[i][j]);
            std::cerr << "ggPssSunSky::Preprocessing: " << 100*(i*Np+j)/((Nt+1)*Np) <<"%  \r";
        }
        std::cerr << "ggPssSunSky::Preprocessing:  100%   " << std::endl;
        */
        for (int i=0; i<Nt+1; ++i) {
                const real_t theta = (real_t(i)/(Nt)) * M_PI;
                for (int j=0; j<Np+1; ++j) {
                        const real_t phi   = (real_t(j)/(Np)) * 2.f*M_PI;
                        CalculateA0(theta, phi,  AT0_1[i][j], AT0_2[i][j]);
                }
        }
}



/**********************************************************
// Evaluation
//
// ********************************************************/

void PssSunSky::GetAtmosphericEffects(const Vector &viewer, const Vector &source_,
                Spectrum &attenuation, Spectrum &inscatter ) const
{
        assert(atmInited);
        // Clean up the 1000 problem
        const real_t h0 = viewer.up();//1000 added to make sure ray doesnt
        //go below zero.

        const Vector source = source_;
        const Vector direction = normalize (source - viewer);
        const real_t thetav = acos(direction.up());
        const real_t phiv = atan2(direction.ahead(),direction.right());
        const real_t s = length(viewer - source);

        // fix added (phresnel)/ would otherwise result in fault mem access if s==0
        if (s<=0) {
                attenuation = Spectrum(Spectrum::real_t(1));
                inscatter = Spectrum(Spectrum::real_t(0));
                return;
        }

        // This should be changed so that we don't need to worry about it.
        // phresnel: Test removed until further notice, app does not seem to worry

        /*if (h0+s*cos(thetav) <= 0) {
                attenuation = Spectrum(Spectrum::real_t(1));
                inscatter = Spectrum(Spectrum::real_t(0));
                std::cerr << "Warning: Ray going below earth's surface\n";
                return;
        }*/

        attenuation = AttenuationFactor(h0, thetav, s);
        inscatter   = InscatteredRadiance(h0, thetav, phiv, s);
}



inline real_t EvalFunc(real_t B, real_t x)
{
        using std::fabs;
        using std::exp;
        if (fabs(B*x)< 0.01) return x;
        return (1-exp(-B*x))/B;
}



PssSunSky::Spectrum
PssSunSky::clearAttenuationFactor(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t s
) const {
        using std::cos;
        using std::exp;
        real_t costheta = cos(theta);
        real_t B_1 = Alpha_1 * costheta;
        real_t B_2 = Alpha_2 * costheta;
        real_t constTerm_1 = exp(-Alpha_1*h0) * EvalFunc(B_1, s);
        real_t constTerm_2 = exp(-Alpha_2*h0) * EvalFunc(B_2, s);

        return (exp(-beta_p * constTerm_1) *
                exp(-beta_m * constTerm_2));
}

PssSunSky::Spectrum
PssSunSky::overcastAttenuationFactor(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t s
) const {
        const double atty = clearAttenuationFactor(h0, theta, s).y();
        //return Spectrum(atty);
        return Spectrum::FromRGB(atty, atty, atty, IlluminantSpectrum);
}

PssSunSky::Spectrum
PssSunSky::AttenuationFactor(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t s
) const {
        if (overcast < 0.0001) {
                return clearAttenuationFactor(h0, theta, s);
        } else if (overcast > 0.9999) {
                return overcastAttenuationFactor(h0, theta, s);
        } else {
                // TODO: clearAttenuationFactor is called twice below,
                //       make an overload of overcastAtt.. which accepts a
                //       spectrum
                return (1-overcast)*clearAttenuationFactor(h0, theta, s)
                        + overcast*overcastAttenuationFactor(h0, theta, s);
        }
}



static void GetA0fromTable(
        PssSunSky::real_t theta,
        PssSunSky::real_t phi,
        PssSunSky::Spectrum &A0_1,
        PssSunSky::Spectrum &A0_2
) {
        PssSunSky::real_t eps = 1e-4;
        if (phi < 0) phi += 2*M_PI; // convert phi from -pi..pi to 0..2pi
        theta = theta*Nt/M_PI - eps;
        phi = phi*Np/(2*M_PI) - eps;
        if (theta < 0) theta = 0;
        if (phi < 0) phi = 0;
        int i = (int) theta;
        PssSunSky::real_t u = theta - i;
        int j = (int)phi;
        PssSunSky::real_t v = phi - j;

        A0_1 = (1-u)*(1-v)*AT0_1[i][j] + u*(1-v)*AT0_1[i+1][j]
               + (1-u)*v*AT0_1[i][j+1] + u*v*AT0_1[i+1][j+1];
        A0_2 = (1-u)*(1-v)*AT0_2[i][j] + u*(1-v)*AT0_2[i+1][j]
               + (1-u)*v*AT0_2[i][j+1] + u*v*AT0_2[i+1][j+1];
}



inline PssSunSky::real_t RiHelper1(
        PssSunSky::real_t A,
        PssSunSky::real_t B,
        PssSunSky::real_t C,
        PssSunSky::real_t D,

        PssSunSky::real_t H,
        PssSunSky::real_t K,
        PssSunSky::real_t u
) {
        PssSunSky::real_t t = std::exp(-K*(H-u));
        return (t/K)*((A*u*u*u + B*u*u + C*u +D) -
                      (3*A*u*u + 2*B*u + C)/K +
                      (6*A*u + 2*B)/(K*K) -
                      (6*A)/(K*K*K));
}



inline void RiCalculateABCD(
        PssSunSky::real_t a,
        PssSunSky::real_t b,
        PssSunSky::real_t c,
        PssSunSky::real_t d,
        PssSunSky::real_t e,

        PssSunSky::real_t den,
        PssSunSky::real_t &A,
        PssSunSky::real_t &B,
        PssSunSky::real_t &C,
        PssSunSky::real_t &D)
{
        A = (-b*d -2 + 2*c + a*e - b*e + a*d)/den;
        B = -(2*a*a*e + a*a*d - 3*a - a*b*e +
              3*a*c + a*b*d - 2*b*b*d - 3*b - b*b*e + 3*b*c)/den;
        C =( -b*b*b*d - 2*b*b*a*e -   b*b*a*d + a*a*b*e +
             2*a*a*b*d - 6*a*b     + 6*b*a*c   + a*a*a*e)/den;
        D = -(   b*b*b - b*b*b*a*d - b*b*a*a*e + b*b*a*a*d
                 -3*a*b*b + b*e*a*a*a - c*a*a*a + 3*c*b*a*a)/den;
}



PssSunSky::Spectrum PssSunSky::clearInscatteredRadiance(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t phi,
        PssSunSky::real_t s
) const {
        using std::exp;
        using std::cos;
        real_t costheta = cos(theta);
        real_t B_1 = Alpha_1*costheta;
        real_t B_2 = Alpha_2*costheta;
        Spectrum A0_1;
        Spectrum A0_2;
        Spectrum result;
        int i;
        Spectrum I_1(Spectrum::noinit), I_2(Spectrum::noinit);

        GetA0fromTable(theta, phi, A0_1, A0_2);

        // approximation (< 0.3 for 1% accuracy)
        if (fabs(B_1*s) < 0.3) {
                real_t constTerm1 =  std::exp(-Alpha_1*h0);
                real_t constTerm2 =  std::exp(-Alpha_2*h0);

                Spectrum C_1 = beta_p * constTerm1;
                Spectrum C_2 = beta_m * constTerm2;

                for (int i = 0; i < Spectrum::num_components; i++) {
                        I_1[i] =  (1-exp(-(B_1 + C_1[i] + C_2[i]) * s)) / (B_1 + C_1[i] + C_2[i]);
                        I_2[i] =  (1-exp(-(B_2 + C_1[i] + C_2[i]) * s)) / (B_2 + C_1[i] + C_2[i]);
                }

                return A0_1 * constTerm1 * I_1 + A0_2 * constTerm2 * I_2;
        }

        // Analytical approximation
        real_t A,B,C,D,H1,H2,K;
        real_t u_f1, u_i1,u_f2, u_i2, int_f, int_i, fs, fdashs, fdash0;
        real_t a1,b1,a2,b2;
        real_t den1, den2;

        b1 = u_f1 = exp(-Alpha_1*(h0+s*costheta));
        H1 = a1 = u_i1 = exp(-Alpha_1*h0);
        b2 = u_f2 = exp(-Alpha_2*(h0+s*costheta));
        H2 = a2 = u_i2 = exp(-Alpha_2*h0);
        den1 = (a1-b1)*(a1-b1)*(a1-b1);
        den2 = (a2-b2)*(a2-b2)*(a2-b2);

        for (i = 0; i < Spectrum::num_components; i++) {
                // for integral 1
                K = beta_p[i]/(Alpha_1*costheta);
                fdash0 = -beta_m[i]*H2;
                fs = exp(-beta_m[i]/(Alpha_2*costheta)*(u_i2 - u_f2));
                fdashs = -fs*beta_m[i]*u_f2;

                RiCalculateABCD(a1,b1,fs,fdash0,fdashs,den1,A,B,C,D);
                int_f = RiHelper1(A,B,C,D,H1,K, u_f1);
                int_i = RiHelper1(A,B,C,D,H1,K, u_i1);
                I_1[i] = (int_f - int_i)/(-Alpha_1*costheta);

                // for integral 2
                K = beta_m[i]/(Alpha_2*costheta);
                fdash0 = -beta_p[i]*H1;
                fs = exp(-beta_p[i]/(Alpha_1*costheta)*(u_i1 - u_f1));
                fdashs = -fs*beta_p[i]*u_f1;

                RiCalculateABCD(a2,b2,fs,fdash0, fdashs, den2, A,B,C,D);
                int_f = RiHelper1(A,B,C,D,H2,K, u_f2);
                int_i = RiHelper1(A,B,C,D,H2,K, u_i2);
                I_2[i] = (int_f - int_i)/(-Alpha_2*costheta);

        }

        /*const Spectrum molecular = A0_2 * I_2;
        std::cout << molecular.y() << "\n";
        static int p;
        if (++p > 64*64) exit(0);
        return Spectrum(I_2.y());
        if (I_2.y()<0)
                return Spectrum::FromRGB(color::RGB(0,0,0));*/

        return A0_1 * I_1
                + A0_2 * I_2;
}

PssSunSky::Spectrum PssSunSky::overcastInscatteredRadiance(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t phi,
        PssSunSky::real_t s
) const {
        const real_t y = clearInscatteredRadiance(h0, theta, phi, s).y();
        //return Spectrum(y);//::FromRGB(iny, iny, iny, IlluminantSpectrum);
        return Spectrum::FromRGB(y, y, y, IlluminantSpectrum);
}

PssSunSky::Spectrum PssSunSky::InscatteredRadiance(
        PssSunSky::real_t h0,
        PssSunSky::real_t theta,
        PssSunSky::real_t phi,
        PssSunSky::real_t s
) const {
        if (overcast < 0.0001) {
                return clearInscatteredRadiance(h0, theta, phi, s);
        } else if (overcast > 0.9999) {
                return overcastInscatteredRadiance(h0, theta, phi, s);
        } else {
                // TODO: clearAttenuationFactor is called twice below,
                //       make an overload of overcastAtt.. which accepts a
                //       spectrum
                return (1-overcast)*clearInscatteredRadiance(h0, theta, phi, s)
                        + overcast*overcastInscatteredRadiance(h0, theta, phi, s);
        }
}

} }
