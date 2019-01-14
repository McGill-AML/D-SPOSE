//
//  gaus_coef_wmm.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-26.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gaus_coef_wmm.c
//%
//% DESCRIPTION:          This function will reformat the WMM coefficients
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 26, 2017
//% VERSION:              1
//%
//% INPUT:                wmm_coef[90][18]: WMM coefficients form input
//%
//% OUTPUT:               double G_wmm[13][13][8]: magnetic potential coefficients
//%                       double H_wmm[13][13][8]: magnetic potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef gaus_coef_wmm_h
#define gaus_coef_wmm_h

#include <stdio.h>

void gaus_coef_wmm(double wmm_coef[90][18], double G_wmm[13][13][8], double H_wmm[13][13][8]);

#endif /* gaus_coef_wmm_h */
