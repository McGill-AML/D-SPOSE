//
//  quatnormalize.h
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

#ifndef quatnormalize_h
#define quatnormalize_h

#include <stdio.h>

void quatnormalize(double q[4]);

#endif /* quatnormalize_h */
