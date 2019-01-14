//
//  sun.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-06.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sun.c
//%
//% DESCRIPTION:          This function loads sun position for current time
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 6, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       int length_of_file[5]: array with lengths of files
//%                       double sun_eph[length_of_file[3]][3]: sun ephemerides
//%
//% OUTPUT:               double r_sun[3]: position of Sun in inertial frame
//%                         (MOD, assumed to be equal to initial TEME) (m)
//%                       double r: distance from Earth to Sun (m)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sun.h"
#include <math.h>
#include "norm.h"

double sun(double t2000tt, int length_of_file[5], double sun_eph[length_of_file[3]][3], double r_sun[3]){
    
    // Constants
    double au = 149597870700;
    
    int xx = t2000tt/(12*60*60);
    double xrem = fmod(t2000tt,12*60*60.0);
    
    double r0[3], r1[3];
    for (int i=0; i<3; i++){
        r0[i] = sun_eph[xx][i];
        r1[i] = sun_eph[xx+1][i];
    }
    
    // Sun position vector
    for (int i=0; i<3; i++)
        r_sun[i] = ((r1[i]-r0[i])*xrem/(12*60*60.0) + r0[i])*au;
    
    // Earth - Sun distance
    double r = norm(r_sun);
    
    return r;
}
