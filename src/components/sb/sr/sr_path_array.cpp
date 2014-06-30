/*  sr_path_array.cpp - part of Motion Engine and SmartBody-lib
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

# include <sr/sr_path_array.h>

//# define SR_USE_TRACE1
//# include <sr/sr_trace.h>

//====================== SrPathArray ==========================

void SrPathArray::basedir_from_filename ( const char* file )
 {
   _basedir = file;
   _basedir.remove_file_name();
   _basedir.make_valid_path();
 }

bool SrPathArray::adjust_path ( SrString& file )
 {
   SrInput in;
   if ( open(in,file) )
    { file = in.filename();
      return true;
    }
   else
    { return false;
    }
 }

//=========================== EOF ===============================
