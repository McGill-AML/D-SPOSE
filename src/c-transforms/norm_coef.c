//
//  norm_coef.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-08-22.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        norm_coef.c
//%
//% DESCRIPTION:          This function will reformat the spherical harmonic
//%                       coefficients for the gravitational potential
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 August 22, 2017
//% VERSION:              1
//%
//% INPUT:                grav_coef[5148][6]: normalized gravity coefficients form input
//%
//% OUTPUT:               double C[101][101]: unnormalized gravity potential coefficients
//%                       double S[101][101]: unnormalized gravity potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "norm_coef.h"
#include <math.h>

void norm_coef(double grav_coef[5148][6], double C[101][101], double S[101][101]){
    
    for (int i=0; i<5148; i++){
        C[(int)grav_coef[i][0]][(int)grav_coef[i][1]] = grav_coef[i][2];
        S[(int)grav_coef[i][0]][(int)grav_coef[i][1]] = grav_coef[i][3];
    }
    
    int k;
    double N;
    
    for (int i=0; i<101; i++){
        for (int j=0; j<=i; j++){
            
            double temp = 1;
            
            if (j==0)
                k = 1;
            else {
                k = 2;
                for (int m=i-j+1; m<=i+j; m++)
                    temp = temp*sqrt(m);
            }
            
            N = sqrt(1.0/(k*(2.0*i+1)))*temp;
            
            C[i][j] = C[i][j]/N;
            S[i][j] = S[i][j]/N;
        }
    }
}
