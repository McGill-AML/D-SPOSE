//
//  quat2dcm.h
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

#ifndef quat2dcm_h
#define quat2dcm_h

#include <stdio.h>

void quat2dcm(double q[4], double dcm[3][3]);

#endif /* quat2dcm_h */
