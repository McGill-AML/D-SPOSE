//
//  sun_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sun_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the Sun
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double r_sun[3]: 3x1 Sun position vector (m)
//%
//% OUTPUT:               double U_sun: potential energy due to Sun (J)
//%
//% COUPLING:             - vectors2angle.c
//%                       - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sun_potential.h"
#include <math.h>
#include "vectors2angle.h"
#include "norm.h"

double sun_potential(double p[3], double m, double r_sun[3]){
    
    double d = norm(p);
    double d_sun = norm(r_sun);
    double mu_sun = 1.32712428*pow(10,20);
    double thetasun = vectors2angle(r_sun,p);
    double dratios = d/d_sun;
    double cos_sun = cos(thetasun);
    double P2 = (3*cos_sun*cos_sun - 1)/2.0;
    double P3 = (5*cos_sun*cos_sun*cos_sun - 3*cos_sun)/2.0;
    double P4 = (35*cos_sun*cos_sun*cos_sun*cos_sun - 30*cos_sun*cos_sun + 3)/8.0;
    double P5 = (63*cos_sun*cos_sun*cos_sun*cos_sun*cos_sun - 70*cos_sun*cos_sun*cos_sun + 15*cos_sun)/8.0;
    double Rsun = dratios*dratios*P2 + dratios*dratios*dratios*P3 + dratios*dratios*dratios*dratios*P4 + dratios*dratios*dratios*dratios*dratios*P5;
    
    double U_sun = -mu_sun*m*Rsun/d_sun;
    return U_sun;
}
