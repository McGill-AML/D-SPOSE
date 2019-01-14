//
//  srp_force.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-12-19.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        srp_force.c
//%
//% DESCRIPTION:          This function calculates the force on a single
//%                       surface due to direct solar radiation
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 December 19, 2017
//% VERSION:              1
//%
//% INPUT:                struct surface geometry: surface geometry
//%                       double cos_alpha: cosine of angle between light
//%                         direction and inward surface normal
//%                       double C_b2i[3][3]: rotation matrix from body
//%                         frame to inertial frame
//%                       double light[3]: incident light direction
//%
//% OUTPUT:               double f_surf[3]: direct solar radiation force
//%                         in inertial frame
//%
//% COUPLING:             - surface.h
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef srp_force_h
#define srp_force_h

#include <stdio.h>
#include "surface.h"

void srp_force(struct surface geometry, double cos_alpha, double C_b2i[3][3], double light[3], double f_surf[3]);

#endif /* srp_force_h */
