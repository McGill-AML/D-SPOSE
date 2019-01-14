//
//  orbit2inertial.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        orbit2inertial.c
//%
//% DESCRIPTION:          This function converts a position and velocity
//%                       vector to a rotation matrix rotating from the
//%                       orbital frame to the inertial frame
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 16, 2016
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m s-1)
//%
//% OUTPUT:               double C_o2i[3][3]: rotation matrix from orbital
//%                         frame to inertial frame
//%
//% COUPLING:             - crossprod.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef orbit2inertial_h
#define orbit2inertial_h

#include <stdio.h>

void orbit2inertial(double p[3], double v[3], double C_o2i[3][3]);

#endif /* orbit2inertial_h */
