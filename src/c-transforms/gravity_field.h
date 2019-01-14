//
//  gravity_field.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-07.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gravity_field.c
//%
//% DESCRIPTION:          This function calculates the gravitational acceleration
//%                       from aspherical terms and gravity-gradient torque
//%                       (See Sections 2.2.1 and 2.2.2 in Sagnieres (2018)
//%                       Doctoral Thesis)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 7, 2017
//% VERSION:              1
//%
//% INPUT:                double p_ecef[3]: position in ECEF frame (m)
//%                       double p[3]: 3x1 position vector (m)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       double Inertia[3][3]: inertia matrix
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int l_max_a: maximum order and degree in spherical
//%                         harmonic expansion for acceleration calculation
//%                       int l_max_g: maximum order and degree in spherical
//%                         harmonic expansion for torque calculation
//%                       int in_grav_a: inclusion of acceleration?
//%                       int in_grav_g: inclusion of torque?
//%
//% OUTPUT:               double a_gravity_inertial[3]: acceleration due to
//%                         non-spherical Earth in inertial frame
//%                       double g_gravity_body[3]: gravity-gradient torque
//%                         in body-fixed frame
//%
//% COUPLING:             - matrixmult.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef gravity_field_h
#define gravity_field_h

#include <stdio.h>

void gravity_field(double p_ecef[3], double p[3], double LLA[4], double C_i2b[3][3], double Inertia[3][3], double C[101][101], double S[101][101], int l_max_a, int l_max_g, int in_grav_a, int in_grav_g, double a_gravity_inertial[3], double g_gravity_body[3]);

#endif /* gravity_field_h */
