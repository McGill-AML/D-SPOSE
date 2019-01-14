//
//  transpose.c
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

#include "transpose.h"

void transpose(double A[3][3], double At[3][3]){
    
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++)
            At[i][j] = A[j][i];
    }
    
}
