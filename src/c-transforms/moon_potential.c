//
//  moon_potential.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2018-10-20.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        moon_potential.c
//%
//% DESCRIPTION:          This function calculates the potential energy from
//%                       the moon
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 20, 2018
//% VERSION:              1
//%
//% INPUT:                double p[3]: 3x1 position vector (m)
//%                       double m: mass of spacecraf (kg)
//%                       double r_moon[3]: 3x1 moon position vector (m)
//%
//% OUTPUT:               double U_moon: potential energy due to Moon (J)
//%
//% COUPLING:             - vectors2angle.c
//%                       - norm.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "moon_potential.h"
#include <math.h>
#include "vectors2angle.h"
#include "norm.h"

double moon_potential(double p[3], double m, double r_moon[3]){
    
    double d = norm(p);
    double d_moon = norm(r_moon);
    double mu_moon = 4902.799 * pow(10,9);
    double thetamoon = vectors2angle(r_moon,p);
    double dratios = d/d_moon;
    double cos_moon = cos(thetamoon);
    double P2 = (3*cos_moon*cos_moon - 1)/2.0;
    double P3 = (5*cos_moon*cos_moon*cos_moon - 3*cos_moon)/2.0;
    double P4 = (35*cos_moon*cos_moon*cos_moon*cos_moon - 30*cos_moon*cos_moon + 3)/8.0;
    double P5 = (63*cos_moon*cos_moon*cos_moon*cos_moon*cos_moon - 70*cos_moon*cos_moon*cos_moon + 15*cos_moon)/8.0;
    double Rmoon = dratios*dratios*P2 + dratios*dratios*dratios*P3 + dratios*dratios*dratios*dratios*P4 + dratios*dratios*dratios*dratios*dratios*P5;
    
    double U_moon = -mu_moon*m*Rmoon/d_moon;
    return U_moon;
}
