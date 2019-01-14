//
//  nutation.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        nutation.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       TOD frame to the MOD frame using 1976 Delaunay
//%                       parameters following Vallado (2013) Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double ttt: julian centuries of terrestrial time
//%                       double iar80[106][5]: nutation parameters
//%                       double rar80[106][4]: nutation parameters
//%                       double ddpsi: EOP corrections
//%                       double ddeps: EOP corrections
//%
//% OUTPUT:               double nut[3][3]: rotation matrix from TOD to MOD
//%                       double nut_param[3]:
//%                         - nut_param[0]: nutation angle
//%                         - nut_param[1]: mean obliquity of the ecliptic
//%                         - nut_param[2]: Delaunay parameter Omega
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef nutation_h
#define nutation_h

#include <stdio.h>

void nutation(double ttt, double iar80[106][5], double rar80[106][4], double ddpsi, double ddeps, double nut_param[3], double nut[3][3]);

#endif /* nutation_h */
