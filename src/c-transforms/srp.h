//
//  srp.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-12-18.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        srp.c
//%
//% DESCRIPTION:          This function calculates the solar radiation
//%                       acceleration and torque for the entire surface model
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 December 18, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double r_sun[3]: position of Sun (m)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       int in_srp_a: inclusion of acceleration?
//%                       int in_srp_g: inclusion of torque?
//%
//% OUTPUT:               double a_srp[3]: direct solar radiation acceleration
//%                         in inertial frame
//%                       double g_srp[3]: direct solar radiation torque
//%                         in body-fixed frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - norm.c
//%                       - srp_force.c
//%                       - crossprod.c
//%                       - matxvec.c
//%                       - invertmat.c
//%                       - shadow_function.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef srp_h
#define srp_h

#include <stdio.h>
#include "surface.h"

void srp(double p[3], double v[3], double r_sun[3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, int in_srp_a, int in_srp_g, double a_srp[3], double g_srp[3]);

#endif /* srp_h */
