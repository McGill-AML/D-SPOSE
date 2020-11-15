//
//  main.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% SCRIPT NAME:          main.c
//%
//% DESCRIPTION:          This is the main script of the Debris SPin/Orbit
//%                       Simulation Environment (D-SPOSE). Please refer to
//%                       the Documentation for help.
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 March 15, 2017
//% VERSION:              1
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "propagation.h"
#include "state2orbital.h"
#include "orbital2state.h"
#include "surface.h"
#include "quatnormalize.h"
#include "sc_parameters.h"
#include "sc_geometry.h"
#include "load_inputs.h"
#include "norm_coef.h"
#include "gaus_coef.h"
#include "gaus_coef_wmm.h"
#include "load_teme.h"
#include "check_inputs.h"
#include "teme2ecef.h"
#include "ecef2eci.h"
#include "transpose.h"
#include "matrixmult.h"
#include "tt2utc.h"
#include "t2doy.h"
#include "dotprod.h"
#include "moon.h"
#include "sun.h"
#include "grav_potential.h"
#include "sun_potential.h"
#include "moon_potential.h"

int main()
{
    
    // VERSION NUMBER
    char version[100] = "1.0.1";
    printf("\nVersion: %s\n\n",version);
    
    // Output File Location
    char loc_output[200] = "output/";
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    // Check length of input files
    int length_of_file[5];
    check_inputs(length_of_file);
    
    // Load input files
    double time_parameters[13], spacecraft_parameters[33], grav_coef[5148][6], mag_coef[195][27], model_parameters[27], ap_index[length_of_file[0]], solar_input[length_of_file[1]][3], wmm_coef[90][18], iar80[106][5], rar80[106][4], eop[length_of_file[2]][10], max_dates[5][3], sun_eph[length_of_file[3]][3], moon_eph[length_of_file[4]][3], albedo[12][20][40][2];
    load_inputs(length_of_file, time_parameters, spacecraft_parameters, grav_coef, mag_coef, model_parameters, ap_index, solar_input, wmm_coef, iar80, rar80, eop, max_dates, sun_eph, moon_eph, albedo);
    
    // Load TLE output (r and v in TEME frame) and set initial orbital elements in (TEME frame)
    load_teme(eop, spacecraft_parameters, time_parameters, model_parameters, max_dates);
    
    // Normalize coefficients of gravitational and magnetic potentials to correct units
    double C[101][101], S[101][101], G[14][14][25], H[14][14][25], G_wmm[13][13][8], H_wmm[13][13][8];
    norm_coef(grav_coef, C, S);
    gaus_coef(mag_coef, G, H);
    gaus_coef_wmm(wmm_coef, G_wmm, H_wmm);
    
    // Initialize time parameters
    double t_start = time_parameters[0]*24*60*60 + time_parameters[1]*60*60 + time_parameters[2]*60 + time_parameters[3];   // 0 = Jan 1, 2000 00:00:00 TT
    double time_s = time_parameters[4]*24*60*60 + time_parameters[5]*60*60 + time_parameters[6]*60 + time_parameters[7];    // Propagation time in seconds
    double output_step = time_parameters[8]*24*60*60 + time_parameters[9]*60*60 + time_parameters[10]*60 + time_parameters[11];
    double time_step = time_parameters[12];
    
    // Initialize spacecraft parameters
    double m, coe[9], Inertia[3][3], I_inv[3][3], w[3], q[4], p[3], v[3], M[3][3];
    int n_surf;
    sc_parameters(spacecraft_parameters, coe, Inertia, I_inv, w, q, M);
    m = spacecraft_parameters[0]; // mass (kg)
    n_surf = spacecraft_parameters[1];  // Number of surfaces in geometry
    struct surface geometry[n_surf];
    double total_surface = sc_geometry(n_surf, geometry);
    
    // Initialize state and orbital parameters. Output to screen.
    orbital2state(p,v,coe);
    state2orbital(p,v,coe);
    for (int i = 0; i<7; i++)
        printf("%f\t",coe[i]);
    printf("\n");
    
    // Damper initial conditions (same as spacecraft)
    double wd[3];
    double qd[4];
    int in_kane = model_parameters[13];
    if (in_kane) {
        qd[0] = q[0];
        qd[1] = q[1];
        qd[2] = q[2];
        qd[3] = q[3];
        wd[0] = w[0];
        wd[1] = w[1];
        wd[2] = w[2];
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    // Initial vector for propagation including 1:3 velocity; 4:6 position; 7:9 angular velocity; 10:13 quaternion
    double x[20];
    for (int i = 0; i<3; i++)
        x[i] = v[i];
    for (int i = 3; i<6; i++)
        x[i] = p[i-3];
    for (int i = 6; i<9; i++)
        x[i] = w[i-6];
    for (int i = 9; i<13; i++)
        x[i] = q[i-9];
    
    // Initial vector for propagation (Kane Damper)
    double xd[7];
    if (in_kane) {
        for (int i = 0; i<3; i++)
            xd[i] = wd[i];
        for (int i = 0; i<4; i++)
            xd[i+3] = qd[i];
    }
        
    /* PROPAGATION PARAMETERS */
    // Initial time
    double time_current = 0.0;
    // Step number
    int n_step = 0.0;
    // Time step at each integration
    double dt = time_step;
    // Number of seconds in one day
    int day = 24*60*60;
    // Day number at each integration
    int daynum;
    // Initialize Terestrial Time (used as propagation time)
    double t2000tt = t_start + time_current;
    
    // Arrays used for integration
    double dx1[13];
    double x2[13], dx2[13];
    double x3[13], dx3[13];
    double x4[13], dx4[13];
    double x5[13], dx5[13];
    double x6[13], dx6[13];
    double x7[13], dx7[13];
    
    // Arrays used for integration (Kane Damper)
    double dxd1[7];
    double xd2[7], dxd2[7];
    double xd3[7], dxd3[7];
    double xd4[7], dxd4[7];
    double xd5[7], dxd5[7];
    double xd6[7], dxd6[7];
    double xd7[7], dxd7[7];
    
    /* RUNGE-KUTTA DORMAND-PRINCE INTEGRATOR */
    // Butcher tableau
    //double C[6] = {1/5.0, 3/10.0, 4/5.0, 8/9.0, 1.0, 1.0};
    double A[7][6] = {
        {1/5.0, 3/40.0, 44/45.0, 19372/6561.0, 9017/3168.0, 35/384.0},
        {0, 9/40.0, -56/15.0, -25360/2187.0, -355/33.0, 0.0},
        {0,           0,       32/9.0,    64448/6561.0,      46732/5247.0,      500/1113.0},
        {0,           0,       0,       -212/729.0,        49/176.0,          125/192.0},
        {0,           0,       0,       0,               -5103/18656.0,     -2187/6784.0},
        {0,           0,       0,       0,               0,               11/84.0},
        {0,           0,       0,       0,               0,               0}
    };
    double B[7] = {35/384.0, 0, 500/1113.0, 125/192.0, -2187/6784.0, 11/84.0};
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    /* Initialize PROPAGATION OUTPUT FILE */
    char loc_propagation[200];
    char monte_propagation[200];
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"_%F_%H-%M-%S",timeinfo);
    sprintf(monte_propagation, "propagation_v");
    strcpy(loc_propagation, loc_output);
    strcat(loc_propagation, monte_propagation);
    strcat(loc_propagation, version);
    strcat(loc_propagation, buffer);
    strcat(loc_propagation, ".txt");
    FILE *f_propagation = fopen(loc_propagation, "w");
    if (f_propagation == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    /* Initialize WORK OUTPUT FILE */
    int in_work = model_parameters[25];
    char loc_param[200];
    char monte_param[200];
    sprintf(monte_param, "parameters_v");
    strcpy(loc_param, loc_output);
    strcat(loc_param, monte_param);
    strcat(loc_param, version);
    strcat(loc_param, buffer);
    strcat(loc_param, ".txt");
    FILE *f_param;
    if (in_work){
        f_param = fopen(loc_param, "w");
        if (f_param == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
    }
    
    /* Initialize PERTURBATIONS OUTPUT FILE */
    int in_pert = model_parameters[26];
    char loc_pert[200];
    char monte_pert[200];
    sprintf(monte_pert, "perturbations_v");
    strcpy(loc_pert, loc_output);
    strcat(loc_pert, monte_pert);
    strcat(loc_pert, version);
    strcat(loc_pert, buffer);
    strcat(loc_pert, ".txt");
    FILE *f_pert;
    if (in_pert){
        f_pert = fopen(loc_pert, "w");
        if (f_pert == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
    }
    
    /* Initialize and Print GEOMETRY OUTPUT FILE */
    char loc_geom[200];
    char monte_geom[200];
    sprintf(monte_geom, "geometry_v");
    strcpy(loc_geom, loc_output);
    strcat(loc_geom, monte_geom);
    strcat(loc_geom, version);
    strcat(loc_geom, buffer);
    strcat(loc_geom, ".txt");
    FILE *f_geom;
    f_geom = fopen(loc_geom, "w");
    if (f_geom == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f_geom,"##### Spacecraft Geometry\n# Each row is one triangular surface\n# Columns 1-3 are inward surface normal\n# Columns 4-6, 7-9, 10-12 are coordinates of the vertices in body-fixed frame\n# Columns 13-18 are the coefficients of diffuse reflection, specular reflection, and absorption in (1) visible and (2) infrared spectrum\n");
    for (int i=0; i<n_surf; i++){
        fprintf(f_geom,"%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.16f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",geometry[i].normal[0],geometry[i].normal[1],geometry[i].normal[2],geometry[i].vertices[0][0],geometry[i].vertices[1][0],geometry[i].vertices[2][0],geometry[i].vertices[0][1],geometry[i].vertices[1][1],geometry[i].vertices[2][1],geometry[i].vertices[0][2],geometry[i].vertices[1][2],geometry[i].vertices[2][2],geometry[i].crd,geometry[i].crs,geometry[i].ca,geometry[i].crd_ir,geometry[i].crs_ir,geometry[i].ca_ir);
    }
    fclose(f_geom);
    
    /* Output Input parameters in Output file */
    time_t now;
    time(&now);
    fprintf(f_propagation,"# VERSION NUMBER: %s\n",version);
    fprintf(f_propagation,"# START TIME: %s# \n", ctime(&now));
    fprintf(f_propagation,"# TIME PARAMETERS:\t");
    for (int i=0; i<sizeof(time_parameters)/sizeof(time_parameters[0]); i++)
        fprintf(f_propagation,"%f\t",time_parameters[i]);
    fprintf(f_propagation,"\n");
    fprintf(f_propagation,"# SPACECRAFT PARAMETERS:\t");
    for (int i=0; i<sizeof(spacecraft_parameters)/sizeof(spacecraft_parameters[0]); i++)
        fprintf(f_propagation,"%f\t",spacecraft_parameters[i]);
    fprintf(f_propagation,"\n");
    fprintf(f_propagation,"# MODEL PARAMETERS:\t");
    for (int i=0; i<sizeof(model_parameters)/sizeof(model_parameters[0]); i++)
        fprintf(f_propagation,"%f\t",model_parameters[i]);
    fprintf(f_propagation,"\n#\n");
    
    // Print Transformation Matrix TEME to J2000 (assuming t_start conditions and both are inertial, non-evolving frames)
    // This is done by first converting TEME to ECEF considering polar motion and sidereal time and then by converting from ECEF to J2000
    
    double t2000utc;
    t2000utc = tt2utc(t2000tt); // convert terrestrial time to UTC time
    double ttt = (t2000tt-(12*60*60.0))/(60*60*24*36525.0);   // julian centuries of TT
    // Day of Year UTC
    int timev[3];
    double sec = t2doy(t2000utc, timev);
    int d2000 = timev[2];   // Day Number starting January 1, 2000
    // Get Earth Orientation Parameters
    double xp = eop[d2000-1][4]*M_PI/(648000.0);
    double yp = eop[d2000-1][5]*M_PI/(648000.0);
    double dut1 = eop[d2000-1][6];
    double lod = eop[d2000-1][7];
    double dpsi = 0;    // We want J2000 and not GCRF
    double deps = 0;    // We want J2000 and not GCRF
    double C_ecef2teme[3][3], C_teme2ecef[3][3], r_ecef[3], v_ecef[3], p_eci[3], v_eci[3], C_ecef2eci[3][3], C_teme2eci[3][3];
    // Convert TEME to ECEF
    teme2ecef(p, v, ttt, t2000utc+dut1, xp, yp, lod, r_ecef, v_ecef, C_ecef2teme);
    // Convert ECEF to ECI
    ecef2eci(r_ecef, v_ecef, ttt, t2000utc+dut1, iar80, rar80, xp, yp, lod, dpsi, deps, p_eci, v_eci, C_ecef2eci);
    transpose(C_ecef2teme, C_teme2ecef);
    matrixmult(C_ecef2eci, C_teme2ecef, C_teme2eci);
    fprintf(f_propagation,"# Rotation Matrix from TEME to ECI (J2000) at propagation start:\n# ");
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++)
            fprintf(f_propagation,"%.16f\t",C_teme2eci[i][j]);
        fprintf(f_propagation,"\n# ");
    }
    fprintf(f_propagation,"\n");
    
    // Print Legend in Output file
    fprintf(f_propagation,"# Time since Start (s)\tVelocity Components in TEME [v_i v_j v_k] (m/s)\tPosition Components in TEME [r_i r_j r_k] (m)\tAngular Velocity Components in Body Frame [w_x w_y w_z] (rad/s)\tOrientation Quaternion [q_0 q_1 q_2 q_3]\n");
    
    // Print Legend in Work file
    if (in_work)
        fprintf(f_param,"# Time since Start (s)\tNon-Conservative Translational Work (J)\tNon-Conservative Rotational Work (J)\tWork from Gravity-Gradient Torque (J)\tWork from Aspherical Earth Potential (J)\tWork from Sun Acceleration (J)\tWork from Moon Acceleration (J)\tEarth Potential Energy (J)\tSun Potential (J)\tMoon Potential (J)\n");
    
    // Print Legend in Perturbations file
    if (in_pert)
        fprintf(f_pert,"# Perturbations Output File\n# Accelerations in ECI; Torques in Body-Fixed Frame\n# Time since Start (s)\tAerodynamic Acceleration (m s-2)\tAerodynamic Torque (N m)\tGravitational Acceleration (extra terms) (m s-2)\tGravity-Gradient Torque (N m)\tEddy-Current Torque (N m)\tSun Acceleration (m s-2)\tMoon Acceleration (m s-2)\tSolar Radiation Acceleration (m s-2)\tAlbedo Acceleration (m s-2)\tIR Acceleration (m s-2)\tSolar Radiation Torque (N m)\tAlbedo Torque (N m)\tIR Torque (N m)\tKane Damper Torque (N m)\n");
    
    // Initialize Work - Energy parameters
    double f[3], g[3], fg_i[42];
    double Wf = 0;
    double Wt = 0;
    double W_gg = 0;
    double W_e = 0;
    double W_sun = 0;
    double W_moon = 0;
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    /* START PROPAGATION */
    while (time_current < time_s){
        
        /* OUTPUT DAY NUMBER */
        if (n_step % (int)(output_step/dt) == 0){
            daynum = time_current /day+1;
            printf("Day Number: %d\n", daynum);
        }
        
        // Time since January 1, 2000, 00:00:00 TT
        t2000tt = t_start + time_current;
        
        /* PRINT STATE TO OUTPUT FILE */
        if (n_step % (int)(output_step/dt) == 0){
            fprintf(f_propagation,"%f\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n", time_current, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12]);
        }
        
        /* PRINT WORK TO WORK FILE */
        if ((n_step % (int)(output_step/dt) == 0)&&(in_work))
            fprintf(f_param,"%f\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t",time_current,Wf,Wt,W_gg,W_e,W_sun,W_moon);
        
        /* INTEGRATE */
        propagation(t2000tt, x, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx1, xd, dxd1);
        for (int j = 0; j<13; j++)
            x2[j] = x[j]+dt*A[0][0]*dx1[j];
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd2[j] = xd[j]+dt*A[0][0]*dxd1[j];
        }
        propagation(t2000tt, x2, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx2, xd2, dxd2);
        for (int j = 0; j<13; j++)
            x3[j] = x[j]+dt*(A[0][1]*dx1[j]+A[1][1]*dx2[j]);
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd3[j] = xd[j]+dt*(A[0][1]*dxd1[j]+A[1][1]*dxd2[j]);
        }
        propagation(t2000tt, x3, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx3, xd3, dxd3);
        for (int j = 0; j<13; j++)
            x4[j] = x[j]+dt*(A[0][2]*dx1[j]+A[1][2]*dx2[j]+A[2][2]*dx3[j]);
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd4[j] = xd[j]+dt*(A[0][2]*dxd1[j]+A[1][2]*dxd2[j]+A[2][2]*dxd3[j]);
        }
        propagation(t2000tt, x4, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx4, xd4, dxd4);
        for (int j = 0; j<13; j++)
            x5[j] = x[j]+dt*(A[0][3]*dx1[j]+A[1][3]*dx2[j]+A[2][3]*dx3[j]+A[3][3]*dx4[j]);
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd5[j] = xd[j]+dt*(A[0][3]*dxd1[j]+A[1][3]*dxd2[j]+A[2][3]*dxd3[j]+A[3][3]*dxd4[j]);
        }
        propagation(t2000tt, x5, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx5, xd5, dxd5);
        for (int j = 0; j<13; j++)
            x6[j] = x[j]+dt*(A[0][4]*dx1[j]+A[1][4]*dx2[j]+A[2][4]*dx3[j]+A[3][4]*dx4[j]+A[4][4]*dx5[j]);
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd6[j] = xd[j]+dt*(A[0][4]*dxd1[j]+A[1][4]*dxd2[j]+A[2][4]*dxd3[j]+A[3][4]*dxd4[j]+A[4][4]*dxd5[j]);
        }
        propagation(t2000tt, x6, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx6, xd6, dxd6);
        for (int j = 0; j<13; j++)
            x7[j] = x[j]+dt*(A[0][5]*dx1[j]+A[1][5]*dx2[j]+A[2][5]*dx3[j]+A[3][5]*dx4[j]+A[4][5]*dx5[j]+A[5][5]*dx6[j]);
        if(in_kane){
            for (int j = 0; j<7; j++)
                xd7[j] = xd[j]+dt*(A[0][5]*dxd1[j]+A[1][5]*dxd2[j]+A[2][5]*dxd3[j]+A[3][5]*dxd4[j]+A[4][5]*dxd5[j]+A[5][5]*dxd6[j]);
        }
        propagation(t2000tt, x7, Inertia, I_inv, M, m, n_surf, geometry, C, S, length_of_file, ap_index, solar_input, G, H, G_wmm, H_wmm, model_parameters, eop, sun_eph, moon_eph, albedo, f, g, fg_i, dx7, xd7, dxd7);
        for (int j = 0; j<13; j++)
            x[j] = x[j] + dt*(B[0]*dx1[j]+B[1]*dx2[j]+B[2]*dx3[j]+B[3]*dx4[j]+B[4]*dx5[j]+B[5]*dx6[j]+B[6]*dx7[j]);
        if (in_kane){
            for (int j = 0; j<7; j++)
                xd[j] = xd[j] + dt*(B[0]*dxd1[j]+B[1]*dxd2[j]+B[2]*dxd3[j]+B[3]*dxd4[j]+B[4]*dxd5[j]+B[5]*dxd6[j]+B[6]*dxd7[j]);
        }
        
        // Calculate Work parameters
        if (in_work){
            // Calculate Work (correct position is here: after last integrator step, so g and f are correct; before state update, so v and w are correct)
            Wf = Wf + dotprod(f,v)*dt;
            Wt = Wt + dotprod(g,w)*dt;
            
            // Work done by Gravity-Gradient Torque
            if (model_parameters[3]==1){
                double g_gg[3];
                for (int i=0; i<3; i++)
                    g_gg[i] = fg_i[i+9];
                W_gg = W_gg + dotprod(g_gg,w)*dt;
            }
            
            // Work done by Earth Aspherical Acceleration
            if (model_parameters[2]==1){
                double f_e[3];
                for (int i=0; i<3; i++)
                    f_e[i] = m*fg_i[i+6];
                W_e = W_e + dotprod(f_e,v)*dt;
            }
            
            // Work done by Sun Acceleration
            if (model_parameters[5]==1){
                double f_sun[3];
                for (int i=0; i<3; i++)
                    f_sun[i] = m*fg_i[i+15];
                W_sun = W_sun + dotprod(f_sun,v)*dt;
            }
            
            // Work done by Moon Acceleration
            if (model_parameters[6]==1){
                double f_moon[3];
                for (int i=0; i<3; i++)
                    f_moon[i] = m*fg_i[i+18];
                W_moon = W_moon + dotprod(f_moon,v)*dt;
            }
            
        }
        
        /* PRINT ENERGY TO WORK FILE */
        if ((n_step % (int)(output_step/dt) == 0)&&(in_work)){
            
            // Calculate Gravitational Potential Energy
            double U = 0;
            if (model_parameters[2]==1){
                int l_max_a = model_parameters[14];
                U = grav_potential(t2000tt, p, v, m, C, S, l_max_a);
            }
            
            // Sun Potential Energy
            double U_sun = 0;
            if (model_parameters[5]==1){
                double r_sun[3];
                sun(t2000tt, length_of_file, sun_eph, r_sun);
                U_sun = sun_potential(p,m,r_sun);
            }
            
            // Moon Potential Energy
            double U_moon = 0;
            if (model_parameters[6]==1){
                double r_moon[3];
                moon(t2000tt, length_of_file, moon_eph, r_moon);
                U_moon = moon_potential(p,m,r_moon);
            }
            
            fprintf(f_param,"%.16e\t%.16e\t%.16e\n",U,U_sun,U_moon);
        }
        
        /* PRINT PERTURBATIONS TO PERTURBATIONS FILE */
        if ((n_step % (int)(output_step/dt) == 0)&&(in_pert)){
            fprintf(f_pert,"%f\t",time_current);
            for (int i=0; i<42; i++)
                fprintf(f_pert,"%.4e\t",fg_i[i]);
            fprintf(f_pert,"\n");
        }
        
        /* UPDATE CURRENT STATE */
        for (int i = 0; i<3; i++)
            v[i] = x[i];
        for (int i = 3; i<6; i++)
            p[i-3] = x[i];
        for (int i = 6; i<9; i++)
            w[i-6] = x[i];
        for (int i = 9; i<13; i++)
            q[i-9] = x[i];
        quatnormalize(q);
        for (int j = 0; j<4; j++)
            x[j+9] = q[j];
        
        // Update Kane Damper state
        if (in_kane) {
            for (int i = 0; i<3; i++)
                wd[i] = xd[i];
            for (int i = 0; i<4; i++)
                qd[i] = xd[i+3];
            quatnormalize(qd);
            for (int i = 0; i<4; i++)
                xd[i+3] = qd[i];
        }
            
        // Go to next time step
        time_current = time_current+dt;
        n_step = n_step+1;
        
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    // Print final state
    fprintf(f_propagation,"%f\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n", time_current, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12]);
    time(&now);
    fprintf(f_propagation,"# \n# FINISH TIME: %s", ctime(&now));
    fclose(f_propagation);
    
    // Final perturbations are null
    if (in_pert){
        fprintf(f_pert,"%f\t",time_current);
        for (int i=0; i<42; i++)
            fprintf(f_pert,"0.0\t");
        fprintf(f_pert,"\n");
        fclose(f_pert);
    }
    
    // Final Work and Energy
    if (in_work){
        
        fprintf(f_param,"%f\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t",time_current,Wf,Wt,W_gg,W_e,W_sun,W_moon);
        
        // Calculate Potential Energy
        double U = 0;
        if (model_parameters[2]==1){
            int l_max_a = model_parameters[14];
            U = grav_potential(t2000tt, p, v, m, C, S, l_max_a);
        }
        
        // Sun Potential Energy
        double U_sun = 0;
        if (model_parameters[5]==1){
            double r_sun[3];
            sun(t2000tt, length_of_file, sun_eph, r_sun);
            U_sun = sun_potential(p,m,r_sun);
        }
        
        // Moon Potential Energy
        double U_moon = 0;
        if (model_parameters[6]==1){
            double r_moon[3];
            moon(t2000tt, length_of_file, moon_eph, r_moon);
            U_moon = moon_potential(p,m,r_moon);
        }
        
        fprintf(f_param,"%.16e\t%.16e\t%.16e\n",U,U_sun,U_moon);
    }
    
    // Output orbital parameters to screen
    state2orbital(p,v,coe);
    for (int i = 0; i<7; i++)
        printf("%f\t",coe[i]);
    printf("\n");
    
    return 0;
}
