//
//  moon_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        moon_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the moon
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double r_moon[3]: 3x1 moon position vector (m)
//%
//% OUTPUT:               double U_moon: potential energy due to Moon (J)
//%
//% COUPLING:             - vectors2angle.c
//%                       - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef moon_potential_h
#define moon_potential_h

#include <stdio.h>

double moon_potential(double p[3], double m, double r_moon[3]);

#endif /* moon_potential_h */
