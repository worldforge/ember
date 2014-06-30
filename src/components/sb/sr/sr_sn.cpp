/*  sr_sn.cpp - part of Motion Engine and SmartBody-lib
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

# include <sr/sr_sn.h>

//# define SR_USE_TRACE1  // SrSn Const/Dest
//# include <sr/sr_trace.h>

//======================================= SrSn ====================================

SrSn::SrSn ( SrSn::Type t, const char* class_name ) 
 { 
   //SR_TRACE1 ( "Constructor" );
   _ref = 0;
   _visible = 1;
   _type = t;
   _label = 0;
   _inst_class_name = class_name;
 }

SrSn::~SrSn ()
 {
   //SR_TRACE1 ( "Destructor" );
   delete _label;
 }

void SrSn::label ( const char* s )
 {
   sr_string_set ( _label, s );
 }

const char* SrSn::label () const
 {
   return _label? _label:"";
 }


//======================================= EOF ====================================

