//
//  grav_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        grav_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the Earth
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int l_max: maximum order and degree in spherical
//%                         harmonic expansion
//%
//% OUTPUT:               double U: potential energy due to Earth (J)
//%
//% COUPLING:             - tt2utc.c
//%                       - teme2ecef.c
//%                       - ecef2lla.c
//%                       - t2doy.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef grav_potential_h
#define grav_potential_h

#include <stdio.h>

double grav_potential(double t2000tt, double p[3], double v[3], double m, double C[101][101], double S[101][101], int l_max);

#endif /* grav_potential_h */
