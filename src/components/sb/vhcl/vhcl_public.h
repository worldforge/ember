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

//-----------------------------------------------------------------------------
// FILE:        vhcl_public.h
//      This file contains system level utilities and definitions.
//      This file should be included at the top of header files that will be used
//      publicly.  For example, in library headers.
//
//      Please avoid including other vhcl headers in public header files because
//      there may be unintended side-effects.  For example, vhcl_win32.h ignores
//      certain compiler warnings, or vhcl_assert.h re-defines ASSERT().  This
//      may not be acceptable in other projects that wish to use these libraries.
//-----------------------------------------------------------------------------

#ifndef VHCL_PUBLIC_H
#define VHCL_PUBLIC_H


#include "vhcl_platform.h"
#include "vhcl_types.h"
#include "vhcl_math.h"


#endif  // VHCL_PUBLIC_H
