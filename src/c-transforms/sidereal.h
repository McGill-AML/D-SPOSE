//
//  sidereal.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sidereal.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       PEF frame to the TOD frame
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double deltapsi: nutation angle
//%                       double meaneps: mean obliquity of the ecliptic
//%                       double omega: Delaunay parameter Omega
//%
//% OUTPUT:               double st[3][3]: rotation matrix from PEF to TOD
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef sidereal_h
#define sidereal_h

#include <stdio.h>

void sidereal(double t2000ut1, double deltapsi, double meaneps, double omega, double st[3][3]);

#endif /* sidereal_h */
