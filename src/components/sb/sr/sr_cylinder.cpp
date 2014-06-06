/*
 *  sr_cylinder.cpp - part of Motion Engine and SmartBody-lib
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

#include <sr/sr_cylinder.h>
#include <sr/sr_box.h>

//================================== SrCylinder ====================================

const char* SrCylinder::class_name = "Cylinder";

SrCylinder::SrCylinder () : a(SrVec::null), b(SrVec::i)
 {
   radius = 0.1f;
 }

SrCylinder::SrCylinder ( const SrCylinder& c ) : a(c.a), b(c.b)
 {
   radius = c.radius;
 }

void SrCylinder::get_bounding_box ( SrBox& box ) const
 { 
   SrVec va = b-a; 
   va.normalize();
   SrVec vr1;
   if ( angle(SrVec::i,va)<0.1f )
     vr1 = cross ( SrVec::j, va );
   else
     vr1 = cross ( SrVec::i, va );
   
   SrVec vr2 = cross ( vr1, va );

   vr1.len ( radius );
   vr2.len ( radius );

   box.set_empty();
   box.extend ( a+vr1 );
   box.extend ( a-vr1 );
   box.extend ( a+vr2 );
   box.extend ( a-vr2 );
   box.extend ( b+vr1 );
   box.extend ( b-vr1 );
   box.extend ( b+vr2 );
   box.extend ( b-vr2 );
 }

SrOutput& operator<< ( SrOutput& o, const SrCylinder& c )
 {
   return o << c.a << srspc << c.b << srspc << c.radius;
 }

SrInput& operator>> ( SrInput& in, SrCylinder& c )
 {
   return in >> c.a >> c.b >> c.radius;
 }

//================================ EOF =================================================
