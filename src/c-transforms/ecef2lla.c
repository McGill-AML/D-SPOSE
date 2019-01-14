//
//  ecef2lla.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-26.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        ecef2lla.c
//%
//% DESCRIPTION:          This function converts the position vector in ECEF
//%                       to geodetic and geocentric latitude, longitude,
//%                       and height above the ellipsoid (altitude) following
//%                       Vallado (2013) Section 3.4.4
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 26, 2017
//% VERSION:              1
//%
//% INPUT:                double r[3]: position vector in ECEF (m)
//%
//% OUTPUT:               double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%
//% COUPLING:             - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "ecef2lla.h"
#include "norm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void ecef2lla(double r[3], double LLA[4]){
    
    // Constants
    double tolerance = 0.00000001;
    double ee2 = 0.006694385000;
    
    // Longitude
    double rdsat = sqrt(r[0]*r[0]+r[1]*r[1]);
    double sign = 1;
    if (r[2]<0)
        sign = -1;
    double lon;
    if (fabs(rdsat)<tolerance)
        lon= sign*M_PI*0.5;
    else
        lon= atan2(r[1],r[0]);

    // Declination
    if (fabs(lon)>=M_PI){
        if (lon<0.0)
            lon= 2.0*M_PI+lon;
        else
            lon= lon-2.0*M_PI;
    }
    double decl = asin(r[2]/norm(r));
    
    // Iteration to find geodetic latitude
    double latgd = decl;
    double sinlatgd, C;
    for (int i=0; i<10; i++){
        sinlatgd = sin(latgd);
        C = 6378137/(sqrt(1.0-ee2*sinlatgd*sinlatgd));
        latgd= atan((r[2]+C*ee2*sinlatgd)/rdsat);
    }

    // Calculate height
    double alt, S;
    if ((M_PI*0.5-fabs(latgd)) > M_PI/180.0)
        alt = (rdsat/cos(latgd)) - C;
    else {
        S = C*(1.0-ee2);
        alt = r[2]/sin(latgd) - S;
    }
    
    // Geocentric latitude
    double latgc= atan((1.0-ee2)*tan(latgd));
    
    // Output
    LLA[0] = latgc;
    LLA[1] = latgd;
    LLA[2] = lon;
    LLA[3] = alt/1000.0;
    
}
