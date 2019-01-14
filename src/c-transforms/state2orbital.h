//
//  state2orbital.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        state2orbital.c
//%
//% DESCRIPTION:          This function converts the state parameters to
//%                       orbital parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 16, 2016
//% VERSION:              1
//%
//% INPUT:                double R[3]: position vector in inertial frame
//%                       double V[3]: velocity vector in inertial frame
//%
//% OUTPUT:               double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%                         - coe[6]: semi-major axis (m)
//%                         - coe[7]: orbital period (s)
//%                         - coe[8]: mean motion (rad s^-1)
//%
//% COUPLING:             - crossprod.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef state2orbital_h
#define state2orbital_h

#include <stdio.h>

void state2orbital(double R[3], double V[3], double coe[9]);

#endif /* state2orbital_h */
