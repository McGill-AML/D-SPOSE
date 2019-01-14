//
//  matrixmult.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        matrixmult.c
//%
//% DESCRIPTION:          This function multiplies two 3x3 matrices
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double A[3][3]: 3x3 matrix
//%                       double B[3][3]: 3x3 matrix
//%
//% OUTPUT:               double C[3][3]: 3x3 matrix obtained from multiplying AB
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef matrixmult_h
#define matrixmult_h

#include <stdio.h>

void matrixmult(double A[3][3], double B[3][3], double C[3][3]);

#endif /* matrixmult_h */
