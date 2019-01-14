//
//  check_inputs.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-11.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        check_inputs.c
//%
//% DESCRIPTION:          This function will check the length of input files
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 11, 2016
//% VERSION:              1
//%
//% OUTPUT:               int length_of_file[5]
//%                         - length_of_file[0]: length of file of 'ap_input.txt'
//%                         - length_of_file[1]: length of file of 'solar_input.txt'
//%                         - length_of_file[2]: length of file of 'eop.txt'
//%                         - length_of_file[3]: length of file of 'sun_ephemeris.txt'
//%                         - length_of_file[4]: length of file of 'moon_ephemeris.txt'
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef check_inputs_h
#define check_inputs_h

#include <stdio.h>

void check_inputs(int length_of_file[5]);

#endif /* check_inputs_h */
