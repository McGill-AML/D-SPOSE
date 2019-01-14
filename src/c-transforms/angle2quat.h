//
//  angle2quat.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        angle2quat.c
//%
//% DESCRIPTION:          This function converts Euler angles to a quaternion
//%                       using 3-2-1 formulation
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double y: yaw
//%                       double p: pitch
//%                       double r: roll
//%
//% OUTPUT:               double q[4]: 4x1 quaternion vector
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef angle2quat_h
#define angle2quat_h

#include <stdio.h>

void angle2quat(double y, double p, double r, double q[4]);

#endif /* angle2quat_h */
