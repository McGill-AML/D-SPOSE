//
//  quat2dcm.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        quat2dcm.c
//%
//% DESCRIPTION:          This function converts a quaternion to a rotation
//%                       matrix
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double q[4]: 4x1 quaternion vector
//%
//% OUTPUT:               double dcm[3][3]: 3x3 rotation matrix
//%
//% COUPLING:             - quatnormalize.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "quat2dcm.h"
#include "quatnormalize.h"
#include <math.h>

void quat2dcm(double q[4], double dcm[3][3])
{
    quatnormalize(q);
    
    dcm[0][0] = pow(q[0],2) + pow(q[1],2) - pow(q[2],2) - pow(q[3],2);
    dcm[0][1] = 2*(q[1]*q[2] + q[0]*q[3]);
    dcm[0][2] = 2*(q[1]*q[3] - q[0]*q[2]);
    dcm[1][0] = 2*(q[1]*q[2] - q[0]*q[3]);
    dcm[1][1] = pow(q[0],2) - pow(q[1],2) + pow(q[2],2) - pow(q[3],2);
    dcm[1][2] = 2*(q[2]*q[3] + q[0]*q[1]);
    dcm[2][0] = 2*(q[1]*q[3] + q[0]*q[2]);
    dcm[2][1] = 2*(q[2]*q[3] - q[0]*q[1]);
    dcm[2][2] = pow(q[0],2) - pow(q[1],2) - pow(q[2],2) + pow(q[3],2);

}
