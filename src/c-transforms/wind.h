//
//  wind.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-03-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        wind.c
//%
//% DESCRIPTION:          This function will calculate the wind at satellite
//%                       position
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 16, 2017
//% VERSION:              1
//%
//% INPUT:                double p_ecef[3]: position vector in ECEF frame
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF to TEME
//%                       int wind_model: which wind model?
//%
//% OUTPUT:               double winds_i[3]: wind velocity at satellite
//%                         position (m s-1)
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef wind_h
#define wind_h

#include <stdio.h>

void wind(double p_ecef[3], double LLA[4], double t2000utc, int length_of_file[5], double ap_index[length_of_file[0]], double C_ecef2teme[3][3], int wind_model, double winds_i[3]);

#endif /* wind_h */
