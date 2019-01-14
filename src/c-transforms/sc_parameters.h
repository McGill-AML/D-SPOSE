//
//  sc_parameters.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-13.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sc_parameters.c
//%
//% DESCRIPTION:          This function will load the spacecraft parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 13, 2016
//% VERSION:              1
//%
//% INPUT:                double sc_parameters[33]: spacecraft parameters
//%
//% OUTPUT:               double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%                       double Inertia[3][3]: body-frame components of
//%                         matrix representation of inertia tensor
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double w[3]: body-frame components of angular
//%                         velocity vector of body-frame w.t.r inertia frame
//%                       double q[4]: quaternion orientation of body-frame
//%                       double M[3][3]: body-frame components of matrix
//%                         representation of magnetic tensor
//%
//% COUPLING:             - invertmat.c
//%                       - angle2quat.c
//%                       - quat2dcm.c
//%                       - orbital2state.c
//%                       - orbital2inertial.c
//%                       - matrixmult.c
//%                       - dcm2angle.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef sc_parameters_h
#define sc_parameters_h

#include <stdio.h>

void sc_parameters(double sc_parameters[33], double coe[9], double Inertia[3][3], double I_inv[3][3], double w[3], double q[4], double M[3][3]);

#endif /* sc_parameters_h */
