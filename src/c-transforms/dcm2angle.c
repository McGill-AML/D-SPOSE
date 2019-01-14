//
//  dcm2angle.c
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

#include "dcm2angle.h"
#include <math.h>

void dcm2angle(double dcm[3][3], double euler[3])
{
    
    //     [          cy*cz,          cy*sz,            -sy]
    //     [ sy*sx*cz-sz*cx, sy*sx*sz+cz*cx,          cy*sx]
    //     [ sy*cx*cz+sz*sx, sy*cx*sz-cz*sx,          cy*cx]
    
    euler[0] = atan2(dcm[0][1], dcm[0][0]);
    euler[1] = asin(-dcm[0][2]);
    euler[2] = atan2(dcm[1][2], dcm[2][2]);
}
