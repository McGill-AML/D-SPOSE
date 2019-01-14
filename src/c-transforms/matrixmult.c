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

#include "matrixmult.h"

void matrixmult(double A[3][3], double B[3][3], double C[3][3]){
    
    int i,j,k;
    
    for(i=0; i<3; ++i)
    {
        for(j=0; j<3; ++j)
        {
            C[i][j]=0;
        }
    }

    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            for (k=0;k<3;k++)
            {
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
    
}
