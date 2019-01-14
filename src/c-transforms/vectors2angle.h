//
//  vectors2angle.h
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

#ifndef vectors2angle_h
#define vectors2angle_h

#include <stdio.h>

double vectors2angle(double a[3], double b[3]);

#endif /* vectors2angle_h */
