//
//  wind.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-03-16.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        wind.c
//%
//% DESCRIPTION:          This function will calculate the wind at satellite
//%                       position
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 16, 2017
//% VERSION:              1
//%
//% INPUT:                double p_ecef[3]: position vector in ECEF frame
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF to TEME
//%                       int wind_model: which wind model?
//%
//% OUTPUT:               double winds_i[3]: wind velocity at satellite
//%                         position (m s-1)
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "wind.h"
#include "t2doy.h"
#include <math.h>
#include "crossprod.h"
#include "matxvec.h"

void wind(double p_ecef[3], double LLA[4], double t2000utc, int length_of_file[5], double ap_index[length_of_file[0]], double C_ecef2teme[3][3], int wind_model, double winds_i[3]){

    double winds_ecef[3] = {0,0,0};
    
    // Horizontal Wind Model 2014
    if (wind_model==1){
        
        // Set up input for HWM14
        int time[3];
        float sec = (float) t2doy(t2000utc,time);
        int year = time[0];
        int day = time[1];
        int iyd = 1000*(year-2000)+day;
        float lat = LLA[1]*180/M_PI;    // geodetic latitude
        float lon = LLA[2]*180/M_PI;    // longitude
        float alt = LLA[3];             // km
        
        // Not used
        float stl = 0;
        float f107[2] = {0,0};
        float f107a[2] = {0,0};
        float ap[2] = {0,0};
        
        float w[2];
        
        // Current 3h ap
        int i=day;
        if (sec<3*3600)
            ap[1] = ap_index[(i+2)*8+1];
        else if (sec<6*3600)
            ap[1] = ap_index[(i+2)*8+2];
        else if (sec<9*3600)
            ap[1] = ap_index[(i+2)*8+3];
        else if (sec<12*3600)
            ap[1] = ap_index[(i+2)*8+4];
        else if (sec<15*3600)
            ap[1] = ap_index[(i+2)*8+5];
        else if (sec<18*3600)
            ap[1] = ap_index[(i+2)*8+6];
        else if (sec<21*3600)
            ap[1] = ap_index[(i+2)*8+7];
        else
            ap[1] = ap_index[(i+2)*8+8];
        
        // Horizontal Wind Model 2014
        hwm14_(&iyd,&sec,&alt,&lat,&lon,&stl,&f107a,&f107,&ap,&w);
        double winds_ned[3] = {0,0,0};
        winds_ned[0] = w[0];
        winds_ned[1] = w[1];
        
        // Rotate to inertial frame
        double C_ned2ecef[3][3] = {
            {-sin(LLA[1])*cos(LLA[2]),    -sin(LLA[2]),  -cos(LLA[1])*cos(LLA[2])},
            {-sin(LLA[1])*sin(LLA[2]),    cos(LLA[2]),   -cos(LLA[1])*sin(LLA[2])},
            {cos(LLA[1]),              0,          -sin(LLA[1])}};
        matxvec(C_ned2ecef,winds_ned,winds_ecef);
    }
    
    // Co-rotating Winds (Neglect Polar Motion and Length of Day)
    double thetasa= 7.29211514670698*pow(10,-05);
    double omegaearth[3] = {0, 0, thetasa};
    double w_earth[3];
    crossprod(omegaearth,p_ecef,w_earth);
    for (int i=0; i<3; i++)
        winds_ecef[i] = winds_ecef[i] + w_earth[i];
    matxvec(C_ecef2teme,winds_ecef,winds_i);

}
