/*
 *  sr_sn_matrix.cpp - part of Motion Engine and SmartBody-lib
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

# include <sr/sr_sn_matrix.h>

//# define SR_USE_TRACE1  // Const/Dest
//# include <sr/sr_trace.h>

//======================================= SrSnMatrix ====================================

const char* SrSnMatrix::class_name = "Matrix";

SrSnMatrix::SrSnMatrix ()
           :SrSn ( SrSn::TypeMatrix, SrSnMatrix::class_name )
 {
   //SR_TRACE1 ( "Constructor" );
 }

SrSnMatrix::SrSnMatrix ( const SrMat& m )
           :SrSn ( SrSn::TypeMatrix, SrSnMatrix::class_name )
 {
   //SR_TRACE1 ( "Constructor from SrMat" );
   _mat = m;
 }

SrSnMatrix::~SrSnMatrix ()
 {
   //SR_TRACE1 ( "Destructor" );
 }


//======================================= EOF ====================================

