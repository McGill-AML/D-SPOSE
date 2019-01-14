//
//  crossprod.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-03-02.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        crossprod.c
//%
//% DESCRIPTION:          This function performs a cross product
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 2, 2016
//% VERSION:              1
//%
//% INPUT:                double A[3]: 3x1 vector
//%                       double B[3]: 3x1 vector
//%
//% OUTPUT:               double AcrossB[3]: 3x1 vector containing A x B
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef crossprod_h
#define crossprod_h

#include <stdio.h>

void crossprod(double A[3], double B[3], double AcrossB[3]);

#endif /* crossprod_h */
