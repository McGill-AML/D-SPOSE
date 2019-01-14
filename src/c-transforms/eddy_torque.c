//
//  eddy_torque.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-12.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        eddy_torque.c
//%
//% DESCRIPTION:          This function calculates the torque due to eddy-
//%                       currents (See Eq. (2.20) in Sagnieres (2018)
//%                       Doctoral Thesis)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 12, 2017
//% VERSION:              1
//%
//% INPUT:                double B_field_b[3]: magnetic field vector in body frame
//%                       double B_field_dot_b[3]: time derivative of magnetic field
//%                         vector in inertial frame as seen from orbiting
//%                         spacecraft, in body-frame components
//%                       double w[3]: angular velocity vector in body frame
//%                       double M[3][3]: magnetic tensor
//%
//% OUTPUT:               double g_eddy[3]: eddy_current torque
//%
//% COUPLING:             - crossprod.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "eddy_torque.h"
#include "crossprod.h"
#include "matxvec.h"

void eddy_torque(double B_field_b[3], double B_field_dot_b[3], double w[3], double M[3][3], double g_eddy[3]){
    
    double wxB[3], MwxB[3];
    crossprod(w,B_field_b,wxB);
    double Bdiff[3];
    for (int i=0; i<3; i++)
        Bdiff[i] = wxB[i]-B_field_dot_b[i];
    matxvec(M,Bdiff,MwxB);
    crossprod(MwxB,B_field_b,g_eddy);
    
}
