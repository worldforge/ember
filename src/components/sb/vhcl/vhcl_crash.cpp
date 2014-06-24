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

#include "vhcl.h"

#if defined(WIN_BUILD)

#include <string>

#include <windows.h>

#include "Exception.h"
#include "Mail.h"


using std::string;
using std::vector;


namespace vhcl
{
namespace Crash
{


vector< CrashCallback * > g_crashCallbacks;


static bool ShowCrashDialog()
{
         // A crash has been detected in your app.  The app is now closing.
         // Do you want to email a crash report to the dev team?

         // mail server:
         // email addresses (separated by semi-colon):


         // See crash details
         // Send Email
         // Don't send Email

   // creating message boxes is a lot easier than custom dialogs, plus they work in both console and win32 apps.  Unfortunately, they're not very user-friendly

   while ( 1 )
   {
      const std::string s = "A crash has been detected in your app.  The app is now closing.  Do you want to email a crash report to the dev team?\n\nAbort: Don't Email\nRetry: Email crash report\nIgnore: See Crash Report";
      int ret = MessageBox( NULL, s.c_str(), "Application Crash", MB_ABORTRETRYIGNORE | MB_SETFOREGROUND | MB_ICONSTOP );
      if ( ret == IDABORT )
      {
         return false;
      }
      else if ( ret == IDRETRY )
      {
         return true;
      }
      else if ( ret == IDIGNORE )
      {
      }
   }
}


void EnableExceptionHandling( const bool enable )
{
   SetUnhandledExceptionFilter( enable ? (PTOP_LEVEL_EXCEPTION_FILTER)&HandleException : NULL );
}


void SetSourceEmail( const string & address )
{
   nocturnal::Debug::SetDefaultSource( address );
}


void SetDestinationEmail( const string & address )
{
   nocturnal::Debug::SetDefaultDestination( address );
}


void SetEmailServer( const string & server )
{
   nocturnal::Debug::SetDefaultServer( server );
}


void AddCrashCallback( CrashCallback * callback )
{
   g_crashCallbacks.push_back( callback );
}


int HandleException( void * info )
{
   // crash has been caught, call callback
   for ( size_t i = 0; i < g_crashCallbacks.size(); i++ )
   {
      g_crashCallbacks[ i ]->OnCrash();
   }


   bool sendReport = ShowCrashDialog();
   if ( sendReport )
   {
      return nocturnal::Debug::HandleException( (LPEXCEPTION_POINTERS)info );
   }

   return EXCEPTION_CONTINUE_SEARCH;
}


void SendExceptionReport( const std::exception & ex )
{
   bool sendReport = ShowCrashDialog();
   if ( sendReport )
   {
      return nocturnal::Debug::ProcessException( ex );
   }
}


}
}


#else  // WIN_BUILD

// TODO: linux

#include <string>

using std::string;

namespace vhcl
{
namespace Crash
{

void EnableExceptionHandling( const bool enable ) {}
void SetSourceEmail( const string & address ) {}
void SetDestinationEmail( const string & address ) {}
void SetEmailServer( const string & server ) {}
void AddCrashCallback( CrashCallback * callback ) {}
int HandleException( void * info ) { return 0;}
void SendExceptionReport( const std::exception & ex ) {}

}
}

#endif
