//
//  shadow_function.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-12-19.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        shadow_function.c
//%
//% DESCRIPTION:          This function calculates the shadow function for
//%                       a satellite in Earth umbra and penumbra following
//%                       Montenbruck and Gill (2005) Satellite Orbits:
//%                       Models, Methods and Applications
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 December 19, 2017
//% VERSION:              1
//%
//% INPUT:                double r_sun[3]: position of Sun (m)
//%                       double p[3]: spacecraft position (m)
//%
//% OUTPUT:               double nu: portion of light blocked by Earth
//%
//% COUPLING:             - dotprod.c
//%                       - norm.c
//%                       - vectors2angle.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "shadow_function.h"
#include "dotprod.h"
#include "norm.h"
#include <math.h>
#include "vectors2angle.h"

double shadow_function(double r_sun[3], double p[3]){
    
    double nu = 1;
    
    double alpha_umb = 0.264121687*M_PI/180.0;
    double alpha_pen = 0.269007205*M_PI/180.0;
    
    double rrd = dotprod(p,r_sun);
    double rn = norm(p);
    
    if (rrd < 0) {
        
        double r_sun_neg[3];
        for (int i=0; i<3; i++)
            r_sun_neg[i] = -r_sun[i];
        
        double sigma = vectors2angle(r_sun_neg,p);
        double sat_hori = rn*cos(sigma);
        double sat_vert = rn*sin(sigma);
        double R_earth = 6378.137e3;
        
        double xx = R_earth/sin(alpha_pen);
        
        double pen_vert = tan(alpha_pen)*(xx+sat_hori);
        
        if (sat_vert<=pen_vert) {
            
            double yy = R_earth/sin(alpha_umb);
            double umb_vert = tan(alpha_umb)*(yy-sat_hori);
            
            if (sat_vert<=umb_vert)
                nu = 0;
            else {
                
                double R_sun = 6.957e8;
                double rsat = norm(p);
                
                double rsr[3];
                for (int i=0; i<3; i++)
                    rsr[i] = r_sun[i] - p[i];
                double nrsr = norm(rsr);
                
                double a = asin(R_sun/nrsr);
                
                double b = asin(R_earth/rsat);
                
                double sdotr = dotprod(p,rsr);
                
                double c = acos(-sdotr/(rsat*nrsr));
                
                double x = (c*c + a*a - b*b)/(2*c);
                double y = sqrt(a*a-x*x);
                
                double A = a*a*acos(x/a)+b*b*acos((c-x)/b)-c*y;
                
                if ((a+b) <= c)
                    nu = 1;
                else if ((c<(b-a))||(c<(a-b)))
                    nu = 0;
                else
                    nu = 1-A/(M_PI*a*a);
                
            }
        }
    }
    
    return nu;
    
}
