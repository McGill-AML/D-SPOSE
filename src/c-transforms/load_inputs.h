//
//  load_inputs.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        load_inputs.c
//%
//% DESCRIPTION:          This function will load the input files
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 11, 2016
//% VERSION:              1
//%
//% INPUT:                int length_of_file[5]: length of input text files
//%
//% OUTPUT:               double time_parameters[13]: time parameters from input file
//%                       double sc_parameters[33]: spacecraft parameters from input file
//%                       double grav_coef[5148][6]: EGM2008 spherical harmonics coefficients
//%                       double mag_coef[195][27]: IGRF-12 Gauss Coefficients
//%                       double model_parameters[27]: model parameters from input file
//%                       double ap_index[length_of_file[0]]: Ap array for NRLMSISE-00
//%                       double solar_input[length_of_file[1]][3]: F10.7 array for NRLMSISE-00
//%                       double wmm_coef[90][18]: WMM coefficients
//%                       double iar80[106][5]: 1980 IAU Thoery of Nutation coefficients
//%                       double rar80[106][4]: 1980 IAU Thoery of Nutation coefficients
//%                       double eop[length_of_file[2]][10]: Earth Orientation Parameters
//%                       double max_dates[5][3]: Array of maximum propagation dates from loaded indices
//%                       double sun_eph[length_of_file[3]][3]: Sun ephemerides
//%                       double moon_eph[length_of_file[4]][3]: Moon ephemerides
//%                       double albedo[12][20][40][2]: Albedo and IR coefficients
//%
//% COUPLING:             - days2mdh.c
//%                       - t2doy.h
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef load_inputs_h
#define load_inputs_h

#include <stdio.h>

void load_inputs(int length_of_file[5], double time_parameters[13], double sc_parameters[33], double grav_coef[5148][6], double mag_coef[195][27], double model_parameters[27], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double wmm_coef[90][18], double iar80[106][5], double rar80[106][4], double eop[length_of_file[2]][10], double max_dates[5][3], double sun_eph[length_of_file[3]][3], double moon_eph[length_of_file[4]][3], double albedo[12][20][40][2]);

#endif /* load_inputs_h */
