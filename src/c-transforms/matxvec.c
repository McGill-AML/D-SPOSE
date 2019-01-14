//
//  matxvec.c
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

#include "matxvec.h"

void matxvec(double A[3][3], double b[3], double Axb[3]){
    
    Axb[0] = A[0][0]*b[0] + A[0][1]*b[1] + A[0][2]*b[2];
    Axb[1] = A[1][0]*b[0] + A[1][1]*b[1] + A[1][2]*b[2];
    Axb[2] = A[2][0]*b[0] + A[2][1]*b[1] + A[2][2]*b[2];
    
}
