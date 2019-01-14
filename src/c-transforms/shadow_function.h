//
//  shadow_function.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-12-19.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        shadow_function.c
//%
//% DESCRIPTION:          This function calculates the shadow function for
//%                       a satellite in Earth umbra and penumbra following
//%                       Montenbruck and Gill (2005) Satellite Orbits:
//%                       Models, Methods and Applications
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 December 19, 2017
//% VERSION:              1
//%
//% INPUT:                double r_sun[3]: position of Sun (m)
//%                       double p[3]: spacecraft position (m)
//%
//% OUTPUT:               double nu: portion of light blocked by Earth
//%
//% COUPLING:             - dotprod.c
//%                       - norm.c
//%                       - vectors2angle.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef shadow_function_h
#define shadow_function_h

#include <stdio.h>

double shadow_function(double r_sun[3], double p[3]);

#endif /* shadow_function_h */
