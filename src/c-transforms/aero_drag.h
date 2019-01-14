//
//  aero_drag.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-13.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        aero_drag.c
//%
//% DESCRIPTION:          This function calculates the acceleration and
//%                       torque due to atmospheric drag
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 13, 2017
//% VERSION:              1
//%
//% INPUT:                double density: atmospheric density (kg m-3)
//%                       double w_vector[3]: wind velocity in inertial frame (m s-1)
//%                       double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double w[3]: angular velocity vector (rad s-1)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       double Cd: drag coefficient
//%                       int in_aero_a: inclusion of acceleration?
//%                       int in_aero_g: inclusion of torque?
//%
//% OUTPUT:               double a_aero[3]: acceleration due to atmospheric
//%                         drag in inertial frame
//%                       double g_aero[3]: torque due to atmospheric drag
//%                         in body frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - aero_force.c
//%                       - aero_torque.c
//%                       - matxvec.c
//%                       - invertmat.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef aero_drag_h
#define aero_drag_h

#include <stdio.h>
#include "surface.h"

void aero_drag(double density, double w_vector[3], double p[3], double v[3], double w[3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, double Cd, int in_aero_a, int in_aero_g, double a_aero[3], double g_aero[3]);

#endif /* aero_drag_h */
