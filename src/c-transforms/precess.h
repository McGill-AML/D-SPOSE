//
//  precess.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-24.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        precess.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       MOD frame to the ECI frame using IAU-76/FK5.
//%                       Eqs. 3-88 and 3-89 Vallado (2013) p. 228
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 24, 2017
//% VERSION:              1
//%
//% INPUT:                double ttt: julian centuries of terrestrial time
//%
//% OUTPUT:               double prec[3][3]: rotation matrix from MOD to ECI
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef precess_h
#define precess_h

#include <stdio.h>

void precess(double ttt, double prec[3][3]);

#endif /* precess_h */
