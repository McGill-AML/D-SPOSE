//
//  tt2utc.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-10.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        tt2utc.c
//%
//% DESCRIPTION:          This function converts terrestrial time to UTC by
//%                       going through atomic time and including leap seconds
//%                       (last leap second included is December 31 2016)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 10, 2017
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%
//% OUTPUT:               double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "tt2utc.h"

double tt2utc(double t2000tt){
    
    // TT to TAI
    double t2000tai = t2000tt - 32.184;
    // Leap seconds until 2000
    double t2000utc = t2000tai - 32;
    if (t2000utc >= (2*366+4*365)*60*60*24)
        t2000utc = t2000utc-1;
    if (t2000utc >= (3*366+6*365)*60*60*24)
        t2000utc = t2000utc-1;
    if (t2000utc >= (3*366+9*365+182)*24*60*60)
        t2000utc = t2000utc-1;
    if (t2000utc >= (4*366+11*365+181)*24*60*60)
        t2000utc = t2000utc-1;
    if (t2000utc >= (5*366+12*365)*24*60*60)
        t2000utc = t2000utc-1;
    
    return t2000utc;
    
}
