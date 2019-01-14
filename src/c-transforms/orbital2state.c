//
//  orbital2state.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        orbital2state.c
//%
//% DESCRIPTION:          This function converts the orbital parameters to
//%                       state parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 6, 2016
//% VERSION:              1
//%
//% INPUT:                double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%
//% OUTPUT:               double R[3]: position vector in inertial frame
//%                       double V[3]: velocity vector in inertial frame
//%
//% COUPLING:             - matrixmult.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "orbital2state.h"
#include "matrixmult.h"
#include "matxvec.h"
#include <math.h>

void orbital2state(double R[3], double V[3], double coe[9]){

    // Gravitational parameter (m^3 s^2)
    double mu = 3.986004418*pow(10,14);
    
    // Input orbital parameters
    double h = coe[0];
    double e = coe[1];
    double RA = coe[2];
    double incl = coe[3];
    double w = coe[4];
    double TA = coe[5];
    
    double rp[3];
    rp[0] = (h*h/mu)* (1/(1 + e*cos(TA))) * (cos(TA)*1 + sin(TA)*0);
    rp[1] = (h*h/mu)* (1/(1 + e*cos(TA))) * (cos(TA)*0 + sin(TA)*1);
    rp[2] = (h*h/mu)* (1/(1 + e*cos(TA))) * (cos(TA)*0 + sin(TA)*0);
    
    double vp[3];
    vp[0] = (mu/h) * (-sin(TA)*1 + (e + cos(TA))*0);
    vp[1] = (mu/h) * (-sin(TA)*0 + (e + cos(TA))*1);
    vp[2] = (mu/h) * (-sin(TA)*0 + (e + cos(TA))*0);
    
    double R3_W[3][3] = {{cos(RA),-sin(RA),0},{sin(RA),cos(RA),0},{0,0,1}};
    
    double R1_i[3][3] = {{1,0,0},{0,cos(incl),-sin(incl)},{0,sin(incl),cos(incl)}};
    
    double R3_w[3][3] = {{cos(w),-sin(w),0},{sin(w),cos(w),0},{0,0,1}};
    
    double temp[3][3], Q_pX[3][3];
    matrixmult(R3_W,R1_i,temp);
    matrixmult(temp,R3_w,Q_pX);
    
    matxvec(Q_pX,rp,R);
    matxvec(Q_pX,vp,V);
}
