//
//  srp.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-12-18.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        srp.c
//%
//% DESCRIPTION:          This function calculates the solar radiation
//%                       acceleration and torque for the entire surface model
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 December 18, 2017
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double r_sun[3]: position of Sun (m)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       int in_srp_a: inclusion of acceleration?
//%                       int in_srp_g: inclusion of torque?
//%
//% OUTPUT:               double a_srp[3]: direct solar radiation acceleration
//%                         in inertial frame
//%                       double g_srp[3]: direct solar radiation torque
//%                         in body-fixed frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - norm.c
//%                       - srp_force.c
//%                       - crossprod.c
//%                       - matxvec.c
//%                       - invertmat.c
//%                       - shadow_function.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "srp.h"
#include "surface.h"
#include "vectors2angle.h"
#include "norm.h"
#include "srp_force.h"
#include "crossprod.h"
#include <math.h>
#include "matxvec.h"
#include "invertmat.h"
#include "shadow_function.h"

void srp(double p[3], double v[3], double r_sun[3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, int in_srp_a, int in_srp_g, double a_srp[3], double g_srp[3]){
    
    // Initialize
    for (int i=0; i<3; i++){
        a_srp[i] = 0;
        g_srp[i] = 0;
    }
    
    // Light direction unit vector
    double rsun = norm(r_sun);
    double light[3];
    for (int i=0; i<3; i++){
        light[i] = -r_sun[i]/rsun;
    }
    
    // Switch from inertial frame to body frame
    double light_body[3];
    matxvec(C_i2b, light, light_body);
    
    // Inverse body rotation matrix
    double C_b2i[3][3];
    invertmat(C_i2b,C_b2i);
    
    // Find angle between light direction vector and surface normal to obtain surfaces in view of sun
    double tot_proj_surf = 0;
    double A_cross = 0;
    double alpha[n_surf], cos_alpha[n_surf];
    for (int i=0; i<n_surf; i++){
        alpha[i] = vectors2angle(light_body,geometry[i].normal);
        cos_alpha[i] = cos(alpha[i]);
        // If surface is in view calculate projected surface area
        if (cos_alpha[i]>0){
            geometry[i].projected_area = cos_alpha[i]*geometry[i].area;
        }
        else {
            geometry[i].projected_area = 0;
        }
        tot_proj_surf = tot_proj_surf + geometry[i].projected_area;
    }
    A_cross = tot_proj_surf;
    
    // Photon flux calculation using shadow function
    double phi = 1361*(149597870700.0/rsun)*(149597870700.0/rsun);
    double shadow = shadow_function(r_sun, p);
    phi = phi*shadow;
    
    // Pressure exerted by photons per unit area
    double c = 299792458.0;
    double p_srp = phi/c;
    
    //// FORCE CALCULATION
    double f_srp[3];
    for (int i=0; i<3; i++){
        f_srp[i] = 0;
    }
    // For each surface
    for (int i=0; i<n_surf; i++){
        
        // If in view
        if (cos_alpha[i]>0){
            
            // Calculate force for that surface in inertial frame
            double f_surf[3];
            srp_force(geometry[i], cos_alpha[i], C_b2i, light, f_surf);
            
            // Sum total force in inertial frame
            for (int j=0; j<3; j++)
                f_srp[j] = f_srp[j] + f_surf[j];
            
            //// TORQUE CALCULATION
            if (in_srp_g){
                
                // Force for that surface in body frame
                double f_surf_b[3];
                matxvec(C_i2b, f_surf, f_surf_b);
                
                // Calculate center of pressure in body frame
                double cp_b[3];
                cp_b[0] = (geometry[i].vertices[0][0] + geometry[i].vertices[0][1] + geometry[i].vertices[0][2])/3.0;
                cp_b[1] = (geometry[i].vertices[1][0] + geometry[i].vertices[1][1] + geometry[i].vertices[1][2])/3.0;
                cp_b[2] = (geometry[i].vertices[2][0] + geometry[i].vertices[2][1] + geometry[i].vertices[2][2])/3.0;
                
                // Torque for that surface
                double g_surf[3];
                crossprod(cp_b, f_surf_b, g_surf);
                
                // Sum up total torque in body frame
                for (int j=0; j<3; j++)
                    g_srp[j] = g_srp[j] + g_surf[j];
                
            }
        }
    }
    
    //// ACCELERATION CALCULATION
    if (in_srp_a){
        // Multiply by flux
        for (int i=0; i<3; i++)
            a_srp[i] = p_srp*f_srp[i]/m;
    }
    
    //// TORQUE CALCULATION
    if (in_srp_g){
        // Multiply by flux
        for (int i=0; i<3; i++)
            g_srp[i] = p_srp*g_srp[i];
    }
    
}
