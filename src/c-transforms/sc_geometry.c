//
//  sc_geometry.c
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-17.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        sc_geometry.c
//%
//% DESCRIPTION:          This function loads the spacecraft geometry from
//%                       input file
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 17, 2016
//% VERSION:              1
//%
//% INPUT:                int n_surf: number of surfaces
//%
//% OUTPUT:               struct surface geometry[n_surf]: surface geometry model
//%                         - double geometry[i].normal[3]: unit inward
//%                           normal of surface i
//%                         - double geometry[i].vertices[3][3]: position of
//%                           3 vertices of surface i in body frame
//%                         - double geometry[i].area: area of surface i
//%                         - double geometry[i].crs: specular reflection
//%                           coefficient of surface i in optical range
//%                         - double geometry[i].crd: diffuse reflection
//%                           coefficient of surface i in optical range
//%                         - double geometry[i].ca: absorption
//%                           coefficient of surface i in optical range
//%                         - double geometry[i].crs_ir: specular reflection
//%                           coefficient of surface i in infrared range
//%                         - double geometry[i].crd_ir: diffuse reflection
//%                           coefficient of surface i in infrared range
//%                         - double geometry[i].ca_ir: absorption
//%                           coefficient of surface i in infrared range
//%
//% COUPLING:             - crossprod.c
//%                       - norm.c
//%                       - surface.h
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <math.h>
#include <string.h>
#include "sc_geometry.h"
#include "surface.h"
#include "crossprod.h"
#include <stdlib.h>
#include <errno.h>
#include "norm.h"

double sc_geometry(int n_surf, struct surface geometry[n_surf]){
    
    // Initialize parameters
    char skip[200];
    int errnum;
    char loc[200] = "input/";
    double spacecraft_geometry[n_surf][18];
    
    // Load geometry file
    char loc_geometry[200];
    strcpy(loc_geometry, loc);
    FILE *fp = fopen(strcat(loc_geometry,"sc_geometry.txt"),"r");
    if (fp == NULL){
        errnum = errno;
        fprintf(stderr, "\nError opening file '%s': %s\n\n", loc_geometry, strerror( errnum ));
        exit(-1);
    }
    for (int i = 0; i < 5; i++)
        fgets(skip, 200, fp);
    for (int i = 0; i < n_surf; i++)
    {
        for (int j = 0; j < 18; j++)
            fscanf(fp, "%lf", &spacecraft_geometry[i][j]);
    }
    fclose(fp);
    
    // Normal vectors pointing inward in body frame
    for (int i=0; i<n_surf; i++){
        for (int j=0; j<3; j++){
            geometry[i].normal[j] = spacecraft_geometry[i][j];
        }
        if ((norm(geometry[i].normal)>1.000000000000001)||(norm(geometry[i].normal)<0.99999999999999)){
            printf("%.16f\n",norm(geometry[i].normal));
            fprintf(stderr, "Warning in 'sc_geometry.txt': Surface normal #%d does not have unit size\n",i);
        }
        if ((norm(geometry[i].normal)>1.000001)||(norm(geometry[i].normal)<0.999999)){
            printf("%.16f\n",norm(geometry[i].normal));
            fprintf(stderr, "Error in 'sc_geometry.txt': Surface normal #%d does not have unit size\n",i);
            exit(-1);
        }
    }
    
    // Vertices
    double vertices[n_surf][3][3];
    for (int i=0; i<n_surf; i++){
        vertices[i][0][0] = spacecraft_geometry[i][3];
        vertices[i][0][1] = spacecraft_geometry[i][4];
        vertices[i][0][2] = spacecraft_geometry[i][5];
        vertices[i][1][0] = spacecraft_geometry[i][6];
        vertices[i][1][1] = spacecraft_geometry[i][7];
        vertices[i][1][2] = spacecraft_geometry[i][8];
        vertices[i][2][0] = spacecraft_geometry[i][9];
        vertices[i][2][1] = spacecraft_geometry[i][10];
        vertices[i][2][2] = spacecraft_geometry[i][11];
    }
    // Switch vertices to correct position (transpose of vertices matrix)
    for (int i=0; i<n_surf; i++){
        for (int j=0; j<3; j++){
            for (int k=0; k<3; k++){
                geometry[i].vertices[j][k] = vertices[i][k][j];
            }
        }
    }
    
    // Optical properties
    for (int i=0; i<n_surf; i++){
        geometry[i].crd = spacecraft_geometry[i][12];
        geometry[i].crs = spacecraft_geometry[i][13];
        geometry[i].ca = spacecraft_geometry[i][14];
        
        geometry[i].crd_ir = spacecraft_geometry[i][15];
        geometry[i].crs_ir = spacecraft_geometry[i][16];
        geometry[i].ca_ir = spacecraft_geometry[i][17];
    }
    
    // Surface areas
    for (int i=0; i<n_surf; i++){
        
        double temp1[3], temp2[3], temp3[3];
        for (int j=0; j<3; j++){
            temp1[j] = vertices[i][1][j]-vertices[i][0][j];
            temp2[j] = vertices[i][2][j]-vertices[i][0][j];
            crossprod(temp1,temp2,temp3);
        }
        geometry[i].area = sqrt(temp3[0]*temp3[0]+temp3[1]*temp3[1]+temp3[2]*temp3[2])/2.0;
    }
    double total_surface = 0;
    for (int i=0; i<n_surf; i++)
        total_surface = total_surface + geometry[i].area;
    
    // Return total surface area
    return total_surface;
}
