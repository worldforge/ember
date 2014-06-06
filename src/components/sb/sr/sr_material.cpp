/*  sr_material.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

# include <sr/sr_material.h>
#include <string>

SrMaterial::SrMaterial () :
            ambient  (  51,  51,  51, 255 ),
            diffuse  ( 204, 204, 204, 255 ),
            specular (   0,   0,   0, 255 ),
            emission (   0,   0,   0, 255 )			
 {
   shininess = 0;         
 }

void SrMaterial::init () 
 { 
   ambient.set  (  51,  51,  51, 255 );
   diffuse.set  ( 204, 204, 204, 255 );
   specular.set (   0,   0,   0, 255 );
   emission.set (   0,   0,   0, 255 );
   shininess = 0;       
 }

bool operator == ( const SrMaterial& m1, const SrMaterial& m2 )
 {
   return ( m1.ambient==m2.ambient &&
            m1.diffuse==m2.diffuse &&
            m1.specular==m2.specular &&
            m1.emission==m2.emission &&
            m1.shininess==m2.shininess)? true:false;
 }

bool operator != ( const SrMaterial& m1, const SrMaterial& m2 )
 {
   return ( m1.ambient==m2.ambient &&
            m1.diffuse==m2.diffuse &&
            m1.specular==m2.specular &&
            m1.emission==m2.emission &&
            m1.shininess==m2.shininess)? false:true;
 }

SrOutput& operator<< ( SrOutput& o, const SrMaterial& m )
 {
   return o << m.ambient  <<' '<< 
               m.diffuse  <<' '<< 
               m.specular <<' '<< 
               m.emission <<' '<< 
               m.shininess ;
 }

SrInput& operator>> ( SrInput& in, SrMaterial& m )
 {
   return in >> m.ambient  >>
                m.diffuse  >>
                m.specular >>
                m.emission >>
                m.shininess ;
 }

//================================ End of File =================================================
