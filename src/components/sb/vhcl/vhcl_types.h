/*
   This file is part of VHMsg written by Edward Fast at 
   University of Southern California's Institute for Creative Technologies.
   http://www.ict.usc.edu
   Copyright 2008 Edward Fast, University of Southern California

   VHMsg is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   VHMsg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VHMsg.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VHCL_TYPES_H
#define VHCL_TYPES_H


#if defined(WIN_BUILD) && _MSC_VER < 1600
// These headers are C99 headers, but they are not included in Visual Studio yet.
// These were obtained from http://code.google.com/p/msinttypes/
// and will be used until Microsoft decides to include them. (VS2010 perhaps?)
#include "msinttypes/stdint.h"
#else
#include <stdint.h>
#endif


#endif  // VHCL_TYPES_H
