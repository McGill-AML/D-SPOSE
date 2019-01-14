//
//  sun_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sun_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the Sun
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double r_sun[3]: 3x1 Sun position vector (m)
//%
//% OUTPUT:               double U_sun: potential energy due to Sun (J)
//%
//% COUPLING:             - vectors2angle.c
//%                       - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef sun_potential_h
#define sun_potential_h

#include <stdio.h>

double sun_potential(double p[3], double m, double r_sun[3]);

#endif /* sun_potential_h */
