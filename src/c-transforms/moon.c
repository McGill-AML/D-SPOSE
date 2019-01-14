//
//  moon.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-06.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        moon.c
//%
//% DESCRIPTION:          This function loads moon position for current time
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 6, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       int length_of_file[5]: array with lengths of files
//%                       double moon_eph[length_of_file[4]][3]: moon ephemerides
//%
//% OUTPUT:               double r_moon[3]: position of Moon in inertial frame
//%                         (MOD, assumed to be equal to initial TEME) (m)
//%                       double r: distance from Earth to Moon (m)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "moon.h"
#include <math.h>
#include "norm.h"

double moon(double t2000tt, int length_of_file[5], double moon_eph[length_of_file[4]][3], double r_moon[3]){
    
    // Constants
    double au = 149597870700;
    
    int xx = t2000tt/(4*60*60);
    double xrem = fmod(t2000tt,4*60*60.0);
    
    double r0[3], r1[3];
    for (int i=0; i<3; i++){
        r0[i] = moon_eph[xx][i];
        r1[i] = moon_eph[xx+1][i];
    }
    
    // Moon position vector
    for (int i=0; i<3; i++)
    r_moon[i] = ((r1[i]-r0[i])*xrem/(4*60*60.0) + r0[i])*au;
    
    // Earth - Moon distance
    double r = norm(r_moon);
    
    return r;
}
