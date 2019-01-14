//
//  sidereal.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sidereal.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       PEF frame to the TOD frame
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000ut1: seconds since January 1, 2000, 00:00:00 UT1
//%                       double deltapsi: nutation angle
//%                       double meaneps: mean obliquity of the ecliptic
//%                       double omega: Delaunay parameter Omega
//%
//% OUTPUT:               double st[3][3]: rotation matrix from PEF to TOD
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sidereal.h"
#include <math.h>

void sidereal(double t2000ut1, double deltapsi, double meaneps, double omega, double st[3][3]){
    
    // GMST from J2000 UT1
    double gmst;
    double tut1 = (t2000ut1-(12*60*60.0))/(60*60*24*36525.0);
    double temp = - 6.2e-6 * tut1 * tut1 * tut1 + 0.093104 * tut1 * tut1 + (876600.0 * 3600.0 + 8640184.812866) * tut1 + 67310.54841;
    temp = fmod( temp*M_PI/180.0/240.0, 2*M_PI );
    if (temp<0.0)
        temp = temp+2*M_PI;
    gmst = temp;
    
    // Apparent time
    double ast= gmst + deltapsi* cos(meaneps) + 0.00264*M_PI /(3600*180)*sin(omega) + 0.000063*M_PI /(3600*180)*sin(2.0 *omega);
    
    ast = fmod(ast,2*M_PI);
    
    // Rotation matrix from PEF to TOD
    st[0][0] =  cos(ast);
    st[0][1] = -sin(ast);
    st[0][2] =  0.0;
    st[1][0] =  sin(ast);
    st[1][1] =  cos(ast);
    st[1][2] =  0.0;
    st[2][0] =  0.0;
    st[2][1] =  0.0;
    st[2][2] =  1.0;
    
}
