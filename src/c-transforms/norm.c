//
//  norm.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-29.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        norm.c
//%
//% DESCRIPTION:          This function normalizes a 3x1 vector
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 29, 2016
//% VERSION:              1
//%
//% INPUT:                double v[3]: 3x1 vector
//%
//% OUTPUT:               double v[3]: 3x1 normalized vector
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <math.h>
#include "norm.h"

double norm(double v[3]){
    
    return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    
}
