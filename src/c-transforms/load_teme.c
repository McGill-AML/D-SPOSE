//
//  load_teme.c
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
//%                       double model_parameters[27]: model parameters
//%                       double max_dates[5][3]: maximum dates from input files
//%
//% OUTPUT:               double sc_parameters[33]:
//%                         - sc_parameters[2]: eccentricity
//%                         - sc_parameters[3]: semi-major axis (m)
//%                         - sc_parameters[4]: right ascension (deg)
//%                         - sc_parameters[5]: inclination (deg)
//%                         - sc_parameters[6]: argument of perigee (deg)
//%                         - sc_parameters[7]: true anomaly (deg)
//%                       double time_parameters[28]:
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

#include "load_teme.h"
#include "state2orbital.h"
#include "orbital2state.h"
#include <string.h>
#include <math.h>
#include "days2mdh.h"
#include <stdlib.h>
#include <errno.h>
#include "t2doy.h"

extern int errno ;

void load_teme(double eop[6479][10], double sc_parameters[33], double time_parameters[13], double model_parameters[27], double max_dates[5][3]){
    
    // Load R, V and t0 from input file;
    int errnum;
    double p_teme[3];
    double v_teme[3];
    double timev[2];
    FILE *fp = fopen("input/initial_conditions.txt","r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening TLE conversion file 'bin/initial_conditions.txt': %s\nMake sure to run SGP4 propagator to initialize orbit\n\n", strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 3; i++)
        fscanf(fp, "%lf", &p_teme[i]);
    for (int i = 0; i < 3; i++)
        fscanf(fp, "%lf", &v_teme[i]);
    for (int i = 0; i < 2; i++)
        fscanf(fp, "%lf", &timev[i]);
    fclose(fp);
    
    // Add start time info into time array
    int yr = timev[0];
    // If TLE year is before 2000 (above 1960, then output error message)
    if (yr>60){
        fprintf(stderr, "Error in 'TLE': Input TLE is before January 1, 2000\n");
        exit(-1);
    }
    double year = yr + 2000;
    double doy = timev[1];
    double no_leap = floor((yr-1)/4.0)+1;
    int days = 366*no_leap + 365*(yr-no_leap) + floor(doy)-1;
    double hours = (doy-floor(doy))*24;
    int hour = floor(hours);
    double minutes = (hours-hour)*60;
    int minute = floor(minutes);
    double dut1 = eop[days][6]; // TLE output is UT1, need UTC.
    double secs_utc = (minutes-minute)*60-dut1;
    double time[5];
    days2mdh(year, doy, time);
    // From UTC to TAI (leap seconds)
    double secs = secs_utc + 32;
    if (year>2005)
        secs = secs + 1;
    if (year>2008)
        secs = secs + 1;
    if (year==2012&&time[0]>6)
        secs = secs + 1;
    else if (year>2012)
        secs = secs + 1;
    if (year==2015&&time[0]>6)
        secs = secs + 1;
    else if (year>2015)
        secs = secs + 1;
    if (year>2016)
        secs = secs + 1;
    // TAI to TT
    secs = secs + 32.184;
    time_parameters[0] = days;
    time_parameters[1] = hour;
    time_parameters[2] = minute;
    time_parameters[3] = secs;
    double t2000utc = days*24*60*60 + hour*60*60 + minute*60 + secs_utc;
    
    // Add R and V info in parameter arrays (TEME reference frame)
    double p[3], v[3];
    for (int i=0; i<3; i++){
        p[i] = p_teme[i]*1000;
        v[i] = v_teme[i]*1000;
    }
    double coe[9];
    state2orbital(p,v,coe);
    orbital2state(p,v,coe);
    sc_parameters[2] = coe[1];
    sc_parameters[3] = coe[6];
    sc_parameters[4] = coe[2]*180.0/M_PI;
    sc_parameters[5] = coe[3]*180.0/M_PI;
    sc_parameters[6] = coe[4]*180.0/M_PI;
    sc_parameters[7] = coe[5]*180.0/M_PI;
    
    // Check Max Dates
    double total_length = time_parameters[4]*24*60*60 + time_parameters[5]*60*60 + time_parameters[6]*60 + time_parameters[7]+60*60*24; // Add one day to check just in case
    double t_finish = total_length+t2000utc;
    int time_finish[3];
    t2doy(t_finish, time_finish);
    double mdhms[5];
    days2mdh(time_finish[0], time_finish[1], mdhms);
    if (model_parameters[0]==1 || model_parameters[1]==1 ){
        if (model_parameters[19]==1){
            if ((model_parameters[17]==0) && ((time_finish[0]>max_dates[0][0]) || (time_finish[0]==max_dates[0][0]&&mdhms[0]>max_dates[0][1]) || (time_finish[0]==max_dates[0][0]&&mdhms[0]==max_dates[0][1]&&mdhms[1]>=max_dates[0][2]))){
                fprintf(stderr, "Error: Propagation period (%d-%d-%d) exceeds 'ap_input.txt' values (%d-%d-%d)\n", time_finish[0], (int)mdhms[0], (int)mdhms[1], (int)max_dates[0][0], (int)max_dates[0][1], (int)max_dates[0][2]);
                exit(-1);
            }
            if ((model_parameters[18]==0) && ((time_finish[0]>max_dates[1][0]) || (time_finish[0]==max_dates[1][0]&&mdhms[0]>max_dates[1][1]) || (time_finish[0]==max_dates[1][0]&&mdhms[0]==max_dates[1][1]&&mdhms[1]>=max_dates[1][2]))){
                fprintf(stderr, "Error: Propagation period (%d-%d-%d) exceeds 'solar_input.txt' values (%d-%d-%d)\n", time_finish[0], (int)mdhms[0], (int)mdhms[1], (int)max_dates[1][0], (int)max_dates[1][1], (int)max_dates[1][2]);
                exit(-1);
            }
        }
    }
    if (model_parameters[5]==1 || model_parameters[7]==1 || model_parameters[8]==1){
        if ((time_finish[0]>max_dates[3][0]) || (time_finish[0]==max_dates[3][0]&&mdhms[0]>max_dates[3][1]) || (time_finish[0]==max_dates[3][0]&&mdhms[0]==max_dates[3][1]&&mdhms[1]>=max_dates[3][2])){
            fprintf(stderr, "Error: Propagation period (%d-%d-%d) exceeds 'sun_ephemeris.txt' values (%d-%d-%d)\n", time_finish[0], (int)mdhms[0], (int)mdhms[1], (int)max_dates[3][0], (int)max_dates[3][1], (int)max_dates[3][2]);
            exit(-1);
        }
    }
    if (model_parameters[6]==1){
        if ((time_finish[0]>max_dates[4][0]) || (time_finish[0]==max_dates[4][0]&&mdhms[0]>max_dates[4][1]) || (time_finish[0]==max_dates[4][0]&&mdhms[0]==max_dates[4][1]&&mdhms[1]>=max_dates[4][2])){
            fprintf(stderr, "Error: Propagation period (%d-%d-%d) exceeds 'moon_ephemeris.txt' values (%d-%d-%d)\n", time_finish[0], (int)mdhms[0], (int)mdhms[1], (int)max_dates[4][0], (int)max_dates[4][1], (int)max_dates[4][2]);
            exit(-1);
        }
    }
    if ((year>max_dates[2][0]) || (year==max_dates[2][0]&&time[0]>max_dates[2][1]) || (year==max_dates[2][0]&&time[0]==max_dates[2][1]&&time[1]>=max_dates[2][2])){
          printf("Error: Propagation start (%d-%d-%d) exceeds 'eop.txt' values (%d-%d-%d)\n", (int)year, (int)time[0], (int)time[1], (int)max_dates[2][0], (int)max_dates[2][1], (int)max_dates[2][2]);
          exit(-1);
    }

    
}
