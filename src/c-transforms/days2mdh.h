//
//  days2mdh.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2017-10-24.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        days2mdh.c
//%
//% DESCRIPTION:          This function converts the day of year to month,
//%                       day of month, hour of day, minute of hour, and
//%                       second of minute following Vallado (2013) Section 3.6
//%
//% AUTHOR:               Luc Sagnieres (following Vallado algorithm)
//% DATE:                 October 24, 2017
//% VERSION:              1
//%
//% INPUT:                int year
//%                       double days: day of year in fraction
//%
//% OUTPUT:               double time[5]:
//%                         - time[0]: month
//%                         - time[1]: day of month
//%                         - time[2]: hour of day
//%                         - time[3]: minute of hour
//%                         - time[4]: second of minute
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef days2mdh_h
#define days2mdh_h

#include <stdio.h>

void days2mdh(int year, double days, double time[5]);

#endif /* days2mdh_h */
