//
//  load_inputs.c
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
//%                       - t2doy.c
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <string.h>
#include <math.h>
#include "load_inputs.h"
#include <stdlib.h>
#include <errno.h>
#include "days2mdh.h"
#include "t2doy.h"

extern int errno ;

void load_inputs(int length_of_file[5], double time_parameters[13], double sc_parameters[33], double grav_coef[5148][6], double mag_coef[195][27], double model_parameters[27], double ap_index[length_of_file[0]], double solar_input[length_of_file[1]][3], double wmm_coef[90][18], double iar80[106][5], double rar80[106][4], double eop[length_of_file[2]][10], double max_dates[5][3], double sun_eph[length_of_file[3]][3], double moon_eph[length_of_file[4]][3], double albedo[12][20][40][2]){
    
    // Initialize parameters
    char skip[500];
    char skip2[500];
    int errnum;
    char temp;
    double temp2;
    
    // Input file location
    char loc[500] = "input/";
    char loc_data[500] = "data/";
    
    // Load time_parameters
    char loc_time_parameters[500];
    strcpy(loc_time_parameters, loc);
    FILE *fp = fopen(strcat(loc_time_parameters,"time_parameters.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_time_parameters, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 500, fp);
    for (int i = 4; i < 13; i++)
        fscanf(fp, "%lf", &time_parameters[i]);
    fclose(fp);
    double total_length = time_parameters[4]*24*60*60 + time_parameters[5]*60*60 + time_parameters[6]*60 + time_parameters[7];
    double output_length = time_parameters[8]*24*60*60 + time_parameters[9]*60*60 + time_parameters[10]*60 + time_parameters[11];
    if (total_length==0){
        fprintf(stderr, "Error in 'time_parameters.txt': Input propagation time is null\n");
        exit(-1);
    }
    if (output_length==0){
        fprintf(stderr, "Error in 'time_parameters.txt': Output time step is null\n");
        exit(-1);
    }
    if (time_parameters[12]==0){
        fprintf(stderr, "Error in 'time_parameters.txt': Propagation time step is null\n");
        exit(-1);
    }
    if (total_length<0){
        fprintf(stderr, "Error in 'time_parameters.txt': Input propagation time is negative\n");
        exit(-1);
    }
    if (output_length<0){
        fprintf(stderr, "Error in 'time_parameters.txt': Output time step is negative\n");
        exit(-1);
    }
    if (time_parameters[12]<0){
        fprintf(stderr, "Error in 'time_parameters.txt': Propagation time step is negative\n");
        exit(-1);
    }
    if (total_length>24*60*60*365.25*15){
        printf("Warning in 'time_parameters.txt': Input propagation time is larger than 15 years\n");
    }
    if (output_length>total_length){
        fprintf(stderr, "Error in 'time_parameters.txt': Output time step is larger than propagation length\n");
        exit(-1);
    }
        if (time_parameters[12]>output_length){
        fprintf(stderr, "Error in 'time_parameters.txt': Propagation time step is larger than propagation length\n");
        exit(-1);
    }
    if (time_parameters[12]>total_length){
        fprintf(stderr, "Error in 'time_parameters.txt': Propagation time step is larger than output time step\n");
        exit(-1);
    }
    
    // Load sc_parameters
    char loc_sc_parameters[500];
    strcpy(loc_sc_parameters, loc);
    fp = fopen(strcat(loc_sc_parameters,"sc_parameters.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_sc_parameters, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 6; i++)
        fgets(skip, 200, fp);
    for (int i = 0; i < 2; i++)
        fscanf(fp, "%lf", &sc_parameters[i]);
    for (int i = 8; i < 33; i++)
        fscanf(fp, "%lf", &sc_parameters[i]);
    fclose(fp);
    if (sc_parameters[0]==0){
        fprintf(stderr, "Error in 'sc_parameters.txt': Spacecraft mass is null\n");
        exit(-1);
    }
    if (sc_parameters[0]<0){
        fprintf(stderr, "Error in 'sc_parameters.txt': Spacecraft mass is negative\n");
        exit(-1);
    }
    if (sc_parameters[1]==0){
        fprintf(stderr, "Error in 'sc_parameters.txt': Number of surfaces is null\n");
        exit(-1);
    }
    if (sc_parameters[1]<0){
        fprintf(stderr, "Error in 'sc_parameters.txt': Number of surfaces is negative\n");
        exit(-1);
    }
    if ((sc_parameters[14]!=1)&&(sc_parameters[14]!=2)){
        fprintf(stderr, "Error in 'sc_parameters.txt': Initial orientation reference frame is undefined\n");
        exit(-1);
    }
    if ((sc_parameters[15]==0)||(sc_parameters[19]==0)||(sc_parameters[23]==0)){
        fprintf(stderr, "Error in 'sc_parameters.txt': Inertia tensor is invalid\n");
        exit(-1);
    }
    if ((sc_parameters[16]!=sc_parameters[18])||(sc_parameters[17]!=sc_parameters[21])||(sc_parameters[20]!=sc_parameters[22])){
        fprintf(stderr, "Error in 'sc_parameters.txt': Inertia tensor is not symmetric\n");
        exit(-1);
    }
    if ((sc_parameters[24]==0)||(sc_parameters[28]==0)||(sc_parameters[32]==0)){
        fprintf(stderr, "Error in 'sc_parameters.txt': Magnetic tensor is invalid\n");
        exit(-1);
    }
    if ((sc_parameters[25]!=sc_parameters[27])||(sc_parameters[26]!=sc_parameters[30])||(sc_parameters[29]!=sc_parameters[31])){
        fprintf(stderr, "Error in 'sc_parameters.txt': Magnetic tensor is not symmetric\n");
        exit(-1);
    }
    
    // Load Spherical Harmonics Coefficients
    char loc_grav_coef[500];
    strcpy(loc_grav_coef, loc_data);
    fp = fopen(strcat(loc_grav_coef,"EGM2008_TideFree_Coefficients.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_grav_coef, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 3; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < 5148; i++)
    {
        for (int j = 0; j < 6; j++)
            fscanf(fp, "%lf", &grav_coef[i][j]);
    }
    fclose(fp);
    
    // Load IGRF-12 Gauss Coefficients
    char loc_mag_coef[500];
    strcpy(loc_mag_coef, loc_data);
    fp = fopen(strcat(loc_mag_coef,"igrf12coeffs.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_mag_coef, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < 195; i++)
    {
        fscanf(fp,"%c", &temp);
        for (int j = 0; j < 27; j++){
            fscanf(fp, "%lf", &mag_coef[i][j]);
        }
        for (int j = 0; j < 27; j++){
            fscanf(fp,"%lf", &temp2);
        }
    }
    fclose(fp);
    
    // Load model parameters
    char loc_model_parameters[500];
    strcpy(loc_model_parameters, loc);
    fp = fopen(strcat(loc_model_parameters,"model_parameters.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_model_parameters, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 7; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < 27; i++)
        fscanf(fp, "%lf", &model_parameters[i]);
    fclose(fp);
    if ((model_parameters[0]!=0)&&(model_parameters[0]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Aerodynamic Drag is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[1]!=0)&&(model_parameters[1]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Aerodynamic Torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[2]!=0)&&(model_parameters[2]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Gravitational Model is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[3]!=0)&&(model_parameters[3]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Gravity-Gradient Torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[4]!=0)&&(model_parameters[4]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Eddy-Current Torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[5]!=0)&&(model_parameters[5]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Solar Gravitational Perturbations is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[6]!=0)&&(model_parameters[6]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Lunar Gravitational Perturbations is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[7]!=0)&&(model_parameters[7]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of SRP acceleration is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[8]!=0)&&(model_parameters[8]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Albedo acceleration is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[9]!=0)&&(model_parameters[9]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Infrared acceleration is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[10]!=0)&&(model_parameters[10]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of SRP torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[11]!=0)&&(model_parameters[11]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Albedo torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[12]!=0)&&(model_parameters[12]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Infrared torque is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[13]!=0)&&(model_parameters[13]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Inclusion of Kane damper is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[14]<0)||(model_parameters[14]>100)){
        fprintf(stderr, "Error in 'model_parameters.txt': Number of terms for Gravitational Model is invalid. Maximum is 100, minimum is 0\n");
        exit(-1);
    }
    if ((model_parameters[15]<0)||(model_parameters[15]>100)){
        fprintf(stderr, "Error in 'model_parameters.txt': Number of terms for Gravitational Model is invalid. Maximum is 100, minimum is 0\n");
        exit(-1);
    }
    if ((model_parameters[16]<1)||(model_parameters[16]>4)){
        fprintf(stderr, "Error in 'model_parameters.txt': Drag coefficient invalid. Possible drag coefficient range is 1-4\n");
        exit(-1);
    }
    if ((model_parameters[17]<0)||(model_parameters[17]>500)){
        fprintf(stderr, "Error in 'model_parameters.txt': Constant Ap value is invalid. Minimum is 0, maximum is 500\n");
        exit(-1);
    }
    if ((model_parameters[18]<0)||(model_parameters[18]>500)){
        fprintf(stderr, "Error in 'model_parameters.txt': Constant F10.7 value is invalid. Minimum is 0, maximum is 500\n");
        exit(-1);
    }
    if ((model_parameters[19]!=1)&&(model_parameters[19]!=2)){
        fprintf(stderr, "Error in 'model_parameters.txt': Which atmospheric model to use is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[20]!=1)&&(model_parameters[20]!=2)){
        fprintf(stderr, "Error in 'model_parameters.txt': Which geomagnetic model to use is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[21]!=0)&&(model_parameters[21]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Which wind model to use is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[22]!=1)&&(model_parameters[22]!=2)&&(model_parameters[22]!=3)){
        fprintf(stderr, "Error in 'model_parameters.txt': Which albedo model to use is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[23]<=0)&&(model_parameters[13]=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Kane damper moment of inertia value is invalid. It has to be greater than 0\n");
        exit(-1);
    }
    if ((model_parameters[24]<=0)&&(model_parameters[13]=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Damper coefficient value is invaled. It as to be greater than 0\n");
        exit(-1);
    }
    if ((model_parameters[25]!=0)&&(model_parameters[25]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Outputting Work file or not is ambiguous\n");
        exit(-1);
    }
    if ((model_parameters[26]!=0)&&(model_parameters[26]!=1)){
        fprintf(stderr, "Error in 'model_parameters.txt': Outputting Perturbations file or not is ambiguous\n");
        exit(-1);
    }
    
    // Load ap input for NRLMSISE-00
    char loc_ap_input[500];
    strcpy(loc_ap_input, loc_data);
    fp = fopen(strcat(loc_ap_input,"ap_input.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_ap_input, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < length_of_file[0]/8; i++){
        for (int j = 0; j < 3; j++)
            fscanf(fp, "%lf", &max_dates[0][j]);
        for (int j = 0; j < 8; j++)
            fscanf(fp, "%lf", &ap_index[i*8+j]);
    }
    fclose(fp);
    
    // Load F10.7 for NRLMSISE-00
    char loc_solar_input[500];
    strcpy(loc_solar_input, loc_data);
    fp = fopen(strcat(loc_solar_input,"solar_input.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_solar_input, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < length_of_file[1]; i++)
    {
        for (int j = 0; j < 3; j++)
            fscanf(fp, "%lf", &max_dates[1][j]);
        for (int j = 0; j < 3; j++){
            fscanf(fp, "%lf", &solar_input[i][j]);
        }
    }
    fclose(fp);
    
    // Load WMM Coefficients
    char loc_wmm_coef[500];
    strcpy(loc_wmm_coef, loc_data);
    fp = fopen(strcat(loc_wmm_coef,"wmm_coef.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_wmm_coef, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 3; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < 90; i++)
    {
        for (int j = 0; j < 18; j++)
            fscanf(fp, "%lf", &wmm_coef[i][j]);
    }
    fclose(fp);
    
    // Load 1980 IAU Thoery of Nutation Coefficients, Epoch J2000 (Vallado, 2013, p. 1043)
    char loc_nut[500];
    strcpy(loc_nut, loc_data);
    fp = fopen(strcat(loc_nut,"nut80.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_nut, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 106; i++)
    {
        for (int j = 0; j < 5; j++)
            fscanf(fp, "%lf", &iar80[i][j]);
        for (int j = 0; j < 4; j++)
            fscanf(fp, "%lf", &rar80[i][j]);
        fscanf(fp, "%lf", &temp2);
    }
    fclose(fp);
    // Convert rar80 to radians
    double convrt = 0.0001 *M_PI / (180.0*3600.0);
    for (int i=0; i<106; i++){
        for (int j=0; j<4; j++)
            rar80[i][j] = rar80[i][j] * convrt;
    }
    
    // Load Earth Orientation Parameters
    char loc_eop[500];
    strcpy(loc_eop, loc_data);
    fp = fopen(strcat(loc_eop,"eop.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_eop, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
        fgets(skip, 500, fp);
    for (int i = 0; i < length_of_file[2]; i++)
    {
        for (int j = 0; j < 10; j++){
            fscanf(fp, "%lf", &eop[i][j]);
        }
    }
    fclose(fp);
    
    // Load Sun Ephemeris
    char loc_sun[500];
    strcpy(loc_sun, loc_data);
    fp = fopen(strcat(loc_sun,"sun_ephemeris.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_sun, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip2, 500, fp);
    for (int i = 0; i < length_of_file[3]; i++)
    {
        for (int j = 0; j < 4; j++){
            fscanf(fp, "%lf", &temp2);
        }
        for (int j = 0; j < 3; j++){
            fscanf(fp, "%lf", &sun_eph[i][j]);
        }
    }
    fclose(fp);
    
    // Load Moon Ephemeris
    char loc_moon[500];
    strcpy(loc_moon, loc_data);
    fp = fopen(strcat(loc_moon,"moon_ephemeris.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_moon, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 4; i++)
        fgets(skip2, 500, fp);
    for (int i = 0; i < length_of_file[4]; i++)
    {
        for (int j = 0; j < 4; j++){
            fscanf(fp, "%lf", &temp2);
        }
        for (int j = 0; j < 3; j++){
            fscanf(fp, "%lf", &moon_eph[i][j]);
        }
    }
    fclose(fp);
    
    // Load Albedo model
    int albedo_model = model_parameters[22];
    
    // Load Stephens Albedo and Emissivity
    if (albedo_model == 1){
        char loc_steph[500];
        strcpy(loc_steph, loc_data);
        fp = fopen(strcat(loc_steph,"albedo_emissivity_stephens_9x9.txt"),"r");
        if (fp == NULL){
            errnum = errno;
            fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_steph, strerror( errnum ));
            exit(-1);
        }
        fgets(skip2, 500, fp);
        for (int i = 0; i < 12; i++){
            for (int ii=0; ii<2; ii++){
                for (int j = 0; j < 20; j++){
                    for (int k = 0; k < 40; k++){
                        fscanf(fp, "%lf", &albedo[i][j][k][ii]);
                    }
                }
                fgets(skip2, 500, fp);
                fgets(skip2, 500, fp);
            }
        }
        fclose(fp);
    }
    
    // Load CERES Albedo and Emissivity
    if (albedo_model == 2){
        char loc_ceres[500];
        strcpy(loc_ceres, loc_data);
        fp = fopen(strcat(loc_ceres,"albedo_emissivity_CERES_9x9.txt"),"r");
        if (fp == NULL){
            errnum = errno;
            fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_ceres, strerror( errnum ));
            exit(-1);
        }
        fgets(skip2, 500, fp);
        for (int i = 0; i < 12; i++){
            for (int ii=0; ii<2; ii++){
                for (int j = 0; j < 20; j++){
                    for (int k = 0; k < 40; k++){
                        fscanf(fp, "%lf", &albedo[i][j][k][ii]);
                    }
                }
                fgets(skip2, 500, fp);
                fgets(skip2, 500, fp);
            }
        }
        fclose(fp);
    }
    
    // Load ECMWF Albedo and Emissivity
    if (albedo_model == 3){
        char loc_ecmwf[500];
        strcpy(loc_ecmwf, loc_data);
        fp = fopen(strcat(loc_ecmwf,"albedo_emissivity_ECMWF_9x9.txt"),"r");
        if (fp == NULL){
            errnum = errno;
            fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_ecmwf, strerror( errnum ));
            exit(-1);
        }
        fgets(skip2, 500, fp);
        for (int i = 0; i < 12; i++){
            for (int ii=0; ii<2; ii++){
                for (int j = 0; j < 20; j++){
                    for (int k = 0; k < 40; k++){
                        fscanf(fp, "%lf", &albedo[i][j][k][ii]);
                    }
                }
                fgets(skip2, 500, fp);
                fgets(skip2, 500, fp);
            }
        }
        fclose(fp);
    }
    
    // Check maximum possible dates for propagation considering the range of input values loaded from files;
    max_dates[2][0] = eop[length_of_file[2]-1][0];
    max_dates[2][1] = eop[length_of_file[2]-1][1];
    max_dates[2][2] = eop[length_of_file[2]-1][2];
    for (int i=0; i<2; i++){
        max_dates[i][0] = max_dates[i][0]+2000;
        max_dates[i][1] = -max_dates[i][1];
        max_dates[i][2] = -max_dates[i][2];
    }
    int time_sun[3];
    double time_sun2[5];
    t2doy((length_of_file[3]/2.0-1)*24*60*60.0,time_sun);
    days2mdh(time_sun[0],time_sun[1],time_sun2);
    max_dates[3][0] = time_sun[0];
    max_dates[3][1] = time_sun2[0];
    max_dates[3][2] = time_sun2[1];
    int time_moon[3];
    double time_moon2[5];
    t2doy((length_of_file[4]/6.0-1)*24*60*60.0,time_moon);
    days2mdh(time_moon[0],time_moon[1],time_moon2);
    max_dates[4][0] = time_moon[0];
    max_dates[4][1] = time_moon2[0];
    max_dates[4][2] = time_moon2[1];
    
}
