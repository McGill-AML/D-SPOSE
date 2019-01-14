//
//  load_teme.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-27.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        load_teme.c
//%
//% DESCRIPTION:          This function will load the initial conditions and
//%                       initialize initial orbit
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 October 27, 2017
//% VERSION:              1
//%
//% INPUT:                double eop[6479][10]: earth orientation parameters
//%                       double model_parameters[28]: model parameters
//%                       double max_dates[5][3]: maximum dates from input files
//%
//% OUTPUT:               double sc_parameters[33]:
//%                         - sc_parameters[2]: eccentricity
//%                         - sc_parameters[3]: semi-major axis (m)
//%                         - sc_parameters[4]: right ascension (deg)
//%                         - sc_parameters[5]: inclination (deg)
//%                         - sc_parameters[6]: argument of perigee (deg)
//%                         - sc_parameters[7]: true anomaly (deg)
//%                       double time_parameters[27]:
//%                         - time_parameters[0]: days since January 1, 2000
//%                         - time_parameters[1]: hour of day
//%                         - time_parameters[2]: minute of hour
//%                         - time_parameters[3]: seconds of minute
//%
//% COUPLING:             - days2mdh.c
//%                       - t2doy.c
//%                       - state2orbital.c
//%                       - orbital2state.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef load_teme_h
#define load_teme_h

#include <stdio.h>

void load_teme(double eop[6479][10], double sc_parameters[33], double time_parameters[13], double model_parameters[27], double max_dates[5][3]);

#endif /* load_teme_h */
