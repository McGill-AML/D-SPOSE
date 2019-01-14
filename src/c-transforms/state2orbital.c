//
//  state2orbital.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-06-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        state2orbital.c
//%
//% DESCRIPTION:          This function converts the state parameters to
//%                       orbital parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 June 16, 2016
//% VERSION:              1
//%
//% INPUT:                double R[3]: position vector in inertial frame
//%                       double V[3]: velocity vector in inertial frame
//%
//% OUTPUT:               double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%                         - coe[6]: semi-major axis (m)
//%                         - coe[7]: orbital period (s)
//%                         - coe[8]: mean motion (rad s^-1)
//%
//% COUPLING:             - crossprod.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "state2orbital.h"
#include <math.h>
#include "crossprod.h"
#include "dotprod.h"

void state2orbital(double R[3], double V[3], double coe[9]){
    
    // Gravitational parameter (m^3 s^2)
    double mu = 3.986004418*pow(10,14);
    
    double eps = pow(10,-10);
    
    double r = sqrt(R[0]*R[0]+R[1]*R[1]+R[2]*R[2]);
    double v = sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
    
    double vr = dotprod(R,V)/r;
    
    // Angular momentum (m^2 s^-1)
    double H[3];
    crossprod(R,V,H);
    double h = sqrt(H[0]*H[0]+H[1]*H[1]+H[2]*H[2]);
    
    // Inclination (rad)
    double incl = acos(H[2]/h);
    
    double z[3] = {0,0,1};
    double N[3];
    crossprod(z,H,N);
    double no = sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2]);
    
    // Right ascension of the ascending node (rad)
    double RA;
    if (no != 0){
        RA = acos(N[0]/no);
        if (N[1] < 0)
            RA = 2*M_PI-RA;
    }
    else
        RA = 0;
    if (isnan(RA))
        RA = 0;
    
    // Eccentricity
    double E[3];
    E[0] = 1.0/mu * ((v*v - mu/r)*R[0] - r*vr*V[0]);
    E[1] = 1.0/mu * ((v*v - mu/r)*R[1] - r*vr*V[1]);
    E[2] = 1.0/mu * ((v*v - mu/r)*R[2] - r*vr*V[2]);
    double e = sqrt(E[0]*E[0]+E[1]*E[1]+E[2]*E[2]);
    
    // Argument of Perigee (rad)
    double w;
    if (no != 0) {
        if (e > eps) {
            w = acos(dotprod(N,E)/no/e);
            if (E[2] <0)
                w = 2*M_PI-w;
        }
        else
            w = 0;
    }
    else
        w = 0;
    if (isnan(w))
        w = 0;
    
    // True anomaly (rad)
    double TA, cp[3];
    if (e > eps) {
        TA = acos(dotprod(E,R)/e/r);
        if (vr < 0)
            TA = 2*M_PI - TA;
    }
    else {
        crossprod(N,R,cp);
        if (cp[2] >= 0)
            TA = acos(dotprod(N,R)/no/r);
        else
            TA = 2*M_PI - acos(dotprod(N,R)/no/r);
    }
    if (isnan(TA))
        TA = 0;
    
    // Semimajor Axis (m)
    double a = h*h/mu/(1-e*e);
    
    // Period (s)
    double P = 2*M_PI/sqrt(mu)*pow(a,1.5);
    
    // Mean motion (rad s^-1)
    double n = 2*M_PI/P;
    
    coe[0] = h;
    coe[1] = e;
    coe[2] = RA;
    coe[3] = incl;
    coe[4] = w;
    coe[5] = TA;
    coe[6] = a;
    coe[7] = P;
    coe[8] = n;
}
