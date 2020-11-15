//
//  get_density.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-10-24.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        get_density.c
//%
//% DESCRIPTION:          This function will obtain the atmospheric density
//%                       at the satellite position from desired model
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 24, 2016
//% VERSION:              1
//%
//% INPUT:                double int atmos_model: which atmospheric model?
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double solar_input[length_of_file[1]][3]: F10.7 array for NRLMSISE-00
//%                       double Apc: user-inputted Ap value
//%                       double F107c: user-inputted F10.7 value
//%
//% OUTPUT:               double density: atmospheric density value at satellite
//%                         position (kg m-3)
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - nrlmsise-00.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "get_density.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "t2doy.h"
#include "crossprod.h"
#include "nrlmsise-00.h"

extern void solfsmy_(double *T1950, double *F10, double *F10B, double *S10, double *S10B, double *XMXX, double *XMXXB, double *XYXX, double *XYXXB);
extern void dtcval_(double *D1950, int *IDTCVAL);
extern void theta_(double *xD1950, double *GWRAS);
extern void sunpos_(double *AMJD, double *SOLRAS, double *SOLDEC);
extern void jb2008_(double *AMJD, double SUN[2], double SAT[3], double *F10, double *F10B, double *S10, double *S10B, double *XM10, double *XM10B, double *Y10, double *Y10B, double *DSTDTC, double TEMP[2], double *RHO);

double get_density(int atmos_model, double LLA[4], double t2000utc, int length_of_file[5], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double Apc, double F107c){
    
    int day, year, time[3];
    double sec = t2doy(t2000utc,time);
    
    double density_mod;
    
    /////////////////////////////////
    ////////// NRLMSISE-00 //////////
    /////////////////////////////////
    if (atmos_model==1){
        
        year = time[0];
        day = time[2];
        
        struct nrlmsise_output output;
        struct nrlmsise_input input;
        struct nrlmsise_flags flags;
        struct ap_array aph;
        
        /* input values */
        flags.switches[0]=0;
        for (int i=1;i<24;i++)
            flags.switches[i]=1;
        
        input.year=0; /* without effect */
        input.doy = time[1];
        input.f107 = solar_input[day-1][0];
        input.f107A = solar_input[day-1][1];
        input.ap = solar_input[day-1][2];
        input.g_lat=LLA[1]*180/M_PI;    // Geodetic (deg)
        input.g_long=LLA[2]*180/M_PI;   // (deg)
        input.alt=LLA[3];               // (km)
        input.sec = sec;
        input.lst=sec/3600.0 + LLA[2]*12/M_PI;
        
        // If constant F10.7
        if (F107c!=0){
            input.f107 = F107c;
            input.f107A = F107c;
        }
        
        if (Apc==0){
            int i=day;
            aph.a[0] = solar_input[i-1][2];
            if (sec<3*3600){
                aph.a[1] = ap_index[(i+2)*8+1];
                aph.a[2] = ap_index[(i+2)*8];
                aph.a[3] = ap_index[(i+2)*8-1];
                aph.a[4] = ap_index[(i+2)*8-2];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-14]+ap_index[(i+2)*8-15]+ap_index[(i+2)*8-16]+ap_index[(i+2)*8-17]+ap_index[(i+2)*8-18])/8.0;
            }
            else if (sec<6*3600){
                aph.a[1] = ap_index[(i+2)*8+2];
                aph.a[2] = ap_index[(i+2)*8+1];
                aph.a[3] = ap_index[(i+2)*8];
                aph.a[4] = ap_index[(i+2)*8-1];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-14]+ap_index[(i+2)*8-15]+ap_index[(i+2)*8-16]+ap_index[(i+2)*8-17]+ap_index[(i+2)*8-10])/8.0;
            }
            else if (sec<9*3600){
                aph.a[1] = ap_index[(i+2)*8+3];
                aph.a[2] = ap_index[(i+2)*8+2];
                aph.a[3] = ap_index[(i+2)*8+1];
                aph.a[4] = ap_index[(i+2)*8];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-14]+ap_index[(i+2)*8-15]+ap_index[(i+2)*8-16]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
            else if (sec<12*3600){
                aph.a[1] = ap_index[(i+2)*8+4];
                aph.a[2] = ap_index[(i+2)*8+3];
                aph.a[3] = ap_index[(i+2)*8+2];
                aph.a[4] = ap_index[(i+2)*8+1];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-14]+ap_index[(i+2)*8-15]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
            else if (sec<15*3600){
                aph.a[1] = ap_index[(i+2)*8+5];
                aph.a[2] = ap_index[(i+2)*8+4];
                aph.a[3] = ap_index[(i+2)*8+3];
                aph.a[4] = ap_index[(i+2)*8+2];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8+1]+ap_index[(i+2)*8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-14]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
            else if (sec<18*3600){
                aph.a[1] = ap_index[(i+2)*8+6];
                aph.a[2] = ap_index[(i+2)*8+5];
                aph.a[3] = ap_index[(i+2)*8+4];
                aph.a[4] = ap_index[(i+2)*8+3];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8+2]+ap_index[(i+2)*8+1]+ap_index[(i+2)*8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-13]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
            else if (sec<21*3600){
                aph.a[1] = ap_index[(i+2)*8+7];
                aph.a[2] = ap_index[(i+2)*8+6];
                aph.a[3] = ap_index[(i+2)*8+5];
                aph.a[4] = ap_index[(i+2)*8+4];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8+3]+ap_index[(i+2)*8+2]+ap_index[(i+2)*8+1]+ap_index[(i+2)*8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-12]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
            else{
                aph.a[1] = ap_index[(i+2)*8+8];
                aph.a[2] = ap_index[(i+2)*8+7];
                aph.a[3] = ap_index[(i+2)*8+6];
                aph.a[4] = ap_index[(i+2)*8+5];
                aph.a[5] = (ap_index[(i+2)*8-3]+ap_index[(i+2)*8+4]+ap_index[(i+2)*8+3]+ap_index[(i+2)*8+2]+ap_index[(i+2)*8+1]+ap_index[(i+2)*8]+ap_index[(i+2)*8-1]+ap_index[(i+2)*8-2])/8.0;
                aph.a[6] = (ap_index[(i+2)*8-11]+ap_index[(i+2)*8-4]+ap_index[(i+2)*8-5]+ap_index[(i+2)*8-6]+ap_index[(i+2)*8-7]+ap_index[(i+2)*8-8]+ap_index[(i+2)*8-9]+ap_index[(i+2)*8-10])/8.0;
            }
        }
        else {
            input.ap = Apc;
            aph.a[0] = Apc;
            aph.a[1] = Apc;
            aph.a[2] = Apc;
            aph.a[3] = Apc;
            aph.a[4] = Apc;
            aph.a[5] = Apc;
            aph.a[6] = Apc;
        }
        input.ap_a=&aph;
        
        // Obtain density
        gtd7(&input, &flags, &output);
        double densityNRL = output.d[5]*1000; // Convert to kg m-3
        
        density_mod = densityNRL;
    }
    
    ////////////////////////////
    ////////// JB2008 //////////
    ////////////////////////////
    if (atmos_model==2){
        
        double sec_year = sec;
        year = time[0];
        day = time[1];
        
        int hour = floor(sec_year/(60*60.0));
        int min = floor((sec_year-hour*60*60)/60.0);
        sec = sec_year - hour*60*60 - min*60;
        
        double alti = LLA[3];           // (km)
        double xlat = LLA[0]*180/M_PI;  // Geocentric (deg)
        double xlon = LLA[2]*180/M_PI;  // (deg)
        
        double F10, F10B, S10, S10B, XM10, XM10B, Y10, Y10B;
        double XMXX, XMXXB, XYXX, XYXXB, XFXX, XFXXB, XSXX, XSXXB;
        double SUN[2], SAT[3];
        double TEMP[2], RHO;
        double AMJD, D1950, T1950;
        
        int lag1 = 1;
        double dlag;
        while (lag1) {
            lag1 = 0;
            
            if (year > 1000)
                year = year - 1900;
            if (year < 50)
                year = year + 100;
            int iyy = ((year-1)/4.0-12);
            iyy = (year-50)*365 + iyy;
            D1950 = iyy + day + hour/24.0 + min/1440.0 + sec/86400.0;
            AMJD = D1950 + 33281.0;
            
            // F10 and S10
            dlag = 1;
            T1950 = D1950 - dlag;
            solfsmy_(&T1950,&F10,&F10B,&S10,&S10B,&XMXX,&XMXXB,&XYXX,&XYXXB);
            if (F10<40)
                lag1=1;
            if (F10B<40)
                lag1=1;
            if (S10<40)
                lag1=1;
            if (S10B<40)
                lag1=1;
            
            // M10
            dlag = 2;
            T1950 = D1950 - dlag;
            solfsmy_(&T1950,&XFXX,&XFXXB,&XSXX,&XSXXB,&XM10,&XM10B,&XYXX,&XYXXB);
            if (XM10<40)
                lag1=1;
            if (XM10B<40)
                lag1=1;
            
            // Y10
            dlag = 5;
            T1950 = D1950 - dlag;
            solfsmy_(&T1950,&XFXX,&XFXXB,&XSXX,&XSXXB,&XMXX,&XMXXB,&Y10,&Y10B);
            if (Y10<40)
                lag1=1;
            if (Y10B<40)
                lag1=1;
            
        }
        
        // Geomagnetic storm DTC value
        double DSTDTC;
        int IDTCVAL;
        dtcval_(&D1950,&IDTCVAL);
        DSTDTC = IDTCVAL;
        
        double GWRAS;
        theta_(&D1950,&GWRAS);
        double mod = floor((GWRAS + xlon*M_PI/180.0 + 2*M_PI) / (2*M_PI));
        SAT[0] = (GWRAS + xlon*M_PI/180.0 + 2*M_PI) - mod*2*M_PI;
        SAT[1] = xlat*M_PI/180.0;
        SAT[2] = alti;
        
        double SOLRAS, SOLDEC;
        sunpos_(&AMJD,&SOLRAS,&SOLDEC);
        SUN[0] = SOLRAS;
        SUN[1] = SOLDEC;
        
        // Obtain density
        jb2008_(&AMJD,SUN,SAT,&F10,&F10B,&S10,&S10B,&XM10,&XM10B,&Y10,&Y10B,&DSTDTC,TEMP,&RHO);
        double densityJB = RHO; //
        
        density_mod = densityJB;
    }
    
    return density_mod;
}
