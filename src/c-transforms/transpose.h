//
//  transpose.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        transpose.c
//%
//% DESCRIPTION:          This function transposes a 3x3 matrix
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double A[3][3]: 3x3 matrix
//%
//% OUTPUT:               double At[3][3]: 3x3 matrix
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef transpose_h
#define transpose_h

#include <stdio.h>

void transpose(double A[3][3], double At[3][3]);

#endif /* transpose_h */
