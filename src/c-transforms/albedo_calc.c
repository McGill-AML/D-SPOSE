//
//  albedo_calc.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-07-02.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        albedo_calc.c
//%
//% DESCRIPTION:          This function calculates the acceleration and
//%                       torque due to reflected and emitted Earth radiation
//%                       (See Section 2.3.3 in Sagnieres (2018) Doctoral Thesis)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 July 2, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double v[3]: 3x1 velocity vector (m)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[0]: geodetic latitude (rad)
//%                         - LLA[0]: longitude (rad)
//%                         - LLA[0]: altitude (km)
//%                       int time[3]:
//%                         - time[0]: year
//%                         - time[1]: day of year
//%                         - time[2]: days since January 1, 2000
//%                       double r_sun[3]: position of Sun (m)
//%                       double C_ecef2teme[3][3]: rotation matrix from ECEF
//%                         frame to TEME
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       int n_surf: number of surfaces in geometry model
//%                       struct surface geometry[n_surf]: surface geometry model
//%                       double m: mass (kg)
//%                       double albedo[12][20][40][2]: albedo and IR coefficients
//%                       int in_alb_a: inclusion of albedo acceleration?
//%                       int in_alb_g: inclusion of albedo torque?
//%                       int in_ir_a: inclusion of IR acceleration?
//%                       int in_ir_g: inclusion of IR torque?
//%
//% OUTPUT:               double a_alb[3]: reflected Earth radiation acceleration
//%                         in inertial frame
//%                       double g_alb[3]: reflected Earth radiation torque
//%                         in body-fixed frame
//%                       double a_in[3]: emitted Earth radiation acceleration
//%                         in inertial frame
//%                       double g_in[3]: emitted Earth radiation torque
//%                         in body-fixed frame
//%
//% COUPLING:             - surface.h
//%                       - vectors2angle.c
//%                       - norm.c
//%                       - crossprod.c
//%                       - matxvec.c
//%                       - invertmat.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "albedo_calc.h"
#include "surface.h"
#include "matxvec.h"
#include "vectors2angle.h"
#include "norm.h"
#include "crossprod.h"
#include <math.h>
#include "invertmat.h"

void albedo_calc(double p[3], double v[3], double LLA[4], int time[3], double r_sun[3], double C_ecef2teme[3][3], double C_i2b[3][3], int n_surf, struct surface geometry[n_surf], double m, double albedo[12][20][40][2], int in_alb_a, int in_alb_g, int in_ir_a, int in_ir_g, double a_alb[3], double g_alb[3], double a_ir[3], double g_ir[3]){
    
    for (int i=0; i<3; i++){
        a_alb[i] = 0;
        g_alb[i] = 0;
        a_ir[i] = 0;
        g_ir[i] = 0;
    }
    
    // Initialize acceleration for each surface
    double f_surf[n_surf][2][3];
    for (int k=0; k<n_surf; k++){
        for (int i=0; i<3; i++){
            f_surf[k][0][i] = 0;
            f_surf[k][1][i] = 0;
        }
    }
    
    // Surface area for each Grid (9° x 9°) as a function of latitude (90-81 to 9-0) independent of longitude (in m^2)
    double grid_area[20] = {79189845238,
        235467450694,
        385524392958,
        525488995462,
        651870341413,
        761652997845,
        852348921372,
        922010908066,
        969218205729,
        993048212774,
        993048212774,
        969218205729,
        922010908066,
        852348921372,
        761652997845,
        651870341413,
        525488995462,
        385524392958,
        235467450694,
        79189845238};
    
    // Earth Equatorial Radius and Eccentricity
    double re         =     6378137;
    double eesqrd     =     0.006694385000;
    
    // Interpolate albedo to current day
    int doy = time[1];
    double albedo_interp[20][40][2];
    for (int i=0; i<20; i++){
        for (int j=0; j<40; j++){
            if (doy<=15){
                albedo_interp[i][j][0] = albedo[11][i][j][0] + (doy + 15) * (albedo[0][i][j][0] - albedo[11][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[11][i][j][1] + (doy + 15) * (albedo[0][i][j][1] - albedo[11][i][j][1]) / 31.0;
            }
            else if (doy<=46){
                albedo_interp[i][j][0] = albedo[0][i][j][0] + (doy - 15) * (albedo[1][i][j][0] - albedo[0][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[0][i][j][1] + (doy - 15) * (albedo[1][i][j][1] - albedo[0][i][j][1]) / 31.0;
            }
            else if (doy<=75){
                albedo_interp[i][j][0] = albedo[1][i][j][0] + (doy - 46) * (albedo[2][i][j][0] - albedo[1][i][j][0]) / 29.0;
                albedo_interp[i][j][1] = albedo[1][i][j][1] + (doy - 46) * (albedo[2][i][j][1] - albedo[1][i][j][1]) / 29.0;
            }
            else if (doy<=106){
                albedo_interp[i][j][0] = albedo[2][i][j][0] + (doy - 75) * (albedo[3][i][j][0] - albedo[2][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[2][i][j][1] + (doy - 75) * (albedo[3][i][j][1] - albedo[2][i][j][1]) / 31.0;
            }
            else if (doy<=136){
                albedo_interp[i][j][0] = albedo[3][i][j][0] + (doy - 106) * (albedo[4][i][j][0] - albedo[3][i][j][0]) / 30.0;
                albedo_interp[i][j][1] = albedo[3][i][j][1] + (doy - 106) * (albedo[4][i][j][1] - albedo[3][i][j][1]) / 30.0;
            }
            else if (doy<=167){
                albedo_interp[i][j][0] = albedo[4][i][j][0] + (doy - 136) * (albedo[5][i][j][0] - albedo[4][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[4][i][j][1] + (doy - 136) * (albedo[5][i][j][1] - albedo[4][i][j][1]) / 31.0;
            }
            else if (doy<=197){
                albedo_interp[i][j][0] = albedo[5][i][j][0] + (doy - 167) * (albedo[6][i][j][0] - albedo[5][i][j][0]) / 30.0;
                albedo_interp[i][j][1] = albedo[5][i][j][1] + (doy - 167) * (albedo[6][i][j][1] - albedo[5][i][j][1]) / 30.0;
            }
            else if (doy<=228){
                albedo_interp[i][j][0] = albedo[6][i][j][0] + (doy - 197) * (albedo[7][i][j][0] - albedo[6][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[6][i][j][1] + (doy - 197) * (albedo[7][i][j][1] - albedo[6][i][j][1]) / 31.0;
            }
            else if (doy<=259){
                albedo_interp[i][j][0] = albedo[7][i][j][0] + (doy - 228) * (albedo[8][i][j][0] - albedo[7][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[7][i][j][1] + (doy - 228) * (albedo[8][i][j][1] - albedo[7][i][j][1]) / 31.0;
            }
            else if (doy<=289){
                albedo_interp[i][j][0] = albedo[8][i][j][0] + (doy - 259) * (albedo[9][i][j][0] - albedo[8][i][j][0]) / 30.0;
                albedo_interp[i][j][1] = albedo[8][i][j][1] + (doy - 259) * (albedo[9][i][j][1] - albedo[8][i][j][1]) / 30.0;
            }
            else if (doy<=320){
                albedo_interp[i][j][0] = albedo[9][i][j][0] + (doy - 289) * (albedo[10][i][j][0] - albedo[9][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[9][i][j][1] + (doy - 289) * (albedo[10][i][j][1] - albedo[9][i][j][1]) / 31.0;
            }
            else if (doy<=350){
                albedo_interp[i][j][0] = albedo[10][i][j][0] + (doy - 320) * (albedo[11][i][j][0] - albedo[10][i][j][0]) / 30.0;
                albedo_interp[i][j][1] = albedo[10][i][j][1] + (doy - 320) * (albedo[11][i][j][1] - albedo[10][i][j][1]) / 30.0;
            }
            else {
                albedo_interp[i][j][0] = albedo[11][i][j][0] + (doy - 350) * (albedo[0][i][j][0] - albedo[11][i][j][0]) / 31.0;
                albedo_interp[i][j][1] = albedo[11][i][j][1] + (doy - 350) * (albedo[0][i][j][1] - albedo[11][i][j][1]) / 31.0;
            }
        }
    }
    
    // Calculate grid center position in ECEF (approximate outward surface normal as same divided by radius)
    double grid_position_ecef[20][40][3];
    double grid_position_eci[20][40][3];
    double norm_grid_position[20][40];
    double unit_grid_eci[20][40][3];
    for (int i=0; i<20; i++){
        for (int j=0; j<40; j++){
            grid_position_ecef[i][j][0] = re*cos((90-4.5-9*i)*M_PI/180.0)*cos((4.5+9*j)*M_PI/180.0);
            grid_position_ecef[i][j][1] = re*cos((90-4.5-9*i)*M_PI/180.0)*sin((4.5+9*j)*M_PI/180.0);
            grid_position_ecef[i][j][2] = re*sqrt(1-eesqrd)*sin((90-4.5-9*i)*M_PI/180.0);
            
            norm_grid_position[i][j] = norm(grid_position_ecef[i][j]);
            
            matxvec(C_ecef2teme, grid_position_ecef[i][j], grid_position_eci[i][j]);
            
            unit_grid_eci[i][j][0] = grid_position_eci[i][j][0]/norm_grid_position[i][j];
            unit_grid_eci[i][j][1] = grid_position_eci[i][j][1]/norm_grid_position[i][j];
            unit_grid_eci[i][j][2] = grid_position_eci[i][j][2]/norm_grid_position[i][j];
        }
    }
    
    // Inverse body rotation matrix
    double C_b2i[3][3];
    invertmat(C_i2b,C_b2i);
    
    // Solar Flux
    double rsun = norm(r_sun);
    double phi = 1361*(149597870700.0/rsun)*(149597870700.0/rsun);
    
    // For each Earth grid
    for (int i=0; i<20; i++){
        for (int j=0; j<40; j++){
            
            // Calculate grid-satellite distance
            double grid_sat_pos[3];
            for (int l=0; l<3; l++){
                grid_sat_pos[l] = p[l] - grid_position_eci[i][j][l];
            }
            double grid_sat_dist = norm(grid_sat_pos);
            
            // Check if grid is in view of satellite
            double grid_view_sat = vectors2angle(unit_grid_eci[i][j],grid_sat_pos);
            double cos_grid_view_sat = cos(grid_view_sat);
            
            // If grid is in view of satellite
            if (cos_grid_view_sat>0){
                
                // Check if Earth grid is in view of sun
                double grid_view_sun = vectors2angle(unit_grid_eci[i][j],r_sun);
                double cos_grid_view_sun = cos(grid_view_sun);
                
                // For every satellite surface
                for (int k=0; k<n_surf; k++){
                    
                    // Rotate surface unit normal in ECI frame
                    double surf_unit_eci[3];
                    matxvec(C_b2i, geometry[k].normal, surf_unit_eci);
                    
                    // Check if surface is in view of Earth grid
                    double grid_view_surf = vectors2angle(unit_grid_eci[i][j],surf_unit_eci);
                    double cos_grid_view_surf = cos(grid_view_surf);
                    
                    // If surface is in view of Earth grid
                    if (cos_grid_view_surf>0){
                        
                        // Optical properties
                        double crd = geometry[k].crd;
                        double crs = geometry[k].crs;
                        double ca = geometry[k].ca;
                        
                        // Optical properties
                        double crd_ir = geometry[k].crd_ir;
                        double crs_ir = geometry[k].crs_ir;
                        double ca_ir = geometry[k].ca_ir;
                        
                        // Speed of light
                        double c = 299792458.0;
                        
                        // Projected surface area
                        double Ap = geometry[k].area*cos_grid_view_surf;
                        
                        // Calculate IR acceleration for that surface from that surface grid in ECI
                        if (in_ir_a || in_ir_g){
                            
                            // Calculate flux
                            double flux_ir = phi*albedo_interp[i][j][1]/1000.0*cos_grid_view_sat*grid_area[i]/(4*M_PI*grid_sat_dist*grid_sat_dist);
                            
                            // Calculate force on that surface
                            f_surf[k][1][0] = f_surf[k][1][0] + flux_ir/c * Ap*((ca_ir+crd_ir)*unit_grid_eci[i][j][0] + (2*crd_ir/3.0 + 2*crs_ir*cos_grid_view_surf) *surf_unit_eci[0]);
                            f_surf[k][1][1] = f_surf[k][1][1] + flux_ir/c * Ap*((ca_ir+crd_ir)*unit_grid_eci[i][j][1] + (2*crd_ir/3.0 + 2*crs_ir*cos_grid_view_surf) *surf_unit_eci[1]);
                            f_surf[k][1][2] = f_surf[k][1][2] + flux_ir/c * Ap*((ca_ir+crd_ir)*unit_grid_eci[i][j][2] + (2*crd_ir/3.0 + 2*crs_ir*cos_grid_view_surf) *surf_unit_eci[2]);
                        }
                        
                        // If Earth grid is in view of sun
                        if (cos_grid_view_sun>0){
                            
                            // Calculate albedo acceleration for that surface from that surface grid in ECI
                            if (in_alb_a || in_alb_g){
                                
                                // Calculate flux
                                double flux_alb = phi*albedo_interp[i][j][0]/1000.0*cos_grid_view_sat*cos_grid_view_sun*grid_area[i]/(M_PI*grid_sat_dist*grid_sat_dist);
                                
                                // Calculate force on that surface
                                f_surf[k][0][0] = f_surf[k][0][0] + flux_alb/c * Ap*((ca+crd)*unit_grid_eci[i][j][0] + (2*crd/3.0 + 2*crs*cos_grid_view_surf) *surf_unit_eci[0]);
                                f_surf[k][0][1] = f_surf[k][0][1] + flux_alb/c * Ap*((ca+crd)*unit_grid_eci[i][j][1] + (2*crd/3.0 + 2*crs*cos_grid_view_surf) *surf_unit_eci[1]);
                                f_surf[k][0][2] = f_surf[k][0][2] + flux_alb/c * Ap*((ca+crd)*unit_grid_eci[i][j][2] + (2*crd/3.0 + 2*crs*cos_grid_view_surf) *surf_unit_eci[2]);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Calculate total IR acceleration
    if (in_ir_a){
        for (int k=0; k<n_surf; k++){
            for (int i=0; i<3; i++){
                a_ir[i] = a_ir[i]+f_surf[k][1][i]/m;
            }
        }
    }
    
    // Calculate total albedo acceleration
    if (in_alb_a){
        for (int k=0; k<n_surf; k++){
            for (int i=0; i<3; i++){
                a_alb[i] = a_alb[i]+f_surf[k][0][i]/m;
            }
        }
    }
    
    // Calculate center of pressure
    double cp_b[n_surf][3];
    for (int i=0; i<n_surf; i++){
        cp_b[i][0] = (geometry[i].vertices[0][0] + geometry[i].vertices[0][1] + geometry[i].vertices[0][2])/3.0;
        cp_b[i][1] = (geometry[i].vertices[1][0] + geometry[i].vertices[1][1] + geometry[i].vertices[1][2])/3.0;
        cp_b[i][2] = (geometry[i].vertices[2][0] + geometry[i].vertices[2][1] + geometry[i].vertices[2][2])/3.0;
    }
    
    double g_surf[3], f_surf_body[3];
    // Calculate IR torque for each surface in BODY
    if (in_ir_g){
        for (int k=0; k<n_surf; k++){
            
            // Force in body frame
            matxvec(C_i2b,f_surf[k][1],f_surf_body);
            
            // Calculate torque
            crossprod(cp_b[k], f_surf_body, g_surf);
            
            for (int i=0; i<3; i++){
                g_ir[i] = g_ir[i]+g_surf[i];
            }
        }
    }
    
    // Calculate albedo torque for each surface in BODY
    if (in_alb_g){
        for (int k=0; k<n_surf; k++){
            
            // Force in body frame
            matxvec(C_i2b,f_surf[k][0],f_surf_body);
            
            // Calculate torque
            crossprod(cp_b[k], f_surf_body, g_surf);
            
            for (int i=0; i<3; i++){
                g_alb[i] = g_alb[i]+g_surf[i];
            }
        }
    }
    
}
