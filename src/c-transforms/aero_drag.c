//
//  aero_drag.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-13.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        aero_drag.c
//%
//% DESCRIPTION:          This function calculates the acceleration and
//%                       torque due to atmospheric drag
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 13, 2017
//% VERSION:              1
//%
//% INPUT:                double density: atmospheric density (kg m-3)
//%                       double w_vector[3]: wind velocity in inertial frame (m s-1)
//%                       double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double w[3]: angular velocity vector (rad s-1)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       double Cd: drag coefficient
//%                       int in_aero_a: inclusion of acceleration?
//%                       int in_aero_g: inclusion of torque?
//%
//% OUTPUT:               double a_aero[3]: acceleration due to atmospheric
//%                         drag in inertial frame
//%                       double g_aero[3]: torque due to atmospheric drag
//%                         in body frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - aero_force.c
//%                       - aero_torque.c
//%                       - matxvec.c
//%                       - invertmat.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "aero_drag.h"
#include <math.h>
#include "invertmat.h"
#include "vectors2angle.h"
#include "aero_force.h"
#include "aero_torque.h"
#include "surface.h"
#include "matxvec.h"

void aero_drag(double density, double w_vector[3], double p[3], double v[3], double w[3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, double Cd, int in_aero_a, int in_aero_g, double a_aero[3], double g_aero[3]){
    
    for (int i=0; i<3; i++){
        a_aero[i] = 0;
        g_aero[i] = 0;
    }
    
    // Get atmosphere angular velocity in inertial frame
    double w_atmos[3] = {0,0,7.29211514670698*pow(10,-05)};
    
    // Calculate relative wind speed in inertial frame
    double rel_wind_inertial[3];
    for (int i=0; i<3; i++)
        rel_wind_inertial[i] = w_vector[i] - v[i];
    
    // Switch from inertial frame to body frame
    double rel_wind_body[3];
    matxvec(C_i2b, rel_wind_inertial, rel_wind_body);
    
    // Inverse body rotation matrix
    double C_b2i[3][3];
    invertmat(C_i2b,C_b2i);
    
    // Switch body angular velocity from body to inertial
    double w_i[3];
    matxvec(C_b2i, w, w_i);
    
    // Calculate relative atmosphere angular velocity inertial frame
    double w_rel_i[3];
    for (int i=0; i<3; i++)
        w_rel_i[i] = w_i[i] - w_atmos[i];
    
    // Switch atmos angular velocity to body frame
    double w_atmos_b[3];
    matxvec(C_i2b, w_atmos, w_atmos_b);
    
    // Calculate relative atmosphere angular velocity in body frame
    double w_rel_b[3];
    for (int i=0; i<3; i++)
        w_rel_b[i] = w[i] - w_atmos_b[i];
    
    // Find angle between wind and surface normal to obtain surfaces in view of wind
    double tot_proj_surf = 0;
    double A_cross = 0;
    double alpha[n_surf], cos_alpha[n_surf];
    for (int i=0; i<n_surf; i++){
        alpha[i] = vectors2angle(rel_wind_body,geometry[i].normal);
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
    
    //// FORCE CALCULATION
    if (in_aero_a){
        
        //Calculate for each surface
        double f_aero[3];
        for (int i=0; i<3; i++)
            f_aero[i] = 0;
        for (int i=0; i<n_surf; i++){
            
            // If in view
            if (cos_alpha[i]>0){
                
                double f_surf[3];
                aero_force(density, Cd, rel_wind_inertial, w_rel_i, geometry[i], C_b2i, f_surf);
                
                // Sum total force
                for (int j=0; j<3; j++)
                    f_aero[j] = f_aero[j] + f_surf[j];
            }
        }
        // Acceleration
        for (int i=0; i<3; i++)
            a_aero[i] = f_aero[i]/m;
    
    }
    
    //// TORQUE CALCULATION
    if (in_aero_g){
        
        // Calculate for each surface
        for (int i=0; i<n_surf; i++){
            
            // If in view
            if (cos_alpha[i]>0){
                
                double g_surf[3];
                aero_torque(density, Cd, rel_wind_body, w_rel_b, geometry[i], g_surf);
                
                // Sum total torque
                for (int j=0; j<3; j++)
                    g_aero[j] = g_aero[j] + g_surf[j];
            }
        }

    }
}
