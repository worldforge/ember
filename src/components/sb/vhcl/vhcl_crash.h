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

#ifndef VHCL_CRASH_H
#define VHCL_CRASH_H


// Requires linking with the following libs:
//   dbghelp.lib
//
// Requires the following .dlls to be alongside your .exe:
//   "$(VS90COMNTOOLS)\..\IDE\dbghelp.dll"    (one is included with windows, but this version is the "full" version)
//   "\lib\vhcl\blat\blat.dll"


#include <exception>


namespace vhcl
{
namespace Crash
{
   // TODO options!
   // vhcl::crash::ReportCrashes( true );
   // vhcl::crash::EmailNotification( true );
   // vhcl::crash::ShowDialogOnCrash()
   // vhcl::crash::SetDialogText();
   // vhcl::crash::SendFullDump()  very large!
   // redesign so that it doesn't always send a mail:
   //    offer crash options, output to LOG(), get a string of the callstack, etc.
   // need to solidify contract of who deletes callbacks

   // also take a look at Google BreakPad and MS WinQual for alternatives

   class CrashCallback
   {
      public:
         virtual void OnCrash() = 0;
   };


   void EnableExceptionHandling( const bool enable );

   void SetSourceEmail( const std::string & address );
   void SetDestinationEmail( const std::string & address );
   void SetEmailServer( const std::string & server );

   void AddCrashCallback( CrashCallback * callback );


   int HandleException( void * info );
   void SendExceptionReport( const std::exception & ex );
}
}


#endif  // VHCL_CRASH_H
