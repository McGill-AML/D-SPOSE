//
//  orbital2state.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        orbital2state.c
//%
//% DESCRIPTION:          This function converts the orbital parameters to
//%                       state parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 6, 2016
//% VERSION:              1
//%
//% INPUT:                double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%
//% OUTPUT:               double R[3]: position vector in inertial frame
//%                       double V[3]: velocity vector in inertial frame
//%
//% COUPLING:             - matrixmult.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef orbital2state_h
#define orbital2state_h

#include <stdio.h>

void orbital2state(double R[3], double V[3], double coe[9]);

#endif /* orbital2state_h */
