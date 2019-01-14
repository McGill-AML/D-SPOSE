//
//  precess.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-24.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        precess.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       MOD frame to the ECI frame using IAU-76/FK5.
//%                       Eqs. 3-88 and 3-89 Vallado (2013) p. 228
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 24, 2017
//% VERSION:              1
//%
//% INPUT:                double ttt: julian centuries of terrestrial time
//%
//% OUTPUT:               double prec[3][3]: rotation matrix from MOD to ECI
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "precess.h"
#include <math.h>

void precess(double ttt, double prec[3][3]){
    
    double convrt = M_PI / (180.0*3600.0);
    double ttt2= ttt * ttt;
    double ttt3= ttt2 * ttt;
    
    double zeta = 2306.2181*ttt + 0.30188*ttt2 + 0.017998*ttt3;
    double theta= 2004.3109*ttt - 0.42665*ttt2 - 0.041833*ttt3;
    double z    =  2306.2181*ttt + 1.09468*ttt2 + 0.018203*ttt3;
    
    // convert units to rad
    zeta = zeta  * convrt;
    theta= theta * convrt;
    z    = z     * convrt;
    
    double coszeta  = cos(zeta);
    double sinzeta  = sin(zeta);
    double costheta = cos(theta);
    double sintheta = sin(theta);
    double cosz     = cos(z);
    double sinz     = sin(z);
    
    prec[0][0] =  coszeta * costheta * cosz - sinzeta * sinz;
    prec[0][1] =  coszeta * costheta * sinz + sinzeta * cosz;
    prec[0][2] =  coszeta * sintheta;
    prec[1][0] = -sinzeta * costheta * cosz - coszeta * sinz;
    prec[1][1] = -sinzeta * costheta * sinz + coszeta * cosz;
    prec[1][2] = -sinzeta * sintheta;
    prec[2][0] = -sintheta * cosz;
    prec[2][1] = -sintheta * sinz;
    prec[2][2] =  costheta;
    
}
