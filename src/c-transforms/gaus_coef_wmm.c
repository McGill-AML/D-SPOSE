//
//  gaus_coef_wmm.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-09-26.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        gaus_coef_wmm.c
//%
//% DESCRIPTION:          This function will reformat the WMM coefficients
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 September 26, 2017
//% VERSION:              1
//%
//% INPUT:                wmm_coef[90][18]: WMM coefficients form input
//%
//% OUTPUT:               double G_wmm[13][13][8]: magnetic potential coefficients
//%                       double H_wmm[13][13][8]: magnetic potential coefficients
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "gaus_coef_wmm.h"
#include <math.h>

void gaus_coef_wmm(double wmm_coef[90][18], double G_wmm[13][13][8], double H_wmm[13][13][8]){
    
    for (int i=0; i<13; i++){
        for (int j=0; j<13; j++){
            for (int k=0; k<8; k++){
                G_wmm[i][j][k] = 0;
                H_wmm[i][j][k] = 0;
            }
        }
    }
    
    for (int i=0; i<90; i++){
        
        for (int j=0; j<8; j++){

            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][0] = wmm_coef[i][2];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][1] = wmm_coef[i][4];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][2] = wmm_coef[i][6];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][3] = wmm_coef[i][8];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][4] = wmm_coef[i][10];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][5] = wmm_coef[i][12];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][6] = wmm_coef[i][14];
            G_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][7] = wmm_coef[i][16];
        
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][0] = wmm_coef[i][3];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][1] = wmm_coef[i][5];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][2] = wmm_coef[i][7];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][3] = wmm_coef[i][9];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][4] = wmm_coef[i][11];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][5] = wmm_coef[i][13];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][6] = wmm_coef[i][15];
            H_wmm[(int)wmm_coef[i][0]][(int)wmm_coef[i][1]][7] = wmm_coef[i][17];
            
        }
    }
    
}
