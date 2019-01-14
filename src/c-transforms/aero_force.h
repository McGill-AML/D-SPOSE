//
//  aero_force.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-29.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        aero_force.c
//%
//% DESCRIPTION:          This function calculates the force on surface
//%                       due to atmospheric drag for a spinning satellite
//%                       (See Eq. (18) and Appendix B in Sagnieres et al.
//%                       (2018), JGCD)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 29, 2016
//% VERSION:              1
//%
//% INPUT:                double rho: atmospheric density (kg m-3)
//%                       double Cd: drag coefficient
//%                       double v[3]: relative wind speed ECI (m s-1)
//%                       double w_rel_i[3]: relative atmosphere angular
//%                         velocity inertial frame (m s-1)
//%                       struct surface geometry: surface geometry
//%                       double C_b2i[3][3]: rotation matrix from body
//%                         frame to inertial frame
//%
//% OUTPUT:               double f[3]: force on surface due to atmospheric
//%                         drag in inertial frame
//%
//% COUPLING:             - surface.h
//%                       - dotprod.c
//%                       - crossprod.c
//%                       - norm.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef aero_force_h
#define aero_force_h

#include <stdio.h>
#include "surface.h"

void aero_force(double rho, double Cd, double v[3], double w[3], struct surface geometry, double C_b2i[3][3], double f[3]);

#endif /* aero_force_h */
