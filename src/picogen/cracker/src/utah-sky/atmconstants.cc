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
/* Copyright 1998
 * Thu Apr 15 15:33:46 1999  Brian Smits  (bes@phoenix.cs.utah.edu)
 *
 * RiAtmConstants.C
 *
 *
 *
 * $Id: RiAtmConstants.C,v 1.1 1999/06/10 20:12:29 bes Exp $
 *
 */

#include "geometry.hh"
#include "constants.hh"
#include "sunsky.hh"

#include <iostream>

/*#ifndef RICOMMON_H
#include <RiCommon.H>
#endif

#ifndef PssSunSky_H
#include <PssSunSky.H>
#endif

#ifndef RISPECTRALCURVE_H
#include <RiSpectralCurve.H>
#endif
*/
namespace picogen { namespace redshift { namespace background {

namespace {
        // 300-800 at 10 nm
        PssSunSky::real_t n2_1Amplitudes[51] = {
          3.4012e-7, 3.3702e-7, 3.3425e-7, 3.3177e-7, 3.2955e-7,
          3.2754e-7, 3.2572e-7, 3.2406e-7, 3.2255e-7, 3.2116e-7,

          3.1989e-7, 3.1873e-7, 3.1765e-7, 3.1665e-7, 3.1572e-7,
          3.1486e-7, 3.1407e-7, 3.1332e-7, 3.1263e-7, 3.1198e-7,

          3.1137e-7, 3.1080e-7, 3.1026e-7, 3.0976e-7, 3.0928e-7,
          3.0884e-7, 3.0841e-7, 3.0801e-7, 3.0763e-7, 3.0728e-7,

          3.0694e-7, 3.0661e-7, 3.0631e-7, 3.0602e-7, 3.0574e-7,
          3.0547e-7, 3.0522e-7, 3.0498e-7, 3.0475e-7, 3.0453e-7,

          3.0432e-7, 3.0412e-7, 3.0393e-7, 3.0375e-7, 3.0357e-7,
          3.0340e-7, 3.0324e-7, 3.0309e-7, 3.0293e-7, 3.0279e-7,

          3.0265e-7
        };

        // 300-800 at 1nm
        PssSunSky::real_t KAmplitudes[501] = {
        #include "K.spectrum"
        };

        PssSunSky::real_t K25Amplitudes[501] = {
        #include "K25.spectrum"
        };

        //300- 790 at 10nm  (for each of 1801 tenths of a degree
        PssSunSky::real_t netaLambdaTable[50][1801] = {
        #include "Neta.Table"
        };

        PssSunSky::Spectrum netaTable[1801];

        PssSunSky::real_t const Anis_cor = 1.06;  // Correction for molecular anisotropy.
        PssSunSky::real_t const N = 2.545e25; // Number of molecules per unit volume.

}

void InitNetaTable()
{
    PssSunSky::real_t data[50];

    // samplesPositions: quick hack
    for(int i = 0; i < 1801; i++) {
        for(int j = 0; j < 50; j++)
          data[j] = netaLambdaTable[j][i];

        netaTable[i] = PssSunSky::Spectrum::FromSampled(data,300,790,50);
    }
}


/* might need to be converted because the above functions use a
   different system ... */
void  PssSunSky::CreateConstants()
{
    InitNetaTable();

    real_t lambda;
    real_t b_m[101], b_p[101];
    real_t b_m_ang_prefix[101];
    real_t b_p_ang_prefix[101];

    int i;
    real_t lambdasi, Nlambda4;
    real_t n2_1;
    real_t K;
    real_t c = (0.06544204545455*turbidity -0.06509886363636)*1e-15;
    Spectrum KCurve = Spectrum::FromSampled(KAmplitudes, 300,800,501);
    Spectrum K25Curve = Spectrum::FromSampled(K25Amplitudes, 300,800,501);
    Spectrum n2_1Curve = Spectrum::FromSampled(n2_1Amplitudes, 300,800,51);

    for (lambda = 300,i=0; lambda <= 800; lambda+=5,i++)
    {
        lambdasi = lambda*1e-9;  /* Converstion to SI units */

        Nlambda4 = N*lambdasi*lambdasi*lambdasi*lambdasi;

        /* Rayleigh total scattering coefficient */
        /* (4-48), p 199, MC */
        n2_1 = n2_1Curve.at(lambda);
        b_m[i] =
                (8*M_PI*M_PI*M_PI*n2_1*Anis_cor)/
                (3*Nlambda4);


        /* Mie total scattering coefficient */
        /* (6-30). p 280; MC */
        if (V > 3.9) K = KCurve.at(lambda); // V = 4.0;
        else K = K25Curve.at(lambda);       // V = 2.5

        b_p[i] = 0.434*M_PI*c*pow(2*constants::pi/lambdasi,V-2) * K *
            pow(0.01,V-3);  /* Last term is unit correction for c */

        /* Rayleigh Angular scattering coefficient */
        /*  (4-55), p 200; MC */
        /* Needs to be multiplied by (1+0.9324cos^2(theta)) to get exact
           angular scattering constant */
        b_m_ang_prefix[i] = 2*M_PI*M_PI*n2_1*Anis_cor*0.7629/
            (3*Nlambda4);


        /* Mie Angular scattering coefficient */
        /* (6-24), p 271; MC */
        /* Needs to be multiplied by neta(theta,lambda) to get exact
           angular scattering constant term */
        b_p_ang_prefix[i] = 0.217*c*pow(2*M_PI/lambdasi,V-2)*
            pow(0.01,V-3);  /* Last term is unit correction for c */
    }

    beta_m = Spectrum::FromSampled(b_m, 300,800,101);
    beta_p = Spectrum::FromSampled(b_p, 300,800,101);
    beta_m_ang_prefix = Spectrum::FromSampled(b_m_ang_prefix, 300,800,101);
    beta_p_ang_prefix = Spectrum::FromSampled(b_p_ang_prefix, 300,800,101);

    /*
    std::cout << "beta_m {\n" << std::fixed;
    std::cout << "   orig: "; for (int i=0;i<101;++i) std::cout << b_m[i] << "  ";
    std::cout << "\n   new: "; for (int i=0;i<Spectrum::num_components;++i) std::cout << beta_m[i] << "  ";
    std::cout << "\n}\n";
    std::cout << std::endl;

    std::cout << "beta_p {\n" << std::fixed;
    std::cout << "   orig: "; for (int i=0;i<101;++i) std::cout << b_p[i] << "  ";
    std::cout << "\n   new: "; for (int i=0;i<Spectrum::num_components;++i) std::cout << beta_p[i] << "  ";
    std::cout << "\n}\n";
    std::cout << std::endl;
    */
}



PssSunSky::Spectrum PssSunSky::GetNeta(
        PssSunSky::real_t theta,
        PssSunSky::real_t //v
) const
{
    /* // original
    theta = theta*(180.0/M_PI) * 10;
    real_t u = theta - (int)theta;
    if((theta < 0)||(theta > 1801)) {
        std::cerr << "Error:getNeta() [theta outside range] theta=" << theta/10
             <<"degrees" << std::endl;
        return Spectrum(real_t(0));
    }

    if (theta > 1800) theta=1800;

    if ((int)theta==1800)
      return  netaTable[1800];
    */
    theta = theta*(180.0/M_PI) * 10;
    const int itheta = static_cast<int>(theta);
    const real_t u = theta - itheta;

    if (itheta < 0) return netaTable[0];
    if (itheta >= 1800) return netaTable[1800];

    return (1-u) * netaTable[itheta] + u * netaTable[itheta+1];

}

} } }
