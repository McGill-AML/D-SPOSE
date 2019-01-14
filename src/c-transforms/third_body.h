//
//  third_body.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-07.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        third_body.c
//%
//% DESCRIPTION:          This function calculates the acceleration due to a
//%                       third body according to Vallado (2013) p. 574-575
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 7, 2017
//% VERSION:              1
//%
//% INPUT:                double p[3]: position vector
//%                       double p_third[3]: position vector of 3rd body
//%                       double mu_third: 3rd body gravitational parameters
//%
//% OUTPUT:               double a_third[3]: acceleration vector due to 3rd body
//%
//% COUPLING:             - norm.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef third_body_h
#define third_body_h

#include <stdio.h>

void third_body(double p[3], double p_third[3], double mu_third, double a_third[3]);

#endif /* third_body_h */
