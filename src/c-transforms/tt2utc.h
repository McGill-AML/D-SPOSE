//
//  tt2utc.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-10.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        tt2utc.c
//%
//% DESCRIPTION:          This function converts terrestrial time to UTC by
//%                       going through atomic time and including leap seconds
//%                       (last leap second included is December 31 2016)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 10, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%
//% OUTPUT:               double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef tt2utc_h
#define tt2utc_h

#include <stdio.h>

double tt2utc(double t2000tt);

#endif /* tt2utc_h */
