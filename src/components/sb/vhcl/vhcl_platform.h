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

#ifndef VHCL_PLATFORM_H
#define VHCL_PLATFORM_H


/*
    This header does platform and build type detection.
    It currently identifies the following platforms and sets the following defines:

    WIN_BUILD         - defined for any windows-based build.  Win32, Win64, Xbox, Console, DLL
    WIN32_BUILD       - defined for Win32 builds
    WIN64_BUILD       - defined for Win64 builds
    XBOX_BUILD        - defined for Xbox builds
    WIN_CONSOLE_BUILD - defined for windows console apps
    WIN_DLL_BUILD     - defined for dll libs

    IPHONE_BUILD - defined for any iOS build
    MAC_BUILD    - defined for any OSX build

    ANDROID_BUILD - defined for any Android build

    FLASH_BUILD - defined for any Flash build (using FlashCC)

    LINUX_BUILD - defined for any linux build (other than above)

    UNKNOWN_BUILD - defined as an error for any other platform

    DEBUG_BUILD - defined for debug builds
    RELEASE_BUILD - defined for release builds


    Usage can be any of:
        #if DEBUG_BUILD
        #ifdef DEBUG_BUILD
        #if defined(DEBUG_BUILD)
*/


// Platform identification.
#if defined(_WIN32)

   // Predefined Macros - http://msdn.microsoft.com/en-us/library/b0084kay(v=VS.90).aspx

   // defined for all Windows variants
   #define WIN_BUILD 1

   #if defined(_XBOX)
      #define XBOX_BUILD 1
   #else
      #if defined(_WIN64)
         #define WIN64_BUILD 1
      #else
         #define WIN32_BUILD 1
      #endif

      #if defined(_CONSOLE)
         #define WIN_CONSOLE_BUILD 1
      #elif defined(_USRDLL) // | defined(_DLL)
         #define WIN_DLL_BUILD 1
      #elif defined( NIAPP )
         #define WIN_NI_BUILD 1
      #endif
   #endif
#elif defined(__APPLE__)
   #include "TargetConditionals.h"
   #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR  // TODO:  need to separate these into different defines.  http://stackoverflow.com/questions/458304/how-can-i-programmatically-determine-if-my-app-is-running-in-the-iphone-simulato
      #define IPHONE_BUILD 1
   #else
      #define MAC_BUILD 1
   #endif
#elif defined(__ANDROID__)
   #define ANDROID_BUILD 1
#elif defined(__FLASHPLAYER__)
   #define FLASH_BUILD 1
#elif defined(linux) || defined(__linux)
   #define LINUX_BUILD 1
#else
   #define UNKNOWN_BUILD 1
#endif

#if defined(UNKNOWN_BUILD)
   #error Platform type not detected!
#endif



// Build type
#if defined(_DEBUG)
   #define DEBUG_BUILD 1
#elif defined(NDEBUG)
   #define RELEASE_BUILD 1
#else
   #if defined(LINUX_BUILD) || defined(MAC_BUILD) || defined(IPHONE_BUILD) || defined(ANDROID_BUILD) || defined(FLASH_BUILD)
      #define RELEASE_BUILD 1
   #else
      #define UNKNOWN_BUILD 1
   #endif
#endif

#if defined(UNKNOWN_BUILD)
   #error Build type not detected!
#endif


#endif  // VHCL_PLATFORM_H
