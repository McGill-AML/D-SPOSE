//
//  dotprod.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-03-02.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        dotprod.c
//%
//% DESCRIPTION:          This function performs a dot product
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 2, 2016
//% VERSION:              1
//%
//% INPUT:                double A[3]: 3x1 vector
//%                       double B[3]: 3x1 vector
//%
//% OUTPUT:               double: A . B
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef dotprod_h
#define dotprod_h

#include <stdio.h>

double dotprod(double A[3], double B[3]);

#endif /* dotprod_h */
