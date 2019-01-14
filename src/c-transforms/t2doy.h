//
//  t2doy.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-11-05.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        days2mdh.c
//%
//% DESCRIPTION:          This function converts t2000 to the year, day of
//%                       year, and seconds of day
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 5, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000: seconds since January 1, 2000, 00:00:00
//%
//% OUTPUT:               int time[3]:
//%                         - time[0]: year
//%                         - time[1]: day of year
//%                         - time[2]: days since January 1, 2000
//%                       double sec: seconds of day
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef t2doy_h
#define t2doy_h

#include <stdio.h>

double t2doy(double t2000, int time[3]);

#endif /* t2doy_h */
