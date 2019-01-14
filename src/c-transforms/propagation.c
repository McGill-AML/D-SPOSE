//
//  propagation.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        propagation.c
//%
//% DESCRIPTION:          This function will propagate the attitude and orbit
//%                       of a spacecraft under certain external torques
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 11, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000tt: seconds since January 1, 2000, 00:00:00 TT
//%                       double x[13]: state vector
//%                       double Inertia[3][3]: inertia matrix
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double M[3][3]: magnetic tensor
//%                       double m: spacecraft mass
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int length_of_file[5]: array containing length of file values
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double solar_input[length_of_file[1]][3]: F10.7 array for NRLMSISE-00
//%                       double G[14][14][25]: IGRF-12 magnetic potential coefficients
//%                       double H[14][14][25]: IGRF-12 magnetic potential coefficients
//%                       double G_wmm[13][13][8]: WMM magnetic potential coefficients
//%                       double H_wmm[13][13][8]: WMM magnetic potential coefficients
//%                       double model_parameters[27]: model parameters
//%                       double eop[length_of_file[2]][10]: earth orientation parameters
//%                       double sun_eph[length_of_file[3]][3]: sun ephemerides array
//%                       double moon_eph[length_of_file[4]][3]: moon ephemerides array
//%                       double albedo[12][20][40][2]: albedo and IR coefficients
//%                       double fn[3]: 3x1 vector sum of non-conservative forces
//%                       double gn[3]: 3x1 vector sum of non-conservative torques
//%                       double fg_i[42]: array containing list of 3x1 vector of forces
//%                         and torques
//%
//% OUTPUT:               double dx[13]: state vector update
//%                       double dxd[7]: state vector update (Kane damper)
//%
//% COUPLING:             - quatnormalize.c
//%                       - teme2ecef.c
//%                       - ecef2lla.c
//%                       - quat2dcm.c
//%                       - matxvec.c
//%                       - surface.h
//%                       - t2doy.c
//%                       - tt2utc.c
//%                       - invertmat.c
//%                       - angvelprop.c
//%                       - moon.c
//%                       - sun.c
//%                       - third_body.c
//%                       - eddy_torque.c
//%                       - gravity_field.c
//%                       - srp.c
//%                       - albedo_calc.c
//%                       - get_density.c
//%                       - wind.c
//%                       - magnet_field.c
//%                       - magnet_field_wmm.c
//%                       - aero_drag.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "propagation.h"
#include "quatnormalize.h"
#include "angvelprop.h"
#include <math.h>
#include "teme2ecef.h"
#include "ecef2lla.h"
#include "gravity_field.h"
#include "quat2dcm.h"
#include "eddy_torque.h"
#include "matxvec.h"
#include "get_density.h"
#include "aero_drag.h"
#include "surface.h"
#include "magnet_field.h"
#include "magnet_field_wmm.h"
#include "wind.h"
#include "t2doy.h"
#include "moon.h"
#include "sun.h"
#include "third_body.h"
#include "tt2utc.h"
#include "srp.h"
#include "invertmat.h"
#include "albedo_calc.h"

void propagation(double t2000tt, double x[13], double Inertia[3][3], double I_inv[3][3], double M[3][3], double m, int n_surf, struct surface geometry[n_surf], double C[101][101], double S[101][101], int length_of_file[5], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double G[14][14][25], double H[14][14][25], double G_wmm[13][13][8], double H_wmm[13][13][8], double model_parameters[27], double eop[length_of_file[2]][10], double sun_eph[length_of_file[3]][3], double moon_eph[length_of_file[4]][3], double albedo[12][20][40][2], double fn[3], double gn[3], double fg_i[42], double dx[13], double xd[7], double dxd[7]){
    
    // Initialize parameters
    double v[3], p[3], w[3], q[4], wd[3], qd[4];
    for (int i = 0; i<3; i++)
        v[i] = x[i];
    for (int i = 0; i<3; i++)
        p[i] = x[i+3];
    for (int i = 0; i<3; i++)
        w[i] = x[i+6];
    for (int i = 0; i<4; i++)
        q[i] = x[i+9];
    quatnormalize(q);
    
    // Model Parameters
    int l_max_a = model_parameters[14];
    int l_max_g = model_parameters[15];
    double Cd = model_parameters[16];
    double Ap =  model_parameters[17];
    double F107 = model_parameters[18];
    double Ik = model_parameters[23];
    double C_damper = model_parameters[24];
    
    // Inclusion Parameters (which model is considered?)
    int in_aero_a = model_parameters[0];
    int in_aero_g = model_parameters[1];
    int in_grav_a = model_parameters[2];
    int in_grav_g = model_parameters[3];
    int in_eddy_g = model_parameters[4];
    int in_sun_a = model_parameters[5];
    int in_moon_a = model_parameters[6];
    int in_srp_a = model_parameters[7];
    int in_alb_a = model_parameters[8];
    int in_ir_a = model_parameters[9];
    int in_srp_g = model_parameters[10];
    int in_alb_g = model_parameters[11];
    int in_ir_g = model_parameters[12];
    int in_kane = model_parameters[13];
    
    // Initialize Kane damper state parameter
    if (in_kane) {
        for (int i = 0; i<3; i++)
            wd[i] = xd[i];
        for (int i = 0; i<4; i++)
            qd[i] = xd[i+3];
        quatnormalize(qd);
    }
    
    // Atmospheric Model
    int atmos_model = model_parameters[19];
    
    // Magnetic Model
    int magnetic_model = model_parameters[20];
    
    // Horizontal Wind Model
    int wind_model = model_parameters[21];
    
    // Initialize torque and acceleration
    double a[3], g[3], an[3];
    for (int i=0; i<3; i++){
        g[i] = 0;   // Total propagated torque
        a[i] = 0;   // Total propagated acceleration
        fn[i] = 0;  // Sum of non-conservative forces
        gn[i] = 0;  // Sum of non-conservative torques
        an[i] = 0;  // Sum of non-conservative accelerations
    }
    for (int i=0; i<42; i++){
        fg_i[i] = 0;
    }
    
    // TT to UTC
    double t2000utc;
    t2000utc = tt2utc(t2000tt);
    double ttt = (t2000tt-(12*60*60.0))/(60*60*24*36525.0);   // julian centuries of TT
    
    // Day of Year UTC
    int time[3];
    double sec = t2doy(t2000utc, time);
    int d2000 = time[2];   // Day Number starting January 1, 2000
    
    // Get Earth Orientation Parameters (Not currently used)
    double dut1 = 0;// Not considering UTC vs UT1 difference for simplicity
    double t2000ut1 = t2000utc + dut1;
    double xp = 0;// TEME to ECEF only considering GMST sidereal time
    double yp = 0;// TEME to ECEF only considering GMST sidereal time
    double lod = 0;// TEME to ECEF only considering GMST sidereal time
    
    // Get position in ECEF frame and LLA coordinates
    double p_ecef[3], v_ecef[3], C_ecef2teme[3][3], LLA[4];
    teme2ecef(p, v, ttt, t2000ut1, xp, yp, lod, p_ecef, v_ecef, C_ecef2teme);
    ecef2lla(p_ecef, LLA);
    
    // Obtain body rotation matrix
    double C_i2b[3][3];
    quat2dcm(q, C_i2b);
    
    // Sun Position
    double r_sun[3];
    if (in_sun_a||in_srp_a||in_srp_g){
        sun(t2000tt, length_of_file, sun_eph, r_sun);
    }
    
    if (in_alb_a || in_alb_g || in_ir_a || in_ir_g){
        
        // Albedo and IR Acceleration and Torque
        double a_alb[3], g_alb[3], a_ir[3], g_ir[3];
        albedo_calc(p, v, LLA, time, r_sun, C_ecef2teme, C_i2b, n_surf, geometry, m, albedo, in_alb_a, in_alb_g, in_ir_a, in_ir_g, a_alb, g_alb, a_ir, g_ir);
        for (int i=0; i<3; i++){
            g[i] = g[i] + g_ir[i] + g_alb[i];
            a[i] = a[i] + a_ir[i] + a_alb[i];
            fg_i[i+24] = a_alb[i];
            fg_i[i+27] = a_ir[i];
            fg_i[i+33] = g_alb[i];
            fg_i[i+36] = g_ir[i];
            an[i] = an[i] + a_ir[i] + a_alb[i];
            gn[i] = gn[i] + g_ir[i] + g_alb[i];
        }
        
    }
    
    if (in_srp_a || in_srp_g){
        
        // Solar Radiation Pressure Acceleration and Torque
        double a_srp[3], g_srp[3];
        srp(p, v, r_sun, C_i2b, n_surf, geometry, m, in_srp_a, in_srp_g, a_srp, g_srp);
        for (int i=0; i<3; i++){
            g[i] = g[i] + g_srp[i];
            a[i] = a[i] + a_srp[i];
            fg_i[i+21] = a_srp[i];
            fg_i[i+30] = g_srp[i];
            an[i] = an[i] + a_srp[i];
            gn[i] = gn[i] + g_srp[i];
        }
    }
    
    if (in_aero_a || in_aero_g){
        
        // Atmospheric Density
        double density;
        density = get_density(atmos_model, LLA, t2000utc, length_of_file, ap_index, solar_input, Ap, F107);
        
        // Horizontal Winds
        double winds_i[3];
        wind(p_ecef, LLA, t2000utc, length_of_file, ap_index, C_ecef2teme, wind_model, winds_i);
        
        // Aerodynamic Acceleration and Torque
        double a_aero[3], g_aero[3];
        aero_drag(density, winds_i, p, v, w, C_i2b, n_surf, geometry, m, Cd, in_aero_a, in_aero_g, a_aero, g_aero);
        for (int i=0; i<3; i++){
            g[i] = g[i] + g_aero[i];
            a[i] = a[i] + a_aero[i];
            fg_i[i] = a_aero[i];
            fg_i[i+3] = g_aero[i];
            an[i] = an[i] + a_aero[i];
            gn[i] = gn[i] + g_aero[i];
        }
        
    }
    
    if (in_eddy_g){
        
        // Magnetic Field
        double B_field_i[3], B_field_i_dot[3];
        if (magnetic_model==1)
            magnet_field(t2000utc, p, p_ecef, v_ecef, LLA, C_ecef2teme, G, H, B_field_i, B_field_i_dot);
        else if (magnetic_model==2)
            magnet_field_wmm(t2000utc, p, p_ecef, v_ecef, LLA, C_ecef2teme, G_wmm, H_wmm, B_field_i, B_field_i_dot);
        double B_field_b[3], B_field_dot_b[3];
        matxvec(C_i2b,B_field_i,B_field_b);
        matxvec(C_i2b,B_field_i_dot,B_field_dot_b);
        
        // Eddy Current Torque
        double g_eddy[3];
        for (int i=0; i<3; i++)
            g_eddy[i] = 0;
        eddy_torque(B_field_b,B_field_dot_b,w,M,g_eddy);
        for (int i=0; i<3; i++){
            g[i] = g[i] + g_eddy[i];
            fg_i[i+12] = g_eddy[i];
            gn[i] = gn[i] + g_eddy[i];
        }
        
    }
    
    if (in_sun_a){
        
        // Third-body accelerations: Sun
        double a_sun[3];
        double mu_sun = 1.32712428*pow(10,20);
        third_body(p, r_sun, mu_sun, a_sun);
        for (int i=0; i<3; i++){
            a[i] = a[i] + a_sun[i];
            fg_i[i+15] = a_sun[i];
        }
        
    }
    
    if (in_moon_a){
        
        // Third-body accelerations: Moon
        double r_moon[3], a_moon[3];
        double mu_moon = 4902.799 * pow(10,9);
        moon(t2000tt, length_of_file, moon_eph, r_moon);
        third_body(p, r_moon, mu_moon, a_moon);
        for (int i=0; i<3; i++){
            a[i] = a[i] + a_moon[i];
            fg_i[i+18] = a_moon[i];
        }
        
    }
    
    if (in_grav_a || in_grav_g){
        
        // Gravitational Torque and Acceleration
        double g_grav_b[3], a_grav[3];
        gravity_field(p_ecef, p, LLA, C_i2b, Inertia, C, S, l_max_a, l_max_g, in_grav_a, in_grav_g, a_grav, g_grav_b);
        for (int i=0; i<3; i++){
            g[i] = g[i] + g_grav_b[i];
            a[i] = a[i] + a_grav[i];
            fg_i[i+6] = a_grav[i];
            fg_i[i+9] = g_grav_b[i];
        }
        
    }
    
    // Kane damper torque
    double gd[3];
    if (in_kane) {
        double C_i2d[3][3], C_d2i[3][3], C_b2i[3][3];
        quat2dcm(qd, C_i2d);
        invertmat(C_i2d, C_d2i);
        invertmat(C_i2b, C_b2i);
        double gd_body[3], gd_i[3];
        double wd_body[3], wd_i[3];
        matxvec(C_d2i,wd,wd_i);
        matxvec(C_i2b,wd_i,wd_body);
        for (int i=0; i<3; i++){
            gd_body[i] = C_damper*(w[i]-wd_body[i]);  // Torque of spacecraft on spherical damper in body frame
            g[i] = g[i] - gd_body[i];
            fg_i[i+39] = -gd_body[i];
            gn[i] = gn[i] - gd_body[i];
        }
        matxvec(C_b2i,gd_body,gd_i);
        matxvec(C_i2d,gd_i,gd);
    }
    
    /* UPDATING PARAMETERS */
    double r = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
    double mu = 3986004.418*pow(10,8);
    double dv[3];
    for (int i=0; i<3; i++){
        dv[i] = -mu*p[i]/(r*r*r) + a[i];
        fn[i] = m*an[i];
    }
    
    // Propagate position
    double dp[3] = {v[0],v[1],v[2]};
    
    // Propagatie angular velocity
    double dw[3];
    angvelprop(Inertia, I_inv, w, g, dw);
    
    // Propagate orientation
    double Omega[4][4] = {
        {0, -w[0], -w[1], -w[2]},
        {w[0], 0, w[2], -w[1]},
        {w[1], -w[2], 0, w[0]},
        {w[2], w[1], -w[0], 0}
    };
    double dq[4] = {
        0.5*(Omega[0][0]*q[0]+Omega[0][1]*q[1]+Omega[0][2]*q[2]+Omega[0][3]*q[3]),
        0.5*(Omega[1][0]*q[0]+Omega[1][1]*q[1]+Omega[1][2]*q[2]+Omega[1][3]*q[3]),
        0.5*(Omega[2][0]*q[0]+Omega[2][1]*q[1]+Omega[2][2]*q[2]+Omega[2][3]*q[3]),
        0.5*(Omega[3][0]*q[0]+Omega[3][1]*q[1]+Omega[3][2]*q[2]+Omega[3][3]*q[3])
    };
    
    // Damper
    if (in_kane) {
        
        double dwd[3];
        
        // Damper inertia diagonal
        double I_d[3][3] = {{Ik,0,0},{0,Ik,0},{0,0,Ik}};
        
        // Propagate damper angular velocity
        angvelprop(I_d, I_d, wd, gd, dwd);
        
        // Propagate damper orientation
        double Omega_d[4][4] = {
            {0, -wd[0], -wd[1], -wd[2]},
            {wd[0], 0, wd[2], -wd[1]},
            {wd[1], -wd[2], 0, wd[0]},
            {wd[2], wd[1], -wd[0], 0}
        };
        double dqd[4] = {
            0.5*(Omega_d[0][0]*qd[0]+Omega_d[0][1]*qd[1]+Omega_d[0][2]*qd[2]+Omega_d[0][3]*qd[3]),
            0.5*(Omega_d[1][0]*qd[0]+Omega_d[1][1]*qd[1]+Omega_d[1][2]*qd[2]+Omega_d[1][3]*qd[3]),
            0.5*(Omega_d[2][0]*qd[0]+Omega_d[2][1]*qd[1]+Omega_d[2][2]*qd[2]+Omega_d[2][3]*qd[3]),
            0.5*(Omega_d[3][0]*qd[0]+Omega_d[3][1]*qd[1]+Omega_d[3][2]*qd[2]+Omega_d[3][3]*qd[3])
        };
        
        // Update damper
        for (int i = 0; i<3; i++)
            dxd[i] = dwd[i];
        for (int i = 0; i<4; i++)
            dxd[i+3] = dqd[i];
    }
    
    // Update
    for (int i = 0; i<3; i++)
        dx[i] = dv[i];
    for (int i = 0; i<3; i++)
        dx[i+3] = dp[i];
    for (int i = 0; i<3; i++)
        dx[i+6] = dw[i];
    for (int i = 0; i<4; i++)
        dx[i+9] = dq[i];
    
}
