//
//  ecef2lla.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-26.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        ecef2lla.c
//%
//% DESCRIPTION:          This function converts the position vector in ECEF
//%                       to geodetic and geocentric latitude, longitude,
//%                       and height above the ellipsoid (altitude) following
//%                       Vallado (2013) Section 3.4.4
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 26, 2017
//% VERSION:              1
//%
//% INPUT:                double r[3]: position vector in ECEF (m)
//%
//% OUTPUT:               double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef ecef2lla_h
#define ecef2lla_h

#include <stdio.h>

void ecef2lla(double r[3], double LLA[4]);

#endif /* ecef2lla_h */
