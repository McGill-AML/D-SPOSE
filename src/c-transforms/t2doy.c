//
//  t2doy.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-11-05.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        days2mdh.c
//%
//% DESCRIPTION:          This function converts t2000 to the year, day of
//%                       year, and seconds of day
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 November 5, 2016
//% VERSION:              1
//%
//% INPUT:                double t2000: seconds since January 1, 2000, 00:00:00
//%
//% OUTPUT:               int time[3]:
//%                         - time[0]: year
//%                         - time[1]: day of year
//%                         - time[2]: days since January 1, 2000
//%                       double sec: seconds of day
//%
//% COUPLING:             None
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "t2doy.h"
#include <math.h>

double t2doy(double t2000, int time[3]){
    
    double sec;
    
    time[0] = 2000;
    time[1] = 0;
    time[2] = 0;
    int daysinyear;
    double temp_sec = t2000;
    
    int go = 1;
    while (go) {
        
        if (((time[0]-2000) % 4)==0)
            daysinyear = 366;
        else
            daysinyear = 365;
        
        temp_sec = temp_sec - daysinyear*24*60*60.0;
        time[0] = time[0] + 1;
        time[2] = time[2] + daysinyear;
        
        if (temp_sec<0){
            go=0;
            temp_sec = temp_sec + daysinyear*24*60*60.0;
            time[0] = time[0] - 1;
            time[2] = time[2] - daysinyear;
        }
    }
    time[1] = (int) (floor(temp_sec/(24*60*60.0))+1.0);
    time[2] = time[2] + time[1];
    sec = temp_sec - 24*60*60*(time[1]-1);
    
    return sec;
    
}
