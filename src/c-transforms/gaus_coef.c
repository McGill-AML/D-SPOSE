//
//  gaus_coef.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-08-22.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gaus_coef.c
//%
//% DESCRIPTION:          This function will reformat the IGRF-12 coefficients
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 August 22, 2017
//% VERSION:              1
//%
//% INPUT:                double mag_coef[195][27]: IGRF-12 coefficients form input
//%
//% OUTPUT:               double G[14][14][25]: magnetic potential coefficients
//%                       double H[14][14][25]: magnetic potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "gaus_coef.h"
#include <math.h>

void gaus_coef(double mag_coef[195][27], double G[14][14][25], double H[14][14][25]){
    
    for (int i=0; i<14; i++){
        for (int j=0; j<14; j++){
            for (int k=0; k<26; k++){
                G[i][j][k] = 0;
                H[i][j][k] = 0;
            }
        }
    }
    
    int k = 0;
    
    for (int i=0; i<13; i++){
        
        for (int j=0; j<25; j++)
            G[(int)mag_coef[k][0]][(int)mag_coef[k][1]][j] = mag_coef[k][j+2];
        k=k+1;
        
        for (int l=0; l<=i; l++){
            for (int j=0; j<25; j++)
                G[(int)mag_coef[k][0]][(int)mag_coef[k][1]][j] = mag_coef[k][j+2];
            k=k+1;
            for (int j=0; j<25; j++)
                H[(int)mag_coef[k][0]][(int)mag_coef[k][1]][j] = mag_coef[k][j+2];
            k=k+1;
        }
    }
    
}
