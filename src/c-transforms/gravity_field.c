//
//  gravity_field.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-07.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gravity_field.c
//%
//% DESCRIPTION:          This function calculates the gravitational acceleration
//%                       from aspherical terms and gravity-gradient torque
//%                       (See Sections 2.2.1 and 2.2.2 in Sagnieres (2018)
//%                       Doctoral Thesis)
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 7, 2017
//% VERSION:              1
//%
//% INPUT:                double p_ecef[3]: position in ECEF frame (m)
//%                       double p[3]: 3x1 position vector (m)
//%                       double LLA[4]:
//%                         - LLA[0]: geocentric latitude (rad)
//%                         - LLA[1]: geodetic latitude (rad)
//%                         - LLA[2]: longitude (rad)
//%                         - LLA[3]: altitude (km)
//%                       double C_i2b[3][3]: rotation matrix from inertial
//%                         frame to body frame
//%                       double Inertia[3][3]: inertia matrix
//%                       double C[101][101]: gravity potential coefficients
//%                       double S[101][101]: gravity potential coefficients
//%                       int l_max_a: maximum order and degree in spherical
//%                         harmonic expansion for acceleration calculation
//%                       int l_max_g: maximum order and degree in spherical
//%                         harmonic expansion for torque calculation
//%                       int in_grav_a: inclusion of acceleration?
//%                       int in_grav_g: inclusion of torque?
//%
//% OUTPUT:               double a_gravity_inertial[3]: acceleration due to
//%                         non-spherical Earth in inertial frame
//%                       double g_gravity_body[3]: gravity-gradient torque
//%                         in body-fixed frame
//%
//% COUPLING:             - matrixmult.c
//%                       - matxvec.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "gravity_field.h"
#include <math.h>
#include "matrixmult.h"
#include "matxvec.h"

void gravity_field(double p_ecef[3], double p[3], double LLA[4], double C_i2b[3][3], double Inertia[3][3], double C[101][101], double S[101][101], int l_max_a, int l_max_g, int in_grav_a, int in_grav_g, double a_gravity_inertial[3], double g_gravity_body[3]){
    
    // Use highest maximum degree and order considered for acceleration anf torque
    int l_max = l_max_a;
    if (!in_grav_a)
        l_max = l_max_g;
    else if (in_grav_g && (l_max_g > l_max_a))
        l_max = l_max_g;
    
    // Constants
    double mu = 3986004.418*pow(10,8);
    double a = 6378136.3;
    
    // Initialize
    for (int i=0; i<3; i++){
        a_gravity_inertial[i] = 0;
        g_gravity_body[i] = 0;
    }
    
    // Spherical geocentric distance, longitude and latitude (declination) (m and rad)
    double r = sqrt(p_ecef[0]*p_ecef[0] + p_ecef[1]*p_ecef[1] + p_ecef[2]*p_ecef[2]);
    double lon = LLA[2];
    double lat = asin(p_ecef[2]/sqrt(p_ecef[0]*p_ecef[0]+p_ecef[1]*p_ecef[1]+p_ecef[2]*p_ecef[2]));
    
    // Legendre Polynomials
    double P[103][103];
    P[0][0] = 1;
    P[1][0] = sin(lat);
    P[1][1] = cos(lat);
    for (int l = 0; l <=l_max; l++){
        P[l][l+1] = 0;
        P[l][l+2] = 0;
    }
    for (int l = 2; l <= l_max+2 ; ++l ){
        P[l][0] = ((2*l-1)*sin(lat)*P[l-1][0]-(l-1)*P[l-2][0])/((double)l);
        for (int m = 1; m<l ; ++m )
            P[l][m] = P[l-2][m]+(2*l-1)*cos(lat)*P[l-1][m-1];
        P[l][l] = (2*l-1)*cos(lat)*P[l-1][l-1];
    }
    
    ///// ASPHERICAL ACCELERATION CALCULATION
    if (in_grav_a){
    
        double dUdr = 0; // Doesn't take into account spherical term. Counted later in Propagation Function
        double dUd0 = 0;
        double dUdl = 0;
    
        // Potential partial derivaties
        for (int l = 2; l <=l_max_a ; ++l ){
            for (int m = 0; m<=l; m++){
                dUdr = dUdr + pow(a/r,l)*(l+1)*P[l][m]*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                dUd0 = dUd0 + pow(a/r,l)*(P[l][m+1]-m*tan(lat)*P[l][m])*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                dUdl = dUdl + pow(a/r,l)*m*P[l][m]*(S[l][m]*cos(m*lon)-C[l][m]*sin(m*lon));
            }
        }
        dUdr = -dUdr*mu/(r*r);
        dUd0 = dUd0*mu/r;
        dUdl = dUdl*mu/r;
    
        ////// Acceleration in TEME Frame
        a_gravity_inertial[0] = (dUdr/r-p[2]/(r*r*sqrt(p[0]*p[0]+p[1]*p[1]))*dUd0)*p[0] - (dUdl/(p[0]*p[0]+p[1]*p[1]))*p[1];
        a_gravity_inertial[1] = (dUdr/r-p[2]/(r*r*sqrt(p[0]*p[0]+p[1]*p[1]))*dUd0)*p[1] + (dUdl/(p[0]*p[0]+p[1]*p[1]))*p[0];
        a_gravity_inertial[2] = dUdr/r*p[2] + sqrt(p[0]*p[0]+p[1]*p[1])/(r*r)*dUd0;
        
    }
    
    
    ////// TORQUE CALCULATION
    if (in_grav_g){

        // Matrices
    
        double i2j2 = p[0]*p[0]+p[1]*p[1];
    
        double d2rdr2[3][3] = {
            {p[0]*p[0]-r*r, p[0]*p[1],      p[0]*p[2]},
            {p[0]*p[1],     p[1]*p[1]-r*r,  p[1]*p[2]},
            {p[0]*p[2],     p[1]*p[2],      p[2]*p[2]-r*r}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                d2rdr2[i][j] = -d2rdr2[i][j]/(r*r*r);
        }
    
        double d20dr2[3][3] = {
            {p[2]*(2*pow(p[0],4)+p[0]*p[0]*p[1]*p[1]-pow(p[1],4)-p[1]*p[1]*p[2]*p[2]),
                p[0]*p[1]*p[2]*(3*i2j2+p[2]*p[2]),
                -p[0]*i2j2*(i2j2-p[2]*p[2])},
            {p[0]*p[1]*p[2]*(3*i2j2+p[2]*p[2]),
                p[2]*(-pow(p[0],4)+p[0]*p[0]*p[1]*p[1]-p[0]*p[0]*p[2]*p[2]+2*pow(p[1],4)),
                -p[1]*i2j2*(i2j2-p[2]*p[2])},
            {-p[0]*i2j2*(i2j2-p[2]*p[2]),
                -p[1]*i2j2*(i2j2-p[2]*p[2]),
                -2*p[2]*i2j2*i2j2}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                d20dr2[i][j] = d20dr2[i][j]/(pow(r,4)*pow(i2j2,3/2.0));
        }
    
        double d2ldr2[3][3] = {
            {2*p[0]*p[1],           p[1]*p[1]-p[0]*p[0],    0},
            {p[1]*p[1]-p[0]*p[0],   -2*p[0]*p[1],           0},
            {0,                     0,                      0}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                d2ldr2[i][j] = d2ldr2[i][j]/(i2j2*i2j2);
        }
    
        double drdrT[3][3] = {
            {p[0]*p[0], p[0]*p[1],  p[0]*p[2]},
            {p[0]*p[1], p[1]*p[1],  p[1]*p[2]},
            {p[0]*p[2], p[2]*p[1],  p[2]*p[2]}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                drdrT[i][j] = drdrT[i][j]/(r*r);
        }
    
        double d0d0T[3][3] = {
            {p[0]*p[0]*p[2]*p[2],   p[0]*p[1]*p[2]*p[2],    -p[0]*p[2]*i2j2},
            {p[0]*p[1]*p[2]*p[2],   p[1]*p[1]*p[2]*p[2],    -p[1]*p[2]*i2j2},
            {-p[0]*p[2]*i2j2,       -p[1]*p[2]*i2j2,        i2j2*i2j2}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                d0d0T[i][j] = d0d0T[i][j]/(r*r*r*r*i2j2*i2j2);
        }
    
        double dldlT[3][3] = {
            {p[1]*p[1],     -p[0]*p[1], 0},
            {-p[0]*p[1],    p[0]*p[0],  0},
            {0,             0,          0}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                dldlT[i][j] = dldlT[i][j]/(i2j2*i2j2);
        }
    
        double drd0T2[3][3] = {
            {-2*p[0]*p[0]*p[2],         -2*p[0]*p[1]*p[2],          p[0]*i2j2-p[0]*p[2]*p[2]},
            {-2*p[0]*p[1]*p[2],         -2*p[1]*p[1]*p[2],          p[1]*i2j2-p[1]*p[2]*p[2]},
            {p[0]*i2j2-p[0]*p[2]*p[2],  p[1]*i2j2-p[1]*p[2]*p[2],   2*p[2]*i2j2}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                drd0T2[i][j] = drd0T2[i][j]/(r*r*r*sqrt(i2j2));
        }
    
        double drdlT2[3][3] = {
            {-2*p[0]*p[1],          p[0]*p[0]-p[1]*p[1],    -p[1]*p[2]},
            {p[0]*p[0]-p[1]*p[1],   2*p[0]*p[1],            p[0]*p[2]},
            {-p[1]*p[2],            p[0]*p[2],              0}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                drdlT2[i][j] = drdlT2[i][j]/(r*i2j2);
        }
    
        double dld0T2[3][3] = {
            {2*p[0]*p[1]*p[2],              p[2]*(p[1]*p[1]-p[0]*p[0]), -p[1]*i2j2},
            {p[2]*(p[1]*p[1]-p[0]*p[0]),    -2*p[0]*p[1]*p[2],          p[0]*i2j2},
            {-p[1]*i2j2,                    p[0]*i2j2,                  0}
        };
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                dld0T2[i][j] = dld0T2[i][j]/(r*r*pow(i2j2,3/2.0));
        }
    
        // First and Second partial derivatives
        double dUdr = 1;   // Takes into account spherical term
        double dUd0 = 0;
        double dUdl = 0;
        double d2Udr2 = 2; // Takes into account spherical term
        double d2Ud02 = 0;
        double d2Udl2 = 0;
        double d2Ud0dr = 0;
        double d2Udldr = 0;
        double d2Udld0 = 0;
    
        for (int l = 2; l <=l_max_g ; ++l ){
            for (int m = 0; m<=l; m++){
                dUdr = dUdr + pow(a/r,l)*(l+1)*P[l][m]*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                dUd0 = dUd0 + pow(a/r,l)*(P[l][m+1]-m*tan(lat)*P[l][m])*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                dUdl = dUdl + pow(a/r,l)*m*P[l][m]*(S[l][m]*cos(m*lon)-C[l][m]*sin(m*lon));
            
                d2Udr2 = d2Udr2 + pow(a/r,l)*(l+1)*(l+2)*P[l][m]*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                d2Ud02 = d2Ud02 + pow(a/r,l)*(P[l][m+2]-(2*m+1)*tan(lat)*P[l][m+1]+m*(m*tan(lat)-(1/(cos(lat)*cos(lat))))*P[l][m])*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                d2Udl2 = d2Udl2 + pow(a/r,l)*m*m*P[l][m]*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
            
                d2Ud0dr = d2Ud0dr + pow(a/r,l)*(l+1)*(P[l][m+1]-m*tan(lat)*P[l][m])*(C[l][m]*cos(m*lon)+S[l][m]*sin(m*lon));
                d2Udldr = d2Udldr + pow(a/r,l)*m*(l+1)*P[l][m]*(S[l][m]*cos(m*lon)-C[l][m]*sin(m*lon));
                d2Udld0 = d2Udld0 + pow(a/r,l)*m*(P[l][m+1]-m*tan(lat)*P[l][m])*(S[l][m]*cos(m*lon)-C[l][m]*sin(m*lon));
            }
        }
    
        dUdr = -dUdr*mu/(r*r);
        dUd0 = dUd0*mu/r;
        dUdl = dUdl*mu/r;
    
        d2Udr2 = d2Udr2*mu/(r*r*r);
        d2Ud02 = d2Ud02*mu/r;
        d2Udl2 = -d2Udl2*mu/r;
    
        d2Ud0dr = -d2Ud0dr*mu/(r*r);
        d2Udldr = -d2Udldr*mu/(r*r);
        d2Udld0 = d2Udld0*mu/r;
    
        // Acceleration Derivative in TEME Frame
    
        double dadr[3][3];
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                dadr[i][j] = d2Udr2*drdrT[i][j] + dUdr*d2rdr2[i][j] + d2Ud02*d0d0T[i][j] + d2Udl2*dldlT[i][j] + d2Ud0dr*drd0T2[i][j] + d2Udldr*drdlT2[i][j] + d2Udld0*dld0T2[i][j] + dUd0*d20dr2[i][j] + dUdl*d2ldr2[i][j];
            }
        }
    
        // Rotate into body-fixed frame
        double dadr_b[3][3], G[3][3];
        matrixmult(C_i2b,dadr,dadr_b);
        double C_b2i[3][3];
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++)
                C_b2i[i][j] = C_i2b[j][i];
        }
        matrixmult(dadr_b,C_b2i,G);
    
        // Gravity-Gradient Torque in body-fixed frame
        g_gravity_body[0] = G[1][2]*(Inertia[2][2]-Inertia[1][1]) - G[0][2]*Inertia[0][1] + G[0][1]*Inertia[0][2] + Inertia[1][2]*(G[1][1]-G[2][2]);
        g_gravity_body[1] = G[0][2]*(Inertia[0][0]-Inertia[2][2]) + G[1][2]*Inertia[0][1] - G[0][1]*Inertia[1][2] + Inertia[0][2]*(G[2][2]-G[0][0]);
        g_gravity_body[2] = G[0][1]*(Inertia[1][1]-Inertia[0][0]) - G[1][2]*Inertia[0][2] + G[0][2]*Inertia[1][2] + Inertia[0][1]*(G[0][0]-G[1][1]);
    }
    
}
