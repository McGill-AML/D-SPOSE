//
//  quatnormalize.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        quatnormalize.c
//%
//% DESCRIPTION:          This function normalizes a quaternion
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double q[4]: 4x1 quaternion vector
//%
//% OUTPUT:               double q[4]: 4x1 normalized quaternion vector
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "quatnormalize.h"
#include <math.h>

void quatnormalize(double q[4])
{
    double norm = sqrt(pow(q[0],2)+pow(q[1],2)+pow(q[2],2)+pow(q[3],2));
    for (int i = 0; i<4; i++){
        q[i] = q[i]/norm;
    }
    
}
