// -*- C++ -*-
#ifndef PssSunSky_H
#define PssSunSky_H
/* Copyright 1999
 * Mon Apr 19 13:42:11 1999  Brian Smits  (bes@phoenix.cs.utah.edu)
 *
 * PssSunSky.H
 *
 *
 *
 * $Id: PssSunSky.H,v 1.1 1999/06/10 20:12:32 bes Exp $
 *
 */

/*
#ifndef RICOMMON_H
#include <RiCommon.H>
#endif

#ifndef RISPECTRUM_H
#include <Spectrum.H>
#endif

#ifndef RIVECTOR3_H
#include <Vector.H>
#endif
*/

#include "../../redshift/include/setup.hh"


namespace redshift {
/***************************************************************
CLASS
    PssSunSky
     <one line summary>

DESCRIPTION
     <Detailed description with any warnings>

****************************************************************/

class PssSunSky {
  public:
        typedef double real_t;
        typedef redshift::SpectrumBase<real_t> Spectrum;
				// GROUP: Constructors and assignment
				//// Constructs an PssSunSky based on
				// [in] lat Latitude (0-360)
				// [in] long Longitude (-90,90) south to north
				// [in] sm  Standard Meridian
				// [in] jd  Julian Day (1-365)
				// [in] tod Time Of Day (0.0,23.99) 14.25 = 2:15PM
				// [in] turb  Turbidity (1.0,30+) 2-6 are most useful for clear days.
				// [in] initAtmEffects  if atm effects are not initialized, bad things will
				// happen if you try to use them....
    PssSunSky(real_t lat, real_t longi, int sm, int jd, real_t tod, real_t turb,bool initAtmEffects = true);
				// GROUP: Members
				////  Position (actual Direction) of the sun
				// South = +x,  East = +y, up = +z
    Vector	GetSunPosition() const;
				//// [out] theta  Sun angle down? from straight above
				//   [out] phi    Sun angle around from South?
    void 	SunThetaPhi(real_t &theta, real_t &phi) const;
				////  Spectral radiance making it to the earth's surface
    Spectrum 	GetSunSpectralRadiance() const;
				////  Solid angle of the sun (constant
    real_t	GetSunSolidAngle() const;
				//// Spectral radiance of the sky in the specified direction
    Spectrum  GetSkySpectralRadiance(const Vector &v) const;
    color::xyY GetSkySpectralRadiance_xyY(const Vector &v) const;
				//// An alternate way of getting the sky's spectral radiance
    Spectrum  GetSkySpectralRadiance(real_t theta, real_t phi) const;
    color::xyY GetSkySpectralRadiance_xyY(real_t theta, real_t phi) const;
				////  Compute Atmospheric Perspective based on
				// [in] viewer  Position of the viewer
				// [in] source  Position of the distant point
				// [out] atmAttenuation Attenuation due to atmosphere
				// [out] atmInscatter  Amount of light scattered into the viewing direction
    void GetAtmosphericEffects(const Vector &viewer,
			       const Vector &source,
			       Spectrum &atmAttenuation,
			       Spectrum &atmInscatter ) const;
  private:
				//// Copy Constructor (unimplemented)
    PssSunSky(const PssSunSky &);
				//// Assignment (unimplemented)
    PssSunSky &operator=(const PssSunSky &);
				//// Compute the sun's position based on IES Sunlight Publication ????
    void 	InitSunThetaPhi();
				//// Transmittance for the sun weighted by sun's spectrum.
    Spectrum ComputeAttenuatedSunlight(real_t theta, int turbidity);
				//// Conversion from xy to a Spectrum
    Spectrum ChromaticityToSpectrum(real_t x, real_t y) const;
				// GROUP:  RiAtmPerspective.C Functions
				////
    Spectrum AttenuationFactor(real_t h0, real_t thetav, real_t s) const;
				////
    Spectrum InscatteredRadiance(real_t h0, real_t thetav, real_t
				   phiv, real_t s) const;
				////
    Spectrum GetNeta(real_t theta, real_t v) const;
				////
    void CalculateA0(real_t thetav, real_t phiv, Spectrum& A0_1, Spectrum& A0_2) const;
				////
    void CreateConstants();
				//// Preprocessing for calculating ambient scattered light for all
				//  directions
    void InitA0() const;
				////
    real_t PerezFunction(const real_t *lam, real_t theta, real_t phi, real_t lvz) const;
				//Group: Data
    real_t 	latitude, longitude;
    int 	julianDay;
    real_t 	timeOfDay;
    int 	standardMeridian;
    real_t 	turbidity;
				//// Sun Position Vector
    Vector 	toSun;
				//// Sun Position
    real_t	thetaS, phiS;
				//// Spectral Radiance computed at creation time
    Spectrum 	sunSpectralRad;
				//// Solid angle of the sun when seen from earth
    real_t 	sunSolidAngle;
				//// Perez data
    real_t zenith_Y, zenith_x, zenith_y;
				//// Perez coefficients (change to 5 when arrays become zero based)
    real_t perez_Y[6], perez_x[6], perez_y[6];
				// GROUP: Constants for Atmospheric Perspective
				////
    Spectrum beta_m, beta_p, beta_m_ang_prefix,  beta_p_ang_prefix;
				////
    real_t	V;
				////
    bool atmInited;
};

} // namespace redshift

#endif /* PssSunSky_H */
