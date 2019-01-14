//
//  surface.h
//  D-SPOSE
//
//  Created by Luc Sagnieres on 2016-02-17.
//  Copyright © 2018 Luc Sagnieres. All rights reserved.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//% FUNCTION NAME:        surface.h
//%
//% DESCRIPTION:          This structure contains information about each
//%                       spacecraft geometry surface
//%
//% AUTHOR:               Luc Sagnieres
//% DATE:                 February 17, 2016
//% VERSION:              1
//%
//% PROPERTIES:           double surface.area: area
//%                       double surface.normal[3]: unit inward normal
//%                       double surface.vertices[3][3]: position of 3
//%                         vertices in body frame
//%                       double surface.projected_area: area projected onto
//%                         other surface
//%                       double surface.crs: specular reflection
//%                         coefficient in optical range
//%                       double surface.crd: diffuse reflection coefficient
//%                         in optical range
//%                       double surface.ca: absorption coefficient in
//%                         optical range
//%                       double surface.crs_ir: specular reflection
//%                         coefficient in infrared range
//%                       double surface.crd_ir: diffuse reflection
//%                         coefficient in infrared range
//%                       double surface.ca_ir: absorption coefficient in
//%                         infrared range
//%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef surface_h
#define surface_h

struct surface
{
    double area;
    double normal[3];
    double vertices[3][3];
    
    double projected_area;
    
    double crs;
    double crd;
    double ca;
    
    double crs_ir;
    double crd_ir;
    double ca_ir;
};

#endif /* surface_h */
