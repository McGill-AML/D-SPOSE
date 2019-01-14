//
//  dcm2angle.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        dcm2angle.c
//%
//% DESCRIPTION:          This function converts a rotation matrix to a
//%                       Euler angles using 3-2-1 formulation
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double dcm[3][3]: 3x3 rotation matrix
//%
//% OUTPUT:               double euler[3]:
//%                         - euler[0]: yaw
//%                         - euler[1]: pitch
//%                         - euler[2]: roll
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef dcm2angle_h
#define dcm2angle_h

#include <stdio.h>

void dcm2angle(double dcm[3][3], double euler[3]);

#endif /* dcm2angle_h */
