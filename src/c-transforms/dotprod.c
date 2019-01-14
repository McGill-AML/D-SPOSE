//
//  dotprod.c
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

#include "dotprod.h"

double dotprod(double A[3], double B[3]){
    return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}
