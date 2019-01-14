//
//  angle2quat.c
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

#include "angle2quat.h"
#include <math.h>

void angle2quat(double y, double p, double r, double q[4])
{
    double angles[3] = {y, p, r};
    
    double cang[3];
    for (int i = 0; i<4; i++)
    {
        cang[i] = cos(angles[i]/2.0);
    }
    double sang[3];
    for (int i = 0; i<4; i++)
    {
        sang[i] = sin(angles[i]/2.0);
    }
    
    q[0] = cang[0]*cang[1]*cang[2]+sang[0]*sang[1]*sang[2];
    q[1] =     cang[0]*cang[1]*sang[2]-sang[0]*sang[1]*cang[2];
    q[2] =     cang[0]*sang[1]*cang[2]+sang[0]*cang[1]*sang[2];
    q[3] =     sang[0]*cang[1]*cang[2]-cang[0]*sang[1]*sang[2];
    
}
