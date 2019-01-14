//
//  invertmat.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        invertmat.c
//%
//% DESCRIPTION:          This function inverts a matrix
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double A[3][3]: 3x3 matrix
//%
//% OUTPUT:               double A_inv[3][3]: 3x3 matrix representing A^{-1}
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef invertmat_h
#define invertmat_h

#include <stdio.h>

void invertmat(double A[3][3], double A_inv[3][3]);

#endif /* invertmat_h */
