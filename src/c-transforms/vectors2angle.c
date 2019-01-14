//
//  vectors2angle.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-18.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        vectors2angle.c
//%
//% DESCRIPTION:          This function calculates the minimum angle between
//%                       two vectors
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 18, 2016
//% VERSION:              1
//%
//% INPUT:                double a[3]: 3x1 vector
//%                       double b[3]: 3x1 vector
//%
//% OUTPUT:               double alpha: minimum angle between two vectors (rad)
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "vectors2angle.h"
#include <math.h>

double vectors2angle(double a[3], double b[3]){
    
    double alpha, cross_a_b[3], norm_cross, dot_a_b;
    
    cross_a_b[0] = a[1]*b[2]-a[2]*b[1];
    cross_a_b[1] = a[2]*b[0]-a[0]*b[2];
    cross_a_b[2] = a[0]*b[1]-a[1]*b[0];
    
    norm_cross = sqrt(cross_a_b[0]*cross_a_b[0]+cross_a_b[1]*cross_a_b[1]+cross_a_b[2]*cross_a_b[2]);
    
    dot_a_b = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
    
    return alpha = atan2(norm_cross, dot_a_b);
}
