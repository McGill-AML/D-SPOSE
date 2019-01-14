//
//  aero_force.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-29.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        aero_force.c
//%
//% DESCRIPTION:          This function calculates the force on surface
//%                       due to atmospheric drag for a spinning satellite
//%                       (See Eq. (18) and Appendix B in Sagnieres et al.
//%                       (2018), JGCD)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 29, 2016
//% VERSION:              1
//%
//% INPUT:                double rho: atmospheric density (kg m-3)
//%                       double Cd: drag coefficient
//%                       double v[3]: relative wind speed ECI (m s-1)
//%                       double w_rel_i[3]: relative atmosphere angular
//%                         velocity inertial frame (m s-1)
//%                       struct surface geometry: surface geometry
//%                       double C_b2i[3][3]: rotation matrix from body
//%                         frame to inertial frame
//%
//% OUTPUT:               double f[3]: force on surface due to atmospheric
//%                         drag in inertial frame
//%
//% COUPLING:             - surface.h
//%                       - dotprod.c
//%                       - crossprod.c
//%                       - norm.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <math.h>
#include "aero_force.h"
#include "surface.h"
#include "dotprod.h"
#include "crossprod.h"
#include "matxvec.h"
#include "norm.h"

void aero_force(double rho, double Cd, double v[3], double w_rel_i[3], struct surface geometry, double C_b2i[3][3], double f[3]){
    
    for (int i=0; i<3; i++)
        f[i] = 0;
    
    double center_p[3];
    // Calculate center of pressure in body frame
    center_p[0] = (geometry.vertices[0][0] + geometry.vertices[0][1] + geometry.vertices[0][2])/3.0;
    center_p[1] = (geometry.vertices[1][0] + geometry.vertices[1][1] + geometry.vertices[1][2])/3.0;
    center_p[2] = (geometry.vertices[2][0] + geometry.vertices[2][1] + geometry.vertices[2][2])/3.0;
    
    // Switch everything to inertial frame
    double c_p[3];
    matxvec(C_b2i, center_p, c_p);
    
    double A = geometry.projected_area;
    
    // Surface normal in inertial frame
    double normal[3];
    matxvec(C_b2i,geometry.normal,normal);
    
    ////////// FIRST TERM
    double F1[3] = {0,0,0};
    double drag_force = 1/2.0*Cd*A*rho*norm(v)*norm(v);
    for (int i=0; i<3; i++)
        F1[i] = drag_force * v[i] / norm(v);
    
    ////////// SECOND TERM
    double F2[3] = {0,0,0};
    double cxw[3];
    crossprod(c_p,w_rel_i,cxw);
    for (int i=0; i<3; i++)
        F2[i] = v[i] * rho*geometry.area*dotprod(normal,cxw)*Cd/2.0;
    
    ////////// THIRD TERM
    double F3[3] = {0,0,0};
    for (int i=0; i<3; i++)
        F3[i] = cxw[i] * rho*A*norm(v)*Cd/2.0;

    ////////// SUM UP TERMS
    for (int i=0; i<3; i++)
        f[i] = F1[i] + F2[i] + F3[i];
}
