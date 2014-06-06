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

#ifndef VHCL_ASSERT_H
#define VHCL_ASSERT_H


#if defined(VHCL_ASSERTS_ENABLED)
   #define POW2_ASSERTS_ENABLED
#elif defined(VHCL_ASSERTS_DISABLED)
#else
   #if defined(DEBUG_BUILD)
      #define POW2_ASSERTS_ENABLED
   #elif defined(RELEASE_BUILD)
      //#define POW2_ASSERTS_ENABLED
   #endif
#endif


#include "Pow2Assert.h"


#undef ASSERT
#define ASSERT  POW2_ASSERT


#endif  // VHCL_ASSERT_H
