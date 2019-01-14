//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% SCRIPT NAME:          TLE2RV
//%
//% DESCRIPTION:          This script will use SGP4 to obtain R and V from
//%                       the input TLE.
//%
//% AUTHOR:               Luc Sagnieres (Vallado)
//% DATE:                 October 23, 2017
//% VERSION:              1
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/* ---------------------------------------------------------------------
*
*                              testSGP4.cpp
*
*  this program tests the sgp4 propagator. an stk ephemeris file is generated
*  along with the test output. the code for this is left justified for easy
*  location.
*
*                          companion code for
*             fundamentals of astrodynamics and applications
*                                  2013
*                            by david vallado
*
*     (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
*     *****************************************************************
*    current :
*               7 dec 15  david vallado
*                           fix jd, jdfrac
*    changes :
*               3 nov 14  david vallado
*                           update to msvs2013 c++
*              11 nov 13  david vallado
*                           conversion to msvs c++
*                           misc fixes to constants and options
*                           add singly averaged state elements to be exported
*               3 sep 08  david vallado
*                           add switch for afspc compatibility and improved operation
*              14 may 08  david vallado
*                           fixes for linux suggested by brian micek
*                           misc fixes noted by the community - manual operation,
*                           formats, char lengths
*              14 aug 06  david vallado
*                           update mfe for verification time steps, constants
*              20 jul 05  david vallado
*                           fixes for paper, corrections from paul crawford
*               7 jul 04  david vallado
*                           fix record file and get working
*              14 may 01  david vallado
*                           2nd edition baseline
*                     80  norad
*                           original baseline
*       ----------------------------------------------------------------      */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#include "SGP4.h"

//using namespace SGP4Funcs;

extern int errno ;

int main()
{

    int errnum;
    FILE *inFile = fopen("input/TLE.txt", "r");
    if (inFile == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file 'input/TLE.txt': %s\n\n", strerror( errnum ));
        exit(-1);
    }
    FILE *outFile = fopen("input/initial_conditions.txt", "w");
    if (outFile == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file 'bin/initial_conditions.txt': %s\n\n", strerror( errnum ));
        exit(-1);
    }
    
    char longstr1[130];
    char longstr2[130];
    
    fgets(longstr1, 130, inFile);
    fgets(longstr1, 130, inFile);
    fgets(longstr1, 130, inFile);
    fgets(longstr2, 130, inFile);
    
	double ro[3];
	double vo[3];

    char typerun, typeinput, opsmode;
    gravconsttype  whichconst;
    
	// ----------------------------  locals  -------------------------------
	double p, a, ecc, incl, node, argp, nu, m, arglat, truelon, lonper;
	double sec, jd, jdFrac, rad, tsince, startmfe, stopmfe, deltamin;
    double tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2;
	int  year; int mon; int day; int hr; int min;
	typedef char str3[4];
	char outname[64];
	elsetrec satrec;
    
	// ------------------------  implementation   --------------------------

	//opsmode = 'a' best understanding of how afspc code works
    opsmode = a;

	//typerun = m manual operation- either mfe, epoch, or day of yr
	typerun = m;

	//typeinput = 'm' input start stop mfe
    typeinput = m;

	whichconst = wgs72;

	// sgp4fix no longer needed. done once in sgp4init
	SGP4Funcs::getgravconst( whichconst, tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2 );

	// ---------------- setup files for operation ------------------

    
    SGP4Funcs::twoline2rv( longstr1, longstr2, typerun, typeinput, opsmode, whichconst, startmfe, stopmfe, deltamin, satrec );
    
    SGP4Funcs::sgp4(satrec,  0.0, ro,  vo);
    
    fprintf(outFile,"%.8f\t%.8f\t%.8f\n%.9f\t%.9f\t%.9f\n",
			 ro[0], ro[1], ro[2], vo[0], vo[1], vo[2]);
    
    if (((ro[0]==0) && (ro[1]==0) && (ro[2]==0))|| ((vo[0]==0)&& (vo[1]==0)&& (vo[2]==0))){
        fprintf(stderr, "Error in 'input/initial_conditions.txt': Invalid initial conditions. Check TLE input file\n");
        exit(-1);
    }
    if (isnan(ro[0]) || isnan(ro[1]) ||isnan(ro[2]) ||isnan(vo[0]) ||isnan(vo[1]) ||isnan(vo[2]) ||isnan(satrec.epochyr)||isnan(satrec.epochdays)){
        fprintf(stderr, "Error in 'input/initial_conditions.txt': Invalid initial conditions. Check TLE input file\n");
        exit(-1);
    }
    
    fprintf(outFile,"%d\t%.8f\n",satrec.epochyr,satrec.epochdays);
    
    fclose(inFile);
    fclose(outFile);
    
	return 0;
}  // testSGP4

