//
//  gaus_coef.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-08-22.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gaus_coef.c
//%
//% DESCRIPTION:          This function will reformat the IGRF-12 coefficients
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 August 22, 2017
//% VERSION:              1
//%
//% INPUT:                double mag_coef[195][27]: IGRF-12 coefficients form input
//%
//% OUTPUT:               double G[14][14][25]: magnetic potential coefficients
//%                       double H[14][14][25]: magnetic potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef gaus_coef_h
#define gaus_coef_h

#include <stdio.h>

void gaus_coef(double mag_coef[195][27], double G[14][14][25], double H[14][14][25]);

#endif /* gaus_coef_h */
