//
//  sc_geometry.h
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

#ifndef sc_geometry_h
#define sc_geometry_h

#include <stdio.h>
#include "surface.h"

double sc_geometry(int n_surf, struct surface geometry[n_surf]);

#endif /* sc_geometry_h */
