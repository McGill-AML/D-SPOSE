//
//  matxvec.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-03-02.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        matxvec.c
//%
//% DESCRIPTION:          This function multiplies a 3x3 matrix by a 3x1 vector
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 2, 2016
//% VERSION:              1
//%
//% INPUT:                double A[3][3]: 3x3 matrix
//%                       double b[3]: 3x1 vector
//%
//% OUTPUT:               double Axb[3]: 3x1 vector obtained from multiplying Ab
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef matxvec_h
#define matxvec_h

#include <stdio.h>

void matxvec(double A[3][3], double b[3], double Axb[3]);

#endif /* matxvec_h */
