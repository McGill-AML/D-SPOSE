//
//  polarm.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-31.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        polarm.c
//%
//% DESCRIPTION:          This function outputs a rotation matrix to go from
//%                       ECEF to PEF taking into account polar motion
//%                       following Vallado (2013) Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 31, 2017
//% VERSION:              1
//%
//% INPUT:                double ttt: julian centuries of terrestrial time
//%                       double xp: polar motion parameter from EOP
//%                       double yp: polar motion parameter from EOP
//%
//% OUTPUT:               double pm[3][3]: rotation matrix from ECEF to PEF
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef polarm_h
#define polarm_h

#include <stdio.h>

void polarm(double xp, double yp, double ttt, double pm[3][3]);

#endif /* polarm_h */
