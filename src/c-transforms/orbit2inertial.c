//
//  orbit2inertial.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        orbit2inertial.c
//%
//% DESCRIPTION:          This function converts a position and velocity
//%                       vector to a rotation matrix rotating from the
//%                       orbital frame to the inertial frame
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 16, 2016
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m s-1)
//%
//% OUTPUT:               double C_o2i[3][3]: rotation matrix from orbital
//%                         frame to inertial frame
//%
//% COUPLING:             - crossprod.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "orbit2inertial.h"
#include "crossprod.h"
#include <math.h>
#include "dotprod.h"

void orbit2inertial(double p[3], double v[3], double C_o2i[3][3]){
    
    double i[3] = {1,0,0}, j[3] = {0,1,0}, k[3] = {0,0,1}, x[3], y[3], z[3];
    
    x[0] = v[0]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    x[1] = v[1]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    x[2] = v[2]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    
    double vxp[3];
    crossprod(v,p,vxp);
    y[0] = vxp[0]/sqrt(vxp[0]*vxp[0]+vxp[1]*vxp[1]+vxp[2]*vxp[2]);
    y[1] = vxp[1]/sqrt(vxp[0]*vxp[0]+vxp[1]*vxp[1]+vxp[2]*vxp[2]);
    y[2] = vxp[2]/sqrt(vxp[0]*vxp[0]+vxp[1]*vxp[1]+vxp[2]*vxp[2]);
    
    crossprod(x,y,z);
    
    C_o2i[0][0] = dotprod(i,x);
    C_o2i[0][1] = dotprod(i,y);
    C_o2i[0][2] = dotprod(i,z);
    C_o2i[1][0] = dotprod(j,x);
    C_o2i[1][1] = dotprod(j,y);
    C_o2i[1][2] = dotprod(j,z);
    C_o2i[2][0] = dotprod(k,x);
    C_o2i[2][1] = dotprod(k,y);
    C_o2i[2][2] = dotprod(k,z);
}
