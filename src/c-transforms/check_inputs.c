//
//  check_inputs.c
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

#include <string.h>
#include <math.h>
#include "check_inputs.h"
#include <stdlib.h>
#include <errno.h>

extern int errno ;

void check_inputs(int length_of_file[5]){
    
    // Initialize parameters
    char skip[200];
    char skip2[400];
    double temp;
    int errnum;
    int n;
    int length;
    char ch;
    
    // File Location
    char loc_data[400] = "data/";
    
    // Check ap input for NRLMSISE-00
    char loc_ap_input[200];
    strcpy(loc_ap_input, loc_data);
    FILE *fp = fopen(strcat(loc_ap_input,"ap_input.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_ap_input, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 200, fp);
    n=0;
    while (fscanf(fp,"%lf",&temp) != EOF)
        n++;
    length = n/11*8;
    length_of_file[0] = length;
    fclose(fp);
    
    // Check F10.7 for NRLMSISE-00
    char loc_solar_input[200];
    strcpy(loc_solar_input, loc_data);
    fp = fopen(strcat(loc_solar_input,"solar_input.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_solar_input, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 200, fp);
    n=0;
    while (fscanf(fp,"%lf",&temp) != EOF)
        n++;
    length = n/6;
    length_of_file[1] = length;
    fclose(fp);
    
    // Check Earth Orientation Parameters
    char loc_eop[200];
    strcpy(loc_eop, loc_data);
    fp = fopen(strcat(loc_eop,"eop.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_eop, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
        fgets(skip, 200, fp);
    n=0;
    while (fscanf(fp,"%lf",&temp) != EOF)
        n++;
    length = n/10;
    length_of_file[2] = length;
    fclose(fp);
    
    // Check Sun Ephemeris
    char loc_sun[400];
    strcpy(loc_sun, loc_data);
    fp = fopen(strcat(loc_sun,"sun_ephemeris.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_sun, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip2, 400, fp);
    n=0;
    while (fscanf(fp,"%lf",&temp) != EOF)
        n++;
    length = n/7;
    length_of_file[3] = length;
    fclose(fp);
    
    // Check Moon Ephemeris
    char loc_moon[400];
    strcpy(loc_moon, loc_data);
    fp = fopen(strcat(loc_moon,"moon_ephemeris.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_moon, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip2, 400, fp);
    n=0;
    while (fscanf(fp,"%lf",&temp) != EOF)
        n++;
    length = n/7;
    length_of_file[4] = length;
    fclose(fp);
    
}
