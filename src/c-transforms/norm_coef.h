//
//  norm_coef.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-08-22.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        norm_coef.c
//%
//% DESCRIPTION:          This function will reformat the spherical harmonic
//%                       coefficients for the gravitational potential
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 August 22, 2017
//% VERSION:              1
//%
//% INPUT:                grav_coef[5148][6]: normalized gravity coefficients form input
//%
//% OUTPUT:               double C[101][101]: unnormalized gravity potential coefficients
//%                       double S[101][101]: unnormalized gravity potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef norm_coef_h
#define norm_coef_h

#include <stdio.h>

void norm_coef(double grav_coef[5148][6], double C[101][101], double S[101][101]);

#endif /* norm_coef_h */
