//
//  teme2ecef.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        teme2ecef.c
//%
//% DESCRIPTION:          This function converts the position and velocity
//%                       vectors from the TEME frame to the ECEF frame
//%                       taking into account the effects of sidereal time
//%                       (GMST) and polar motion following Vallado (2013)
//%                       Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double r_teme[3]: position vector in TEME (m)
//%                       double v_teme[3]: velocity vector in TEME (m s-1)
//%                       doublt ttt: julian centuries of terrestrial time
//%                       double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double xp: polar motion parameter from EOP
//%                       double yp: polar motion parameter from EOP
//%                       double lod: length of day from EOP
//%
//% OUTPUT:               double r_ecef[3]: position vector in ECEF (m)
//%                       double v_ecef[3]: velocity vector in ECEF (m s-1)
//%                       double C_ecef2teme[3][3]: rotation matrix from
//%                         ECEF to TEME
//%
//% COUPLING:             - matxvec.c
//%                       - transpose.c
//%                       - crossprod.c
//%                       - polarm.c
//%                       - matrixmult.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef teme2ecef_h
#define teme2ecef_h

#include <stdio.h>

void teme2ecef(double r_teme[3], double v_teme[3], double ttt, double t2000ut1, double xp, double yp, double lod, double r_ecef[3], double v_ecef[3], double C_ecef2teme[3][3]);

#endif /* teme2ecef_h */
