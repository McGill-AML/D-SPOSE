//
//  teme2ecef.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        teme2ecef.c
//%
//% DESCRIPTION:          This function converts the position and velocity
//%                       vectors from the TEME frame to the ECEF frame
//%                       taking into account the effects of sidereal time
//%                       (GMST) and polar motion following Vallado (2013)
//%                       Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double r_teme[3]: position vector in TEME (m)
//%                       double v_teme[3]: velocity vector in TEME (m s-1)
//%                       double ttt: julian centuries of terrestrial time
//%                       double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double xp: polar motion parameter from EOP
//%                       double yp: polar motion parameter from EOP
//%                       double lod: length of day from EOP
//%
//% OUTPUT:               double r_ecef[3]: position vector in ECEF (m)
//%                       double v_ecef[3]: velocity vector in ECEF (m s-1)
//%                       double C_ecef2teme[3][3]: rotation matrix from
//%                         ECEF to TEME
//%
//% COUPLING:             - matxvec.c
//%                       - transpose.c
//%                       - crossprod.c
//%                       - polarm.c
//%                       - matrixmult.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "teme2ecef.h"
#include <math.h>
#include "matxvec.h"
#include "transpose.h"
#include "crossprod.h"
#include "polarm.h"
#include "matrixmult.h"

void teme2ecef(double r_teme[3], double v_teme[3], double ttt, double t2000ut1, double xp, double yp, double lod, double r_ecef[3], double v_ecef[3], double C_ecef2teme[3][3]){
    
    // Calculate GMST from J2000+dUT1
    double gmst;
    double tut1 = (t2000ut1-(12*60*60.0))/(60*60*24*36525.0);
    double temp = - 6.2e-6 * tut1 * tut1 * tut1 + 0.093104 * tut1 * tut1 + (876600.0 * 3600.0 + 8640184.812866) * tut1 + 67310.54841;
    temp = fmod( temp*M_PI/180.0/240.0, 2*M_PI );
    if (temp<0.0)
        temp = temp+2*M_PI;
    gmst = temp;
    
    // Rotation matrix from PEF to TEME considering sidereal time with GMST
    double st[3][3], st_t[3][3];
    st[0][0] =  cos(gmst);
    st[0][1] = -sin(gmst);
    st[0][2] =  0.0;
    st[1][0] =  sin(gmst);
    st[1][1] =  cos(gmst);
    st[1][2] =  0.0;
    st[2][0] =  0.0;
    st[2][1] =  0.0;
    st[2][2] =  1.0;
    transpose(st,st_t);
    
    // Rotation matrix from ECEF to PEF considering polar motion
    double pm[3][3], pm_t[3][3];
    polarm(xp,yp,ttt,pm);
    transpose(pm,pm_t);
    
    // Earth angular velocity
    double thetasa    = 7.29211514670698e-05 * (1.0  - lod/86400.0 );
    double omegaearth[3] = {0, 0, thetasa};
    
    // Position transformation
    double r_pef[3];
    matxvec(st_t,r_teme,r_pef);
    matxvec(pm_t,r_pef,r_ecef);
    matrixmult(st, pm, C_ecef2teme);
    
    // Velocity transformation
    double v_pef[3], v_temp[3];
    matxvec(st_t,v_teme,v_pef);
    crossprod(omegaearth,r_pef,v_temp);
    for (int i=0; i<3; i++)
        v_pef[i] = v_pef[i] - v_temp[i];
    matxvec(pm_t,v_pef,v_ecef);
    
}
