//
//  days2mdh.c
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

#include <math.h>
#include "days2mdh.h"

void days2mdh(int year, double days, double time[5]){
    
    // Set up list containing number of days in each month
    int length_month[12];
    for (int i=0; i<12; i++){
        length_month[i] = 31;
        if (i==1)
            length_month[i] = 28;
        if (i==3 || i==5 || i==8 || i==10)
            length_month[i] = 30;
    }
    int dayofyear = floor(days);
    
    // Find month and day of month
    if (((year-1900) % 4)==0)
        length_month[1] = 29;
    
    int i = 1;
    int temp=0;
    while ((dayofyear > temp + length_month[i-1]) && (i<12)){
        temp = temp + length_month[i-1];
        i=i+1;
    }
    double month = i;
    double day = dayofyear - temp;
    
    // Find hours minutes and seconds
    double temp2 = (days-dayofyear)*24.0;
    double hour = floor(temp2);
    temp2 = (temp2-hour)*60.0;
    double minute = floor(temp2);
    double sec = (temp2-minute)*60.0;
    
    // Output
    time[0] = month;
    time[1] = day;
    time[2] = hour;
    time[3] = minute;
    time[4] = sec;
    
}
