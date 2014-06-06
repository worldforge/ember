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

#ifndef VHCL_WIN32_H
#define VHCL_WIN32_H


#include "vhcl_platform.h"


// make sure debugging switch is turned on before any headers are included
// "Memory Leak Detection Enabling" http://msdn.microsoft.com/en-us/library/e5ewb1h3(v=VS.90).aspx
#if defined(WIN_BUILD)
   #if defined(DEBUG_BUILD)
      #define _CRTDBG_MAP_ALLOC
   #endif
#endif


// windows.h specific section
#if defined(WIN_BUILD)

   // turn off windows.h features
   #if !defined( WIN32_LEAN_AND_MEAN )
      #define WIN32_LEAN_AND_MEAN
   #endif

   #if !defined( STRICT )
      #define STRICT
   #endif


   /*
      Minimum System Required Macros to Define. See site for details (as of 9/07):
      Using the Windows Headers
      http://msdn2.microsoft.com/en-us/library/Aa383745.aspx

      //  (more info)
      // What's the difference between WINVER, _WIN32_WINNT, _WIN32_WINDOWS, and _WIN32_IE?
      // http://blogs.msdn.com/oldnewthing/archive/2007/04/11/2079137.aspx

         Windows Server 2008        NTDDI_VERSION >= NTDDI_LONGHORN
         Windows Vista              NTDDI_VERSION >= NTDDI_VISTA
         Windows Server 2003 SP1    NTDDI_VERSION >= NTDDI_WS03SP1
         Windows Server 2003        NTDDI_VERSION >= NTDDI_WS03
         Windows XP SP2             NTDDI_VERSION >= NTDDI_WINXPSP2
         Windows XP SP1             NTDDI_VERSION >= NTDDI_WINXPSP1
         Windows XP                 NTDDI_VERSION >= NTDDI_WINXP
         Windows 2000 SP4           NTDDI_VERSION >= NTDDI_WIN2KSP4
         Windows 2000 SP3           NTDDI_VERSION >= NTDDI_WIN2KSP3
         Windows 2000 SP2           NTDDI_VERSION >= NTDDI_WIN2KSP2
         Windows 2000 SP1           NTDDI_VERSION >= NTDDI_WIN2KSP1
         Windows 2000               NTDDI_VERSION >= NTDDI_WIN2K


         (legacy defines, when the latest Microsofk SDK isn't installed):

         Windows Server 2008                 _WIN32_WINNT>=0x0600   and  WINVER>=0x0600
         Windows Vista                       _WIN32_WINNT>=0x0600   and  WINVER>=0x0600
         Windows Server 2003                 _WIN32_WINNT>=0x0502   and  WINVER>=0x0502
         Windows XP                          _WIN32_WINNT>=0x0501   and  WINVER>=0x0501
         Windows 2000                        _WIN32_WINNT>=0x0500   and  WINVER>=0x0500
         Windows NT 4.0                      _WIN32_WINNT>=0x0400   and  WINVER>=0x0400
         Windows Me                          _WIN32_WINDOWS=0x0500  and  WINVER>=0x0500
         Windows 98                          _WIN32_WINDOWS>=0x0410 and  WINVER>=0x0410
         Windows 95                          _WIN32_WINDOWS>=0x0400 and  WINVER=0x0400

         Internet Explorer 7.0               _WIN32_IE>=0x0700
         Internet Explorer 6.0 SP2           _WIN32_IE>=0x0603
         Internet Explorer 6.0 SP1           _WIN32_IE>=0x0601
         Internet Explorer 6.0               _WIN32_IE>=0x0600
         Internet Explorer 5.01, 5.5         _WIN32_IE>=0x0501
         Internet Explorer 5.0, 5.0a, 5.0b   _WIN32_IE>=0x0500
         Internet Explorer 4.01              _WIN32_IE>=0x0401
         Internet Explorer 4.0               _WIN32_IE>=0x0400
         Internet Explorer 3.0, 3.01, 3.02   _WIN32_IE>=0x0300
   */
   #if defined( NTDDI_WIN2K )
      #define NTDDI_VERSION   NTDDI_WIN2K
   #else
      #define _WIN32_WINNT    0x0501
      #define WINVER          0x0501
      #define _WIN32_WINDOWS  0x0500
   #endif

   // described in windows.h (my comments say which file they are in)
   // Currently based off of windows.h used in Visual Studio.NET 2003
   //#define NOGDICAPMASKS       // used in <wingdi.h>   - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
   //#define NOVIRTUALKEYCODES   // used in <winuser.h>  - VK_*
   //#define NOWINMESSAGES       // used in <winuser.h>  - WM_*, EM_*, LB_*, CB_*
   //#define NOWINSTYLES         // used in <winuser.h>  - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
   //#define NOSYSMETRICS        // used in <winuser.h>  - SM_*
   //#define NOMENUS             // used in <winuser.h>  - MF_*
   //#define NOICONS             // used in <winuser.h>  - IDI_*
   //#define NOKEYSTATES         // used in <winuser.h>  - MK_*
   //#define NOSYSCOMMANDS       // used in <winuser.h>  - SC_*
   //#define NORASTEROPS         // used in <wingdi.h>   - Binary and Tertiary raster ops
   //#define NOSHOWWINDOW        // used in <winuser.h>  - SW_*
   //#define OEMRESOURCE         // used in <winuser.h>  - OEM Resource values
   //#define NOATOM              //                      - Atom Manager routines
   //#define NOCLIPBOARD         // used in <winuser.h>  - Clipboard routines
   //#define NOCOLOR             // used in <winuser.h>  - Screen colors
   //#define NOCTLMGR            // used in <winuser.h>  - Control and Dialog routines
   //#define NODRAWTEXT          // used in <winuser.h>  - DrawText() and DT_*
   //#define NOGDI               // all of <wingdi.h> used in <winuser.h>  - All GDI defines and routines
   //#define NOKERNEL            //                      - All KERNEL defines and routines
   //#define NOUSER              // all of <commctrl.h> most of <winuser.h> - All USER defines and routines
   //#define NONLS               // all of <winnls.h>    - All NLS defines and routines
   //#define NOMB                // used in <winuser.h>  - MB_* and MessageBox()
   //#define NOMEMMGR            //                      - GMEM_*, LMEM_*, GHND, LHND, associated routines
   #define NOMETAFILE            // used in <wingdi.h>   - typedef METAFILEPICT
   #define NOMINMAX              // used in <windef.h>   - Macros min(a,b) and max(a,b)
   //#define NOMSG               // used in <winuser.h>  - typedef MSG and associated routines
   //#define NOOPENFILE          //                      - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
   //#define NOSCROLL            // used in <winuser.h>  - SB_* and scrolling routines
   #define NOSERVICE             // all of  <winsvc.h>   - All Service Controller routines, SERVICE_ equates, etc.
   #define NOSOUND               //                      - Sound driver routines
   //#define NOTEXTMETRIC        // used in <wingdi.h>   - typedef TEXTMETRIC and associated routines
   //#define NOWH                // used in <winuser.h>  - SetWindowsHook and WH_*
   //#define NOWINOFFSETS        // used in <winuser.h>  - GWL_*, GCL_*, associated routines
   //#define NOCOMM              //                      - COMM driver routines
   //#define NOKANJI             //                      - Kanji support stuff.
   //#define NOHELP              // used in <winuser.h>  - Help engine interface.
   //#define NOPROFILER          //                      - Profiler interface.
   //#define NODEFERWINDOWPOS    // used in <winuser.h>  - DeferWindowPos routines
   #define NOMCX                 // all of  <mcx.h>      - Modem Configuration Extensions

   // doesn't have a windows.h comment, but used
   #define NOIME                 // all of  <imm.h>
   //#define NOCRYPT             // all of  <wincrypt.h> <winefs.h> <winscard.h> - crypto API (ignored if WIN32_LEAN_AND_MEAN is set)

   // This can be used to actually *decrease* compile time if Precompiled Headers are turned on.
   //#define <windows.h>
#endif



// disable certain compiler specific warnings
#if defined(WIN_BUILD)
   #if defined(WIN32_BUILD)
   // level 1
   #pragma warning(disable:4267)  // (64-bit portibility issue) conversion from 'size_t' to 'const uint32', possible loss of data
   #pragma warning(disable:4311)  // (64-bit portibility issue) pointer truncation from 'void *' to 'uint32'
   #pragma warning(disable:4312)  // (64-bit portibility issue) conversion from 'DWORD' to 'LPCVOID' of greater size
   //#pragma warning(disable:4786)  // identifier was truncated to '255' characters in the debug information
   #endif

   // level 3
   //#pragma warning(disable:4511)  // copy constructor could not be generated
   //#pragma warning(disable:4512)  // assignment operator could not be generated

   // level 4
   #pragma warning(disable:4100)  // unreferenced formal parameter
   #pragma warning(disable:4127)  // conditional expression is constant
   //#pragma warning(disable:4189)  // local variable is initialized but not referenced
   #pragma warning(disable:4201)  // nonstandard extension used : nameless struct/union
   #pragma warning(disable:4324)  // structure was padded due to __declspec(align())
   #pragma warning(disable:4505)  // unreferenced local function has been removed
   //#pragma warning(disable:4663)  // C++ language change: to explicitly specialize class template use the following syntax
   #pragma warning(disable:4702)  // unreachable code
   #pragma warning(disable:4714)  // function marked as __forceinline not inlined
#endif


#endif  // VHCL_WIN32_H
