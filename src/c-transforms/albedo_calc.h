//
//  albedo_calc.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-07-02.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        albedo_calc.c
//%
//% DESCRIPTION:          This function calculates the acceleration and
//%                       torque due to reflected and emitted Earth radiation
//%                       (See Section 2.3.3 in Sagnieres (2018) Doctoral Thesis)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 July 2, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[0]: geodetic latitude (rad)
//%                         - LLA[0]: longitude (rad)
//%                         - LLA[0]: altitude (km)
//%                       int time[3]:
//%                         - time[0]: year
//%                         - time[1]: day of year
//%                         - time[2]: days since January 1, 2000
//%                       double r_sun[3]: position of Sun (m)
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF
//%                         frame to TEME
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       double albedo[12][20][40][2]: albedo and IR coefficients
//%                       int in_alb_a: inclusion of albedo acceleration?
//%                       int in_alb_g: inclusion of albedo torque?
//%                       int in_ir_a: inclusion of IR acceleration?
//%                       int in_ir_g: inclusion of IR torque?
//%
//% OUTPUT:               double a_alb[3]: reflected Earth radiation acceleration
//%                         in inertial frame
//%                       double g_alb[3]: reflected Earth radiation torque
//%                         in body-fixed frame
//%                       double a_in[3]: emitted Earth radiation acceleration
//%                         in inertial frame
//%                       double g_in[3]: emitted Earth radiation torque
//%                         in body-fixed frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - norm.c
//%                       - crossprod.c
//%                       - matxvec.c
//%                       - invertmat.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef albedo_calc_h
#define albedo_calc_h

#include <stdio.h>
#include "surface.h"

void albedo_calc(double p[3], double v[3], double LLA[4], int time[3], double r_sun[3], double C_ecef2teme[3][3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, double albedo[12][20][40][2], int in_alb_a, int in_alb_g, int in_ir_a, int in_ir_g, double a_alb[3], double g_alb[3], double a_ir[3], double g_ir[3]);

#endif /* albedo_calc_h */
