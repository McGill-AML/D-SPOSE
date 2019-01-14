//
//  third_body.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-11-07.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        third_body.c
//%
//% DESCRIPTION:          This function calculates the acceleration due to a
//%                       third body according to Vallado (2013) p. 574-575
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 7, 2017
//% VERSION:              1
//%
//% INPUT:                double p[3]: position vector
//%                       double p_third[3]: position vector of 3rd body
//%                       double mu_third: 3rd body gravitational parameters
//%
//% OUTPUT:               double a_third[3]: acceleration vector due to 3rd body
//%
//% COUPLING:             - norm.c
//%                       - dotprod.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "third_body.h"
#include "norm.h"
#include <math.h>
#include "dotprod.h"

void third_body(double p[3], double p_third[3], double mu_third, double a_third[3]){
    
    double mu = 3986004.418*pow(10,8);
    
    double p_third_norm = norm(p_third);
    
    double p_sat3[3];
    for(int i=0; i<3; i++)
        p_sat3[i] = p_third[i]-p[i];
    double p_sat3_norm = norm(p_sat3);
    
    double p_norm = norm(p);
    
    double Q = ((p_norm*p_norm+2*dotprod(p,p_sat3)) * (p_third_norm*p_third_norm + p_third_norm*p_sat3_norm + p_sat3_norm*p_sat3_norm)) / (p_third_norm*p_third_norm*p_third_norm*p_sat3_norm*p_sat3_norm*p_sat3_norm*(p_third_norm + p_sat3_norm));
    
    for (int i=0; i<3; i++)
        a_third[i] = mu_third*(Q*p_sat3[i] - p[i]/(p_third_norm*p_third_norm*p_third_norm));
    
}
