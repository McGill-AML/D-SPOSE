//
//  ecef2eci.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        ecef2eci.c
//%
//% DESCRIPTION:          This function converts the position and velocity
//%                       vectors from the ECEF frame to the ECI frame
//%                       taking into account the effects of sidereal time,
//%                       polar motion, nutation, and precession (IAU-76/FK5)
//%                       following Vallado (2013) Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double r_teme[3]: position vector in TEME (m)
//%                       double v_teme[3]: velocity vector in TEME (m s-1)
//%                       double ttt: julian centuries of terrestrial time
//%                       double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double iar80[106][5]: nutation parameters
//%                       double rar80[106][4]: nutation parameters
//%                       double xp: polar motion parameter from EOP
//%                       double yp: polar motion parameter from EOP
//%                       double lod: length of day from EOP
//%                       double dpsi: EOP corrections
//%                       double deps: EOP corrections
//%
//% OUTPUT:               double r_eci[3]: position vector in ECI (m)
//%                       double v_eci[3]: velocity vector in ECI (m s-1)
//%                       double C_ecef2eci[3][3]: rotation matrix from
//%                         ECEF to ECI
//%
//% COUPLING:             - precess.c
//%                       - nutation.c
//%                       - matxvec.c
//%                       - sidereal.c
//%                       - crossprod.c
//%                       - polarm.c
//%                       - matrixmult.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "ecef2eci.h"
#include "precess.h"
#include "nutation.h"
#include "matxvec.h"
#include "sidereal.h"
#include "crossprod.h"
#include "polarm.h"
#include "matrixmult.h"

void ecef2eci(double r_ecef[3], double v_ecef[3], double ttt, double t2000ut1, double iar80[106][5], double rar80[106][4], double xp, double yp, double lod, double dpsi, double deps, double r_eci[3], double v_eci[3], double C_ecef2eci[3][3]){
    
    // Rotation matrix from ECEF to PEF
    double pm[3][3];
    polarm(xp,yp,ttt,pm);
    
    // Earth angular velocity
    double thetasa   = 7.29211514670698e-05 * (1.0  - lod/86400.0 );
    double omegaearth[3] = {0, 0, thetasa};
    
    // Precession MOD to ECI
    double prec[3][3];
    precess(ttt, prec);
    
    // Nutation TOD to MOD
    double nut[3][3], nut_param[3];
    nutation(ttt, iar80, rar80, dpsi, deps, nut_param, nut);
    
    // Sidereal Time PEF to TOD
    double st[3][3];
    sidereal(t2000ut1, nut_param[0], nut_param[1], nut_param[2], st);
    
    // Position transformation
    double r_tod[3], r_mod[3], r_pef[3];
    matxvec(pm,r_ecef,r_pef);
    matxvec(st,r_pef,r_tod);
    matxvec(nut,r_tod,r_mod);
    matxvec(prec,r_mod,r_eci);
    
    // Velocity transformation
    double v_tod[3], v_mod[3], v_pef[3], v_temp[3], temp[3];
    matxvec(pm,v_ecef,v_pef);
    crossprod(omegaearth,r_pef,temp);
    for (int i=0; i<3; i++)
        v_temp[i] = v_pef[i] + temp[i];
    matxvec(st,v_temp,v_tod);
    matxvec(nut,v_tod,v_mod);
    matxvec(prec,v_mod,v_eci);
    
    // ECEF to ECI Rotation Matrix
    double C_ecef2mod[3][3], C_ecef2tod[3][3];
    matrixmult(st,pm,C_ecef2tod);
    matrixmult(nut,C_ecef2tod,C_ecef2mod);
    matrixmult(prec,C_ecef2mod,C_ecef2eci);
}
