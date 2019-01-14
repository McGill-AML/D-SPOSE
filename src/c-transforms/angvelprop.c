//
//  angvelprop.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        angvelprop.c
//%
//% DESCRIPTION:          This function propagates angular velocity
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double Inertia[3][3]: inertia matrix
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double w[3]: angular velocity vector
//%                       double g[3]: sum of torques vector
//%
//% OUTPUT:               double dw[3]: angular velocity update
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "angvelprop.h"

void angvelprop(double Inertia[3][3], double I_inv[3][3], double w[3], double g[3], double dw[3]){
    
    // dw/dt = I^-1 (-w x Iw) + g
    
    double Iw[3] = {
        Inertia[0][0]*w[0]+Inertia[0][1]*w[1]+Inertia[0][2]*w[2],
        Inertia[1][0]*w[0]+Inertia[1][1]*w[1]+Inertia[1][2]*w[2],
        Inertia[2][0]*w[0]+Inertia[2][1]*w[1]+Inertia[2][2]*w[2]
    };
    
    double cwIwg[3] = {
        -w[1]*Iw[2]+w[2]*Iw[1]+g[0],
        w[0]*Iw[2]-w[2]*Iw[0]+g[1],
        -w[0]*Iw[1]+w[1]*Iw[0]+g[2]
    };
    
    dw[0] = I_inv[0][0]*cwIwg[0]+I_inv[0][1]*cwIwg[1]+I_inv[0][2]*cwIwg[2];
    dw[1] = I_inv[1][0]*cwIwg[0]+I_inv[1][1]*cwIwg[1]+I_inv[1][2]*cwIwg[2];
    dw[2] = I_inv[2][0]*cwIwg[0]+I_inv[2][1]*cwIwg[1]+I_inv[2][2]*cwIwg[2];
    
}
