//
//  crossprod.c
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

#include "crossprod.h"

void crossprod(double A[3], double B[3], double AcrossB[3]){
    AcrossB[0] = A[1]*B[2] - A[2]*B[1];
    AcrossB[1] = A[2]*B[0] - A[0]*B[2];
    AcrossB[2] = A[0]*B[1] - A[1]*B[0];
}
