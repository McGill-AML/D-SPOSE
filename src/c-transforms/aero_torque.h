//
//  aero_torque.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-29.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        aero_torque.c
//%
//% DESCRIPTION:          This function calculates the torque on surface
//%                       due to atmospheric drag for a spinning satellite
//%                       (See Eq. (19) and Appendix B in Sagnieres et al.
//%                       (2018), JGCD)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 29, 2016
//% VERSION:              1
//%
//% INPUT:                double rho: atmospheric density (kg m-3)
//%                       double Cd: drag coefficient
//%                       double v[3]: relative wind speed body frame (m s-1)
//%                       double w[3]: relative atmosphere angular
//%                         velocity in body frame (m s-1)
//%                       struct surface geometry: surface geometry
//%
//% OUTPUT:               double g[3]: torque on surface due to atmospheric
//%                         drag in body frame
//%
//% COUPLING:             - surface.h
//%                       - dotprod.c
//%                       - crossprod.c
//%                       - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef aero_torque_h
#define aero_torque_h

#include <stdio.h>
#include "surface.h"

void aero_torque(double rho, double Cd, double v[3], double w[3], struct surface geometry, double g[3]);

#endif /* aero_torque_h */
