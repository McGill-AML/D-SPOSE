//
//  invertmat.c
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

#include "invertmat.h"

void invertmat(double A[3][3], double A_inv[3][3]){
    
    double A_det = A[0][0]*(A[1][1]*A[2][2]-A[1][2]*A[2][1])-A[0][1]*(A[1][0]*A[2][2]-A[1][2]*A[2][0])+A[0][2]*(A[1][0]*A[2][1]-A[1][1]*A[2][0]);
    
    double A_adj[3][3] = {
        {A[1][1]*A[2][2]-A[1][2]*A[2][1], -A[0][1]*A[2][2]+A[0][2]*A[2][1], A[0][1]*A[1][2]-A[0][2]*A[1][1]},
        {-A[1][0]*A[2][2]+A[1][2]*A[2][0], A[0][0]*A[2][2]-A[0][2]*A[2][0], -A[0][0]*A[1][2]+A[0][2]*A[1][0]},
        {A[1][0]*A[2][1]-A[1][1]*A[2][0], -A[0][0]*A[2][1]+A[0][1]*A[2][0], A[0][0]*A[1][1]-A[1][0]*A[0][1]}
    };
    
    for (int i = 0; i<3; i++){
        for (int j = 0; j<3; j++){
            A_inv[i][j] = A_adj[i][j]/A_det;
        }
    }
    
}
