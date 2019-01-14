//
//  nutation.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        nutation.c
//%
//% DESCRIPTION:          This function outputs the rotation matrix from the
//%                       TOD frame to the MOD frame using 1976 Delaunay
//%                       parameters following Vallado (2013) Section 3.7
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double ttt: julian centuries of terrestrial time
//%                       double iar80[106][5]: nutation parameters
//%                       double rar80[106][4]: nutation parameters
//%                       double ddpsi: EOP corrections
//%                       double ddeps: EOP corrections
//%
//% OUTPUT:               double nut[3][3]: rotation matrix from TOD to MOD
//%                       double nut_param[3]:
//%                         - nut_param[0]: nutation angle
//%                         - nut_param[1]: mean obliquity of the ecliptic
//%                         - nut_param[2]: Delaunay parameter Omega
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "nutation.h"
#include <math.h>

void nutation(double ttt, double iar80[106][5], double rar80[106][4], double ddpsi, double ddeps, double nut_param[3], double nut[3][3]){
    
    double deg2rad = M_PI/180.0;
    
    double ttt2= ttt*ttt;
    double ttt3= ttt2*ttt;
    
    double meaneps = -46.8150 *ttt - 0.00059 *ttt2 + 0.001813 *ttt3 + 84381.448;
    meaneps = fmod( meaneps/3600.0 ,360.0  );
    meaneps = meaneps * deg2rad;
    
    // Delaunay Parameters (rad)
    double l    =  134.96298139  + ( 1717915922.6330 *ttt + 31.310 *ttt2 + 0.064 *ttt3 ) / 3600.0;
    double l1   =  357.52772333  + (  129596581.2240 *ttt - 0.577 *ttt2 - 0.012 *ttt3 )  / 3600.0;
    double f    =   93.27191028  + ( 1739527263.1370 *ttt - 13.257 *ttt2 + 0.011 *ttt3 )  / 3600.0;
    double d    =  297.85036306  + ( 1602961601.3280 *ttt - 6.891 *ttt2 + 0.019 *ttt3 )  / 3600.0;
    double omega=  125.04452222  + (   -6962890.5390 *ttt + 7.455 *ttt2 + 0.008 *ttt3 )  / 3600.0;
    l    = fmod( l , 360.0  )     * deg2rad;
    l1   = fmod( l1 , 360.0  )    * deg2rad;
    f    = fmod( f , 360.0  )     * deg2rad;
    d    = fmod( d , 360.0  )     * deg2rad;
    omega= fmod( omega , 360.0  ) * deg2rad;
    
    double deltapsi= 0.0;
    double deltaeps= 0.0;
    double tempval;
    for (int i = 0; i<106; i++){
        tempval= iar80[i][0]*l + iar80[i][1]*l1 + iar80[i][2]*f + iar80[i][3]*d + iar80[i][4]*omega;
        deltapsi= deltapsi + (rar80[i][0]+rar80[i][1]*ttt) * sin( tempval );
        deltaeps= deltaeps + (rar80[i][2]+rar80[i][3]*ttt) * cos( tempval );
    }
    deltapsi = fmod( deltapsi +ddpsi, 2*M_PI  );
    deltaeps = fmod( deltaeps +ddeps, 2*M_PI  );
    
    double trueeps  = meaneps + deltaeps;
    
    double cospsi  = cos(deltapsi);
    double sinpsi  = sin(deltapsi);
    double coseps  = cos(meaneps);
    double sineps  = sin(meaneps);
    double costrueeps = cos(trueeps);
    double sintrueeps = sin(trueeps);
    
    nut[0][0] =  cospsi;
    nut[0][1] =  costrueeps * sinpsi;
    nut[0][2] =  sintrueeps * sinpsi;
    nut[1][0] = -coseps * sinpsi;
    nut[1][1] =  costrueeps * coseps * cospsi + sintrueeps * sineps;
    nut[1][2] =  sintrueeps * coseps * cospsi - sineps * costrueeps;
    nut[2][0] = -sineps * sinpsi;
    nut[2][1] =  costrueeps * sineps * cospsi - sintrueeps * coseps;
    nut[2][2] =  sintrueeps * sineps * cospsi + costrueeps * coseps;
    
    nut_param[0] = deltapsi;
    nut_param[1] = meaneps;
    nut_param[2] = omega;
    
}
