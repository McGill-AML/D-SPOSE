//
//  magnet_field.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        magnetic_field.c
//%
//% DESCRIPTION:          This function will calculate the magnetic field
//%                       vector and its time derivative at the satellite position
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 11, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000utc: seconds since January 1, 2000, 00:00:00 UTC
//%                       double p[3]: position in TEME frame (m)
//%                       double p_LLA[3]: position in ECEF frame (m)
//%                       double v_LLA[3]: velocity in ECEF frame (m s-1)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF to TEME
//%                       double G[14][14][25]: magnetic potential coefficients
//%                       double H[14][14][25]: magnetic potential coefficients
//%
//% OUTPUT:               double B_field_i[3]: magnetic field vector in inertial frame
//%                       double B_field_i_dot[3]: time derivative of magnetic field
//%                         vector in inertial frame as seen from orbiting
//%                         spacecraft
//%
//% COUPLING:             - t2doy.c
//%                       - crossprod.c
//%                       - invertmat.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "magnet_field.h"
#include "t2doy.h"
#include <math.h>
#include "matxvec.h"
#include "invertmat.h"
#include "crossprod.h"

void magnet_field(double t2000utc, double p[3], double p_LLA[3], double v_LLA[3], double LLA[4], double C_ecef2teme[3][3], double G[14][14][25], double H[14][14][25], double B_field_i[3], double B_field_i_dot[3]){
    
    // Constants
    double a = 6371200; // Geomagnetic conventional Earth’s mean reference spherical radius
    
    // Get Decimal Year
    double sec, year_decimal;
    int time[3];
    sec = t2doy(t2000utc, time);
    if (time[0] % 4 == 0)
        year_decimal = time[0] + sec/(366*24*60*60);
    else
        year_decimal = time[0] + sec/(365*24*60*60);
    
    // Interpolate G and H linearly
    double G_t[14][14], H_t[14][14];
    int k;
    if (time[0] < 2005)
        k = 20;
    else if (time[0] < 2010)
        k = 21;
    else if (time[0] < 2015)
        k = 22;
    else
        k = 23;
    double year_start = (k+380)*5;
    if (k<23){
        for (int i=0; i<14; i++){
            for (int j=0; j<14; j++){
                G_t[i][j] = G[i][j][k]+(year_decimal-year_start)*(G[i][j][k+1]-G[i][j][k])/5.0;
                H_t[i][j] = H[i][j][k]+(year_decimal-year_start)*(H[i][j][k+1]-H[i][j][k])/5.0;
            }
        }
    }
    else if (k==23){
        for (int i=0; i<14; i++){
            for (int j=0; j<14; j++){
                G_t[i][j] = G[i][j][k]+(year_decimal-year_start)*G[i][j][k+1];
                H_t[i][j] = H[i][j][k]+(year_decimal-year_start)*H[i][j][k+1];
            }
        }
    }
    
    // Time derivative of G and H
    double dG_t[14][14], dH_t[14][14];
    if (k<23){
        for (int i=0; i<14; i++){
            for (int j=0; j<14; j++){
                dG_t[i][j] = (G[i][j][k+1]-G[i][j][k])/(5.0*365.25*24*60*60);
                dH_t[i][j] = (H[i][j][k+1]-H[i][j][k])/(5.0*365.25*24*60*60);
            }
        }
    }
    else if (k==23){
        for (int i=0; i<14; i++){
            for (int j=0; j<14; j++){
                dG_t[i][j] = G[i][j][k+1]/(365.25*24*60*60);
                dH_t[i][j] = H[i][j][k+1]/(365.25*24*60*60);
            }
        }
    }
    
    // Spherical geocentric distance, longitude and latitude (declination), and colatitude (m and rad)
    double r = sqrt(p_LLA[0]*p_LLA[0] + p_LLA[1]*p_LLA[1] + p_LLA[2]*p_LLA[2]);
    double lon = LLA[2];
    double lat = asin(p_LLA[2]/sqrt(p_LLA[0]*p_LLA[0]+p_LLA[1]*p_LLA[1]+p_LLA[2]*p_LLA[2]));
    double colat = M_PI/2.0 - lat;
    
    // Check for Poles
    if (colat>-0.00000001*M_PI/180.0 && colat<0.00000001*M_PI/180.0)
        colat=0.00000001*M_PI/180.0;
    else if(colat<180.00000001*M_PI/180.0 && colat>179.99999999*M_PI/180.0)
        colat=179.99999999*M_PI/180.0;
    
    // Calculate Schmidt normalized associated Legendre functions
    double P[14][14];
    double dP[14][14];
    double ddP[14][14];
    double S[14][14];
    for (int i=1; i<14; i++){
        for (int j=1; j<14; j++){
            P[i][j]=0;
            dP[i][j]=0;
            ddP[i][j]=0;
            S[i][j]=0;
        }
    }
    P[0][0] = 1;
    P[1][0] = cos(colat);
    for (int n=1; n<14; n++)
        P[n][n] = sin(colat)*P[n-1][n-1];
    for (int n=2; n<14; n++){
        for(int m=0; m<n; m++)
            P[n][m] = cos(colat)*P[n-1][m] - ((n-1)*(n-1)-m*m)*P[n-2][m]/((2*n-1)*(2*n-3));
    }
    dP[0][0] = 0;
    dP[1][0] = -sin(colat);
    for (int n=1; n<14; n++)
        dP[n][n] = sin(colat)*dP[n-1][n-1] + cos(colat)*P[n-1][n-1];
    for (int n=2; n<14; n++){
        for (int m=0; m<n; m++)
            dP[n][m] = cos(colat)*dP[n-1][m] - sin(colat)*P[n-1][m] - ((n-1)*(n-1)-m*m)*dP[n-2][m]/((2*n-1)*(2*n-3));
    }
    ddP[0][0] = 0;
    ddP[1][0] = -cos(colat);
    for (int n=1; n<14; n++)
        ddP[n][n] = 2*cos(colat)*dP[n-1][n-1] - sin(colat)*P[n-1][n-1] + sin(colat)*ddP[n-1][n-1];
    for (int n=2; n<14; n++){
        for (int m=0; m<n; m++)
            ddP[n][m] = cos(colat)*ddP[n-1][m] - 2*sin(colat)*dP[n-1][m] - cos(colat)*P[n-1][m] - ((n-1)*(n-1)-m*m)*ddP[n-2][m]/((2*n-1)*(2*n-3));
    }
    double delta;
    S[0][0] = 1;
    for (int n=1; n<14; n++)
        S[n][0] = S[n-1][0]*(2*n-1)/n;
    for (int n=1; n<14; n++){
        for (int m=1; m<=n; m++){
            if (m==1)
                delta = 1;
            else
                delta = 0;
            S[n][m] = S[n][m-1]*sqrt((n-m+1)*(delta+1)/(n+m));
        }
    }
    for (int n=0; n<14; n++){
        for (int m=0; m<=n; m++){
            P[n][m] = P[n][m] *S[n][m];
            dP[n][m] = dP[n][m] *S[n][m];
            ddP[n][m] = ddP[n][m] *S[n][m];
        }
    }
    
    // Partial Derivatives
    double dVdr = 0;
    double dVd0 = 0;
    double dVdl = 0;
    for (int l = 1; l <=13 ; l++ ){
        for (int m = 0; m<=l; m++){
            dVdr = dVdr - pow(a/r,l+2)*(l+1)*P[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            dVd0 = dVd0 + a*pow(a/r,l+1)*dP[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            dVdl = dVdl + a*pow(a/r,l+1)*m*P[l][m]*(H_t[l][m]*cos(m*lon)-G_t[l][m]*sin(m*lon));
        }
    }
    
    // Second Partial Derivatives
    double d2Vdr2 = 0;
    double d2Vd0dr = 0;
    double d2Vdldr = 0;
    double d2Vdtdr = 0;
    double d2Vd02 = 0;
    double d2Vdld0 = 0;
    double d2Vdtd0 = 0;
    double d2Vdl2 = 0;
    double d2Vdtdl = 0;
    for (int l = 1; l <=13 ; l++ ){
        for (int m = 0; m<=l; m++){
            d2Vdr2 = d2Vdr2 + 1/r*pow(a/r,l+2)*(l+2)*(l+1)*P[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            d2Vd0dr = d2Vd0dr - pow(a/r,l+2)*(l+1)*dP[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            d2Vdldr = d2Vdldr + pow(a/r,l+2)*(l+1)*m*P[l][m]*(G_t[l][m]*sin(m*lon)-H_t[l][m]*cos(m*lon));
            d2Vdtdr = d2Vdtdr - pow(a/r,l+2)*(l+1)*P[l][m]*(dG_t[l][m]*cos(m*lon)+dH_t[l][m]*sin(m*lon));
            d2Vd02 = d2Vd02 + a*pow(a/r,l+1)*ddP[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            d2Vdld0 = d2Vdld0 + a*pow(a/r,l+1)*m*dP[l][m]*(H_t[l][m]*cos(m*lon)-G_t[l][m]*sin(m*lon));
            d2Vdtd0 = d2Vdtd0 + a*pow(a/r,l+1)*dP[l][m]*(dG_t[l][m]*cos(m*lon)+dH_t[l][m]*sin(m*lon));
            d2Vdl2 = d2Vdl2 - a*pow(a/r,l+1)*m*m*P[l][m]*(G_t[l][m]*cos(m*lon)+H_t[l][m]*sin(m*lon));
            d2Vdtdl = d2Vdtdl + a*pow(a/r,l+1)*m*P[l][m]*(dH_t[l][m]*cos(m*lon)-dG_t[l][m]*sin(m*lon));
        }
    }
    
    // Magnetic Field in Spherical Components
    double B_field_spherical[3];
    B_field_spherical[0] = -dVdr*pow(10,-9);
    B_field_spherical[1] = -dVd0/r*pow(10,-9);
    B_field_spherical[2] = -dVdl/(r*sin(colat))*pow(10,-9);
    
    // Magnetic Field in North, East, Down Components
    double B_field_ned[3];
    B_field_ned[0] = -B_field_spherical[1];
    B_field_ned[1] = B_field_spherical[2];
    B_field_ned[2] = -B_field_spherical[0];
    
    // Rotate to ecef frame
    double C_ned2ecef[3][3] = {
        {-sin(lat)*cos(lon),    -sin(lon),  -cos(lat)*cos(lon)},
        {-sin(lat)*sin(lon),    cos(lon),   -cos(lat)*sin(lon)},
        {cos(lat),              0,          -sin(lat)}};
    double B_field_ecef[3];
    matxvec(C_ned2ecef,B_field_ned,B_field_ecef);
    matxvec(C_ecef2teme,B_field_ecef,B_field_i);
    
    // Velocity in Spherical Coordinates
    double v_sph[3], v_ned[3];
    double C_ecef2ned[3][3];
    invertmat(C_ned2ecef,C_ecef2ned);
    matxvec(C_ecef2ned, v_LLA,v_ned);
    v_sph[0] = - v_ned[2];
    v_sph[1] = - v_ned[0];
    v_sph[2] = v_ned[1];
    
    // Time Derivative of Magnetic Field
    double B_field_sph_dot[3];
    B_field_sph_dot[0] = -(v_sph[0]*d2Vdr2 + v_sph[1]/r*d2Vd0dr + v_sph[2]/(r*sin(colat))*d2Vdldr + d2Vdtdr)*pow(10,-9);
    B_field_sph_dot[1] = -(v_sph[0]/r*d2Vd0dr - v_sph[0]/(r*r)*dVd0 + v_sph[1]/(r*r)*d2Vd02 + v_sph[2]/(r*r*sin(colat))*d2Vdld0 + d2Vdtd0/r)*pow(10,-9);
    B_field_sph_dot[2] = -(v_sph[0]/(r*sin(colat))*d2Vdldr - v_sph[0]/(r*r*sin(colat))*dVdl - (v_sph[1]*cos(colat))/(r*r*sin(colat)*sin(colat))*dVdl + v_sph[1]/(r*r*sin(colat)*sin(colat))*d2Vdld0 + v_sph[2]/(r*r*sin(colat)*sin(colat))*d2Vdl2 + d2Vdtdl/(r*sin(colat)))*pow(10,-9);
    
    // Time Derivative Magnetic Field in North, East, Down Components
    double B_field_ned_dot[3];
    B_field_ned_dot[0] = -B_field_sph_dot[1];
    B_field_ned_dot[1] = B_field_sph_dot[2];
    B_field_ned_dot[2] = -B_field_sph_dot[0];
    
    // Rotate to ecef frame
    double w_ned[3] = {v_ned[1]/r, -v_ned[0]/r, 0};
    double wxB_ned[3];
    crossprod(w_ned,B_field_ned,wxB_ned);
    for (int i=0; i<3; i++)
        B_field_ned_dot[i] = B_field_ned_dot[i] + wxB_ned[i];
    double B_field_ecef_dot[3];
    matxvec(C_ned2ecef,B_field_ned_dot,B_field_ecef_dot);
    
    // Rotate to inertial frame
    double omegaearth[3] = {0, 0, 7.29211514670698e-05};
    double wxB_ecef[3];
    crossprod(omegaearth,B_field_ecef,wxB_ecef);
    for (int i=0; i<3; i++)
        B_field_ecef_dot[i] = B_field_ecef_dot[i] + wxB_ecef[i];
    matxvec(C_ecef2teme,B_field_ecef_dot,B_field_i_dot);
    
}
