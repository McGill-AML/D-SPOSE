//
//  moon.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-06.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        moon.c
//%
//% DESCRIPTION:          This function loads moon position for current time
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 6, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       int length_of_file[5]: array with lengths of files
//%                       double moon_eph[length_of_file[4]][3]: moon ephemerides
//%
//% OUTPUT:               double r_moon[3]: position of Moon in inertial frame
//%                         (MOD, assumed to be equal to initial TEME) (m)
//%                       double r: distance from Earth to Moon (m)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef moon_h
#define moon_h

#include <stdio.h>

double moon(double t2000tt, int length_of_file[5], double moon_eph[length_of_file[4]][3], double r_moon[3]);

#endif /* moon_h */
