//
//  get_density.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-10-24.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        get_density.c
//%
//% DESCRIPTION:          This function will obtain the atmospheric density
//%                       at the satellite position from desired model
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 24, 2016
//% VERSION:              1
//%
//% INPUT:                double int atmos_model: which atmospheric model?
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double solar_input[length_of_file[1]][3]: F10.7 array for NRLMSISE-00
//%                       double Apc: user-inputted Ap value
//%                       double F107c: user-inputted F10.7 value
//%
//% OUTPUT:               double density: atmospheric density value at satellite
//%                         position (kg m-3)
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - nrlmsise-00.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef get_density_h
#define get_density_h

#include <stdio.h>

double get_density(int atmos_model, double LLA[4], double t2000utc, int length_of_file[5], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double Apc, double F107c);

#endif /* get_density_h */
