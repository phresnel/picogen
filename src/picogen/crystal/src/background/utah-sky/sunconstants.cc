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
 * Thu Apr 15 15:17:17 1999  Brian Smits  (bes@phoenix.cs.utah.edu)
 *
 * RiSunConstants.C
 *
 *
 *
 * $Id: RiSunConstants.C,v 1.2 1999/07/15 00:07:33 bes Exp $
 *
 */
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

#include "sunsky.hh"
#include "geometry.hh"
#include "constants.hh"

namespace crystal { namespace redshift { namespace background {

/* All data lifted from MI */
/* Units are either [] or cm^-1. refer when in doubt MI */

// k_o Spectrum table from pg 127, MI.
static PssSunSky::real_t k_oWavelengths[64] = {
300, 305, 310, 315, 320,
325, 330, 335, 340, 345,
350, 355,

445, 450, 455, 460, 465,
470, 475, 480, 485, 490,
495,

500, 505, 510, 515, 520,
525, 530, 535, 540, 545,
550, 555, 560, 565, 570,
575, 580, 585, 590, 595,

600, 605, 610, 620, 630,
640, 650, 660, 670, 680,
690,

700, 710, 720, 730, 740,
750, 760, 770, 780, 790,
};




static PssSunSky::real_t k_oAmplitudes[65] = {
  10.0,
  4.8,
  2.7,
  1.35,
  .8,
  .380,
  .160,
  .075,
  .04,
  .019,
  .007,
  .0,

  .003,
  .003,
  .004,
  .006,
  .008,
  .009,
  .012,
  .014,
  .017,
  .021,
  .025,

  .03,
  .035,
  .04,
  .045,
  .048,
  .057,
  .063,
  .07,
  .075,
  .08,
  .085,
  .095,
  .103,
  .110,
  .12,
  .122,
  .12,
  .118,
  .115,
  .12,

  .125,
  .130,
  .12,
  .105,
  .09,
  .079,
  .067,
  .057,
  .048,
  .036,
  .028,

  .023,
  .018,
  .014,
  .011,
  .010,
  .009,
  .007,
  .004,
  .0,
  .0
};


// k_g Spectrum table from pg 130, MI.
static PssSunSky::real_t k_gWavelengths[4] = {
  759,
  760,
  770,
  771
};

static PssSunSky::real_t k_gAmplitudes[4] = {
  0,
  3.0,
  0.210,
  0
};

// k_wa Spectrum table from pg 130, MI.
static PssSunSky::real_t k_waWavelengths[13] = {
  689,
  690,
  700,
  710,
  720,
  730,
  740,
  750,
  760,
  770,
  780,
  790,
  800
};

static PssSunSky::real_t k_waAmplitudes[13] = {
  0,
  0.160e-1,
  0.240e-1,
  0.125e-1,
  0.100e+1,
  0.870,
  0.610e-1,
  0.100e-2,
  0.100e-4,
  0.100e-4,
  0.600e-3,
  0.175e-1,
  0.360e-1
};


// 380-750 by 10nm
static PssSunSky::real_t solAmplitudes[38] = {
    165.5, 162.3, 211.2, 258.8, 258.2,
    242.3, 267.6, 296.6, 305.4, 300.6,
    306.6, 288.3, 287.1, 278.2, 271.0,
    272.3, 263.6, 255.0, 250.6, 253.1,
    253.5, 251.3, 246.3, 241.7, 236.8,
    232.1, 228.2, 223.4, 219.7, 215.3,
    211.0, 207.3, 202.4, 198.7, 194.3,
    190.7, 186.3, 182.6
};



/**********************************************************
// Sunlight Transmittance Functions
//
// ********************************************************/

/* Most units not in SI system - For units, refer MI */
PssSunSky::Spectrum
PssSunSky::ComputeAttenuatedSunlight(
        PssSunSky::real_t theta,
        real_t turbidity)
{
        using std::cos;
        using std::pow;
        using std::exp;

    /*RiIrregularSpectralCurve k_oCurve(k_oAmplitudes, k_oWavelengths, 64);
    RiIrregularSpectralCurve k_gCurve(k_gAmplitudes, k_gWavelengths, 4);
    RiIrregularSpectralCurve k_waCurve(k_waAmplitudes, k_waWavelengths, 13);
    RiRegularSpectralCurve   solCurve(solAmplitudes, 380, 750, 38);  // every 10 nm  IN WRONG UNITS
                                                                     // Need a factor of 100 (done below)
    */


    Spectrum k_oCurve = Spectrum::FromSampled (k_oAmplitudes, k_oWavelengths, 64);
    Spectrum k_gCurve = Spectrum::FromSampled (k_gAmplitudes, k_gWavelengths , 4);
    Spectrum k_waCurve = Spectrum::FromSampled (k_waAmplitudes, k_waWavelengths, 13);
    Spectrum solCurve = Spectrum::FromSampled(solAmplitudes, 380, 750, 38);  // every 10 nm  IN WRONG UNITS
                                                                     // Need a factor of 100 (done below)
    real_t  data[91];  // (800 - 350) / 5  + 1

    real_t beta = 0.04608365822050 * turbidity - 0.04586025928522;
    real_t tauR, tauA, tauO, tauG, tauWA;

    real_t m = 1.0/(cos(theta) + 0.15*pow(93.885-theta/constants::pi*180.0,-1.253));  // Relative Optical Mass
                                // equivalent
//    real_t m = 1.0/(cos(theta) + 0.000940 * pow(1.6386 - theta,-1.253));  // Relative Optical Mass

    int i;
    real_t lambda;
    for(i = 0, lambda = 350; i < 91; i++, lambda+=5) {
                                // Rayleigh Scattering
                                // Results agree with the graph (pg 115, MI) */
        tauR = exp( -m * 0.008735 * pow(lambda/1000, real_t(-4.08)));

                                // Aerosal (water + dust) attenuation
                                // beta - amount of aerosols present
                                // alpha - ratio of small to large particle sizes. (0:4,usually 1.3)
                                // Results agree with the graph (pg 121, MI)
        const real_t alpha = 1.3;
        tauA = exp(-m * beta * pow(lambda/1000, -alpha));  // lambda should be in um

                                // Attenuation due to ozone absorption
                                // lOzone - amount of ozone in cm(NTP)
                                // Results agree with the graph (pg 128, MI)
        const real_t lOzone = .35;
        tauO = exp(-m * k_oCurve.at(lambda) * lOzone);

                                // Attenuation due to mixed gases absorption
                                // Results agree with the graph (pg 131, MI)
        tauG = exp(-1.41 * k_gCurve.at(lambda) * m / pow(1 + 118.93 * k_gCurve.at(lambda) * m, 0.45));

                                // Attenuation due to water vapor absorbtion
                                // w - precipitable water vapor in centimeters (standard = 2)
                                // Results agree with the graph (pg 132, MI)
        const real_t w = 2.0;
        tauWA = exp(-0.2385 * k_waCurve.at(lambda) * w * m /
                    pow(1 + 20.07 * k_waCurve.at(lambda) * w * m, 0.45));

        data[i] = 100 * solCurve.at(lambda) * tauR * tauA * tauO * tauG * tauWA;  // 100 comes from solCurve being
                                                                               // in wrong units.

    }
    return Spectrum::FromSampled(data, 350,800,91) * (1-overcast);  // Converts to Spectrum
//#endif
}

} } }
