//
//  sc_parameters.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-01-13.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sc_parameters.c
//%
//% DESCRIPTION:          This function will load the spacecraft parameters
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 January 13, 2016
//% VERSION:              1
//%
//% INPUT:                double sc_parameters[33]: spacecraft parameters
//%
//% OUTPUT:               double coe[9]: orbital parameters
//%                         - coe[0]: specific angular momentum (m^2 s^-1)
//%                         - coe[1]: eccentricity
//%                         - coe[2]: right ascension (rad)
//%                         - coe[3]: inclination (rad)
//%                         - coe[4]: argument of perigee (rad)
//%                         - coe[5]: true anomaly (rad)
//%                       double Inertia[3][3]: body-frame components of
//%                         matrix representation of inertia tensor
//%                       double I_inv[3][3]: inverse of inertia matrix
//%                       double w[3]: body-frame components of angular
//%                         velocity vector of body-frame w.t.r inertia frame
//%                       double q[4]: quaternion orientation of body-frame
//%                       double M[3][3]: body-frame components of matrix
//%                         representation of magnetic tensor
//%
//% COUPLING:             - invertmat.c
//%                       - angle2quat.c
//%                       - quat2dcm.c
//%                       - orbital2state.c
//%                       - orbital2inertial.c
//%                       - matrixmult.c
//%                       - dcm2angle.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sc_parameters.h"
#include "invertmat.h"
#include "angle2quat.h"
#include "quat2dcm.h"
#include "orbital2state.h"
#include "orbit2inertial.h"
#include "matrixmult.h"
#include "dcm2angle.h"
#include <math.h>

void sc_parameters(double sc_parameters[33], double coe[9], double Inertia[3][3], double I_inv[3][3], double w[3], double q[4], double M[3][3]){
    
    // Constants
    double mu_grav = 3.986004418*pow(10,14);
    
    // Orbital Elements
    coe[1] = sc_parameters[2];                              // eccentricity
    coe[0] = sqrt(sc_parameters[3]*(1-coe[1]*coe[1])*mu_grav);  // angular momentum
    coe[2] = sc_parameters[4]*M_PI/180.0;                   // right ascension of ascending node
    coe[3] = sc_parameters[5]*M_PI/180.0;                   // inclination
    coe[4] = sc_parameters[6]*M_PI/180.0;                   // argument of perigee
    coe[5] = sc_parameters[7]*M_PI/180.0;                   // true anomaly
    
    // Initial angular velocity of body frame w.r.t. inertial frame in body frame (rad)
    w[0] = sc_parameters[8]*M_PI/180.0;
    w[1] = sc_parameters[9]*M_PI/180.0;
    w[2] = sc_parameters[10]*M_PI/180.0;
    // Initial attitude of body frame w.r.t. inertial frame (Euler angles with 3-2-1 rotation) (rad)
    double yaw = sc_parameters[11]*M_PI/180.0;
    double pitch = sc_parameters[12]*M_PI/180.0;
    double roll = sc_parameters[13]*M_PI/180.0;
    angle2quat(yaw, pitch, roll, q);
    
    // If initial attitude is w.r.t. orbital frame
    if (sc_parameters[14]==2){
        double C_o2b[3][3];
        quat2dcm(q,C_o2b);
        
        double p[3],v[3], C_o2i[3][3];
        orbital2state(p,v,coe);
        orbit2inertial(p,v,C_o2i);
        
        double C_i2o[3][3];
        invertmat(C_o2i,C_i2o);
        
        double C_i2b[3][3];
        matrixmult(C_o2b,C_i2o,C_i2b);
        
        double euler[3];
        dcm2angle(C_i2b, euler);
        yaw = euler[0];
        pitch = euler[1];
        roll = euler[2];
        angle2quat(yaw, pitch, roll, q);
    }
    
    // Inertia Matrix
    Inertia[0][0] = sc_parameters[15];
    Inertia[0][1] = sc_parameters[16];
    Inertia[0][2] = sc_parameters[17];
    Inertia[1][0] = sc_parameters[18];
    Inertia[1][1] = sc_parameters[19];
    Inertia[1][2] = sc_parameters[20];
    Inertia[2][0] = sc_parameters[21];
    Inertia[2][1] = sc_parameters[22];
    Inertia[2][2] = sc_parameters[23];
    
    // Inverse of Inertia Matrix
    invertmat(Inertia,I_inv);
    
    // Magnetic Tensor
    M[0][0] = sc_parameters[24];
    M[0][1] = sc_parameters[25];
    M[0][2] = sc_parameters[26];
    M[1][0] = sc_parameters[27];
    M[1][1] = sc_parameters[28];
    M[1][2] = sc_parameters[29];
    M[2][0] = sc_parameters[30];
    M[2][1] = sc_parameters[31];
    M[2][2] = sc_parameters[32];
    
}
