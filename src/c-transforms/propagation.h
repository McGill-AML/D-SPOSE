//
//  propagation.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        propagation.c
//%
//% DESCRIPTION:          This function will propagate the attitude and orbit
//%                       of a spacecraft under certain external torques
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       double x[13]: state vector
//%                       double Inertia[3][3]: inertia matrix
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double M[3][3]: magnetic tensor
//%                       double m: spacecraft mass
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double solar_input[length_of_file[1]][3]: F10.7 array for NRLMSISE-00
//%                       double G[14][14][25]: IGRF-12 magnetic potential coefficients
//%                       double H[14][14][25]: IGRF-12 magnetic potential coefficients
//%                       double G_wmm[13][13][8]: WMM magnetic potential coefficients
//%                       double H_wmm[13][13][8]: WMM magnetic potential coefficients
//%                       double model_parameters[27]: model parameters
//%                       double eop[length_of_file[2]][10]: earth orientation parameters
//%                       double sun_eph[length_of_file[3]][3]: sun ephemerides array
//%                       double moon_eph[length_of_file[4]][3]: moon ephemerides array
//%                       double albedo[12][20][40][2]: albedo and IR coefficients
//%                       double fn[3]: 3x1 vector sum of non-conservative forces
//%                       double gn[3]: 3x1 vector sum of non-conservative torques
//%                       double fg_i[42]: array containing list of 3x1 vector of forces
//%                         and torques
//%
//% OUTPUT:               double dx[13]: state vector update
//%                       double dxd[7]: state vector update (Kane damper)
//%
//% COUPLING:             - quatnormalize.c
//%                       - teme2ecef.c
//%                       - ecef2lla.c
//%                       - quat2dcm.c
//%                       - matxvec.c
//%                       - surface.h
//%                       - t2doy.c
//%                       - tt2utc.c
//%                       - invertmat.c
//%                       - angvelprop.c
//%                       - moon.c
//%                       - sun.c
//%                       - third_body.c
//%                       - eddy_torque.c
//%                       - gravity_field.c
//%                       - srp.c
//%                       - albedo_calc.c
//%                       - get_density.c
//%                       - wind.c
//%                       - magnet_field.c
//%                       - magnet_field_wmm.c
//%                       - aero_drag.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef propagation_h
#define propagation_h

#include <stdio.h>
#include "surface.h"

void propagation(double t2000tt, double x[13], double Inertia[3][3], double I_inv[3][3], double M[3][3], double m, int n_surf, struct surface geometry[n_surf], double C[101][101], double S[101][101], int length_of_file[5], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double G[14][14][25], double H[14][14][25], double G_wmm[13][13][8], double H_wmm[13][13][8], double model_parameters[27], double eop[length_of_file[2]][10], double sun_eph[length_of_file[3]][3], double moon_eph[length_of_file[4]][3], double albedo[12][20][40][2], double fn[3], double gn[3], double fg_i[42], double dx[13], double xd[7], double dxd[7]);

#endif /* propagation_h */
