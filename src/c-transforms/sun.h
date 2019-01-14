//
//  sun.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-06.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sun.c
//%
//% DESCRIPTION:          This function loads sun position for current time
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 6, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       int length_of_file[5]: array with lengths of files
//%                       double sun_eph[length_of_file[3]][3]: sun ephemerides
//%
//% OUTPUT:               double r_sun[3]: position of Sun in inertial frame
//%                         (MOD, assumed to be equal to initial TEME) (m)
//%                       double r: distance from Earth to Sun (m)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef sun_h
#define sun_h

#include <stdio.h>

double sun(double t2000tt, int length_of_file[5], double sun_eph[length_of_file[3]][3], double r_sun[3]);

#endif /* sun_h */
