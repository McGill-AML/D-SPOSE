//
//  ecef2eci.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        ecef2eci.c
//%
//% DESCRIPTION:          This function converts the position and velocity
//%                       vectors from the ECEF frame to the ECI frame
//%                       taking into account the effects of sidereal time,
//%                       polar motion, nutation, and precession (IAU-76/FK5)
//%                       following Vallado (2013) Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double r_teme[3]: position vector in TEME (m)
//%                       double v_teme[3]: velocity vector in TEME (m s-1)
//%                       double ttt: julian centuries of terrestrial time
//%                       double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double iar80[106][5]: nutation parameters
//%                       double rar80[106][4]: nutation parameters
//%                       double xp: polar motion parameter from EOP
//%                       double yp: polar motion parameter from EOP
//%                       double lod: length of day from EOP
//%                       double dpsi: EOP corrections
//%                       double deps: EOP corrections
//%
//% OUTPUT:               double r_eci[3]: position vector in ECI (m)
//%                       double v_eci[3]: velocity vector in ECI (m s-1)
//%                       double C_ecef2eci[3][3]: rotation matrix from
//%                         ECEF to ECI
//%
//% COUPLING:             - precess.c
//%                       - nutation.c
//%                       - matxvec.c
//%                       - sidereal.c
//%                       - crossprod.c
//%                       - polarm.c
//%                       - matrixmult.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef ecef2eci_h
#define ecef2eci_h

#include <stdio.h>

void ecef2eci(double r_ecef[3], double v_ecef[3], double ttt, double t2000ut1, double iar80[106][5], double rar80[106][4], double xp, double yp, double lod, double dpsi, double deps, double r_eci[3], double v_eci[3], double C_ecef2eci[3][3]);

#endif /* ecef2eci_h */
