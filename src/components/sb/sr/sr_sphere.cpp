/*
 *  sr_sphere.cpp - part of Motion Engine and SmartBody-lib
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
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

# include <sr/sr_sphere.h>
# include <sr/sr_box.h>

//================================== SrSphere ====================================

const char* SrSphere::class_name = "Sphere";

SrSphere::SrSphere () : center(SrPnt::null)
 {
   radius = 1.0f;
 }

SrSphere::SrSphere ( const SrSphere& s ) : center(s.center)
 {
   radius = s.radius;
 }

void SrSphere::get_bounding_box ( SrBox& box ) const
 { 
   SrVec r ( radius, radius, radius );
   box.set ( center-r, center+r );
 }

SrOutput& operator<< ( SrOutput& o, const SrSphere& sph )
 {
   return o << sph.center << ' ' << sph.radius;
 }

SrInput& operator>> ( SrInput& in, SrSphere& sph )
 {
   return in >> sph.center >> sph.radius;
 }

//================================ EOF =================================================
