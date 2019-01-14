//
//  magnet_field.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        magnetic_field.c
//%
//% DESCRIPTION:          This function will calculate the magnetic field
//%                       vector and its time derivative at the satellite position
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 11, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       double p[3]: position in TEME frame (m)
//%                       double p_LLA[3]: position in ECEF frame (m)
//%                       double v_LLA[3]: velocity in ECEF frame (m s-1)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF to TEME
//%                       double G[14][14][25]: magnetic potential coefficients
//%                       double H[14][14][25]: magnetic potential coefficients
//%
//% OUTPUT:               double B_field_i[3]: magnetic field vector in inertial frame
//%                       double B_field_i_dot[3]: time derivative of magnetic field
//%                         vector in inertial frame as seen from orbiting
//%                         spacecraft
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - invertmat.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef magnet_field_h
#define magnet_field_h

#include <stdio.h>

void magnet_field(double t2000utc, double p[3], double p_LLA[3], double v_LLA[3], double LLA[4], double C_ecef2teme[3][3], double G[14][14][25], double H[14][14][25], double B_field_i[3], double B_field_i_dot[3]);

#endif /* magnet_field_h */
