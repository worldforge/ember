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
// FILE:        vhcl.h
//      This file contains system level utilities and definitions.
//      This file should be included at the top of every source file,
//      even before standard includes.  For example:
//
//         #include "vhcl.h"
//         #include "foo.h"
//
//         #include <windows.h>
//         #include <stdio.h>
//         #include <string>
//
//         int main()
//         {
//            printf( "Hello world\n" );
//            return 0;
//         }
//
//      - #include "vhcl.h" should go at the top of source files
//      - It should *not* go in header files
//      - Public headers have their own common header.  See vhcl_public.h.
//      - It can be used in or as a precompiled header.
//-----------------------------------------------------------------------------

#ifndef VHCL_H
#define VHCL_H


#include "vhcl_platform.h"
#include "vhcl_win32.h"
#include "vhcl_types.h"

// requires vhcl(_d).lib in order to use
#include "vhcl_memory.h"
#include "vhcl_assert.h"
#include "vhcl_string.h"
#include "vhcl_math.h"
#include "vhcl_timer.h"
#include "vhcl_crash.h"
#include "vhcl_log.h"





//////////////////////////////////////////////
//
// Future includes
//


// 64-bit considerations
//      *All* 3rd party libs must have 64-bit versions
//      64-bit programming for Game Developers  http://msdn.microsoft.com/en-us/library/bb147385.aspx


// UNICODE considerations
//      Should try to move all apps to unicode
//      3rd party issues.


// VHCL Math
//      3rd party?  (sony bullet?  http://bullet.svn.sourceforge.net/viewvc/bullet/trunk/Extras/)
//                   http://www.gamasutra.com/features/20000131/barad_03.htm
//                   http://forums.amd.com/forum/messageview.cfm?catid=77&threadid=84545
//                   http://www.intel.com/cd/ids/developer/asmo-na/eng/dc/games/293451.htm
//                   http://www.amd.com/us-en/assets/content_type/DownloadableAssets/dwamd_Dx9Software.pdf
//                   CML - http://cmldev.net/forums/viewtopic.php?t=7


// VHCL Threading (process priority, thread creation/deletion/priority, locking)
//    could be included in C++ 0x


// VHCL Bit Fiddling
//#define SET_FLAG( flag, field )     ( ( flag ) |= ( field ) )
//#define CLEAR_FLAG( flag, field )   ( ( flag ) &= ~( field ) )
//#define CHECK_FLAG( flag, field )   ( ( ( flag ) & ( field ) ) ? true : false )
//#define TOGGLE_FLAG( flag, field )  ( ( flag ) ^= ( field ) )

//#define SET_BIT( value, bit )       ( ( value ) |= ( 1 << ( bit ) ) )
//#define CLEAR_BIT( value, bit )     ( ( value ) &= ~( 1 << ( bit ) ) )
//#define CHECK_BIT( value, bit )     ( ( ( value ) & ( 1 << ( bit ) ) ) ? true : false )
//#define TOGGLE_BIT( value, bit )    ( ( value ) ^= ( 1 << ( bit ) ) )


#endif  // VHCL_H
