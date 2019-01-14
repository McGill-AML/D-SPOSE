//
//  grav_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        grav_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the Earth
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int l_max: maximum order and degree in spherical
//%                         harmonic expansion
//%
//% OUTPUT:               double U: potential energy due to Earth (J)
//%
//% COUPLING:             - tt2utc.c
//%                       - teme2ecef.c
//%                       - ecef2lla.c
//%                       - t2doy.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "grav_potential.h"
#include <math.h>
#include "tt2utc.h"
#include "teme2ecef.h"
#include "ecef2lla.h"
#include "t2doy.h"

double grav_potential(double t2000tt, double p[3], double v[3], double m, double C[101][101], double S[101][101], int l_max){
    
    // TT to UTC
    double t2000utc;
    t2000utc = tt2utc(t2000tt);
    double ttt = (t2000tt-(12*60*60.0))/(60*60*24*36525.0);   // julian centuries of TT
    
    // Day of Year UTC
    int time[3];
    double sec = t2doy(t2000utc, time);
    
    // Get Earth Orientation Parameters
    int d2000 = time[2];   // Day Number starting January 1, 2000
    double dut1 = 0;//eop[d2000-1][6];
    double t2000ut1 = t2000utc + dut1;
    double xp = 0; // TEME to ECEF only considering GMST sidereal time
    double yp = 0; // TEME to ECEF only considering GMST sidereal time
    double lod = 0; // TEME to ECEF only considering GMST sidereal time
    
    // Get position in ECEF frame and LLA coordinates
    double p_ecef[3], v_ecef[3], C_ecef2teme[3][3], LLA[4];
    teme2ecef(p, v, ttt, t2000ut1, xp, yp, lod, p_ecef, v_ecef, C_ecef2teme);
    ecef2lla(p_ecef, LLA);
    
    // Constants
    double mu = 3986004.418*pow(10,8);
    double a = 6378136.3;
    
    // Spherical geocentric distance, longitude and latitude (declination) (m and rad)
    double r = sqrt(p_ecef[0]*p_ecef[0] + p_ecef[1]*p_ecef[1] + p_ecef[2]*p_ecef[2]);
    double lon = LLA[2];
    double lat = asin(p_ecef[2]/sqrt(p_ecef[0]*p_ecef[0]+p_ecef[1]*p_ecef[1]+p_ecef[2]*p_ecef[2]));
    
    // Legendre Polynomials
    double P[103][103];
    P[0][0] = 1;
    P[1][0] = sin(lat);
    P[1][1] = cos(lat);
    for (int l = 0; l <=l_max; l++){
        P[l][l+1] = 0;
        P[l][l+2] = 0;
    }
    for (int l = 2; l <= l_max+2 ; ++l ){
        P[l][0] = ((2*l-1)*sin(lat)*P[l-1][0]-(l-1)*P[l-2][0])/((double)l);
        for (int m = 1; m<l ; ++m )
            P[l][m] = P[l-2][m]+(2*l-1)*cos(lat)*P[l-1][m-1];
        P[l][l] = (2*l-1)*cos(lat)*P[l-1][l-1];
    }

    double U = 1; // Takes into account spherical term.
    
    // Potential function
    for (int l = 2; l <=l_max ; ++l ){
        for (int m = 0; m<=l; m++){
            U = U + pow(a/r,l)*P[l][m]*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
        }
    }
    U = -U*mu*m/r;
    
    return U;
}
