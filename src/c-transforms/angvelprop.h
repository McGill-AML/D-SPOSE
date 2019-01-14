//
//  angvelprop.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        angvelprop.c
//%
//% DESCRIPTION:          This function propagates angular velocity
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double Inertia[3][3]: inertia matrix
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double w[3]: angular velocity vector
//%                       double g[3]: sum of torques vector
//%
//% OUTPUT:               double dw[3]: angular velocity update
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef angvelprop_h
#define angvelprop_h

#include <stdio.h>

void angvelprop(double Inertia[3][3], double I_inv[3][3], double w[3], double g[3], double dw[3]);
    
#endif /* angvelprop_h */
