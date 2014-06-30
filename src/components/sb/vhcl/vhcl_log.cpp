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


#include <vhcl/vhcl.h>

#include <string>

#if defined(WIN_BUILD)
#include <windows.h>
#endif

#include <stdio.h>
#include <stdarg.h>


#define VHCL_LOG_FUNCTION_ENABLED  1


using namespace vhcl::Log;


#if !defined(ANDROID_BUILD)


// global instance of a logger
Logger vhcl::Log::g_log;


void LOG( const char * message, ... )
{
#if VHCL_LOG_FUNCTION_ENABLED
   va_list argList;
   va_start( argList, message );
   vhcl::Log::g_log.vLog( message, argList );
   va_end( argList );
#endif
}



FileListener::FileListener( const char * filename )
{
   m_filename = filename;

   // wipe the file before starting
   FILE * file = fopen( m_filename.c_str(), "w" );
   if ( file )
   {
      fclose( file );
   }
};


FileListener::~FileListener()
{
}


void FileListener::OnMessage( const std::string & message )
{
   // we open the file, write the message, then close the file.
   // this makes sure the file is properly flushed and released each time a message is received.
   // otherwise, the last output may not be received before a crash.
   // also, if it crashes in a wierd way or debugging is stopped manually, the file may still be locked after exit
   // also, viewing the file while it's open may not show the full contents of the file (due to delayed writing, etc)
   // also, the file is locked during a run, which may be an issue if you want to copy/move the file while the program is running.

   FILE * file = fopen( m_filename.c_str(), "a" );
   if ( file )
   {
      fprintf( file, "%s", message.c_str() );
      fclose( file );
   }
}


DebuggerListener::DebuggerListener()
{
}


DebuggerListener::~DebuggerListener()
{
}


void DebuggerListener::OnMessage( const std::string & message )
{
#if defined(WIN_BUILD)
   OutputDebugString( message.c_str() );
#endif
}


StdoutListener::StdoutListener()
{
}


StdoutListener::~StdoutListener()
{
}


void StdoutListener::OnMessage( const std::string & message )
{
   printf( "%s", message.c_str() );
}




Logger::Logger()
{
}


Logger::~Logger()
{
   RemoveAllListeners();
}


void Logger::AddListener( Listener * listener )
{
   m_listeners.push_back( listener );
}


void Logger::RemoveListener( Listener * listener )
{
   std::vector<Listener *>::iterator iter = m_listeners.end();
   for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
   {
      if ((*iter) == listener)
         break;
   }

   if (iter != m_listeners.end())
      m_listeners.erase(iter);
}


void Logger::RemoveAllListeners()
{
   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      // don't call delete until we more formally describe the ownership policy
      //delete m_listeners[ i ];
   }
   m_listeners.clear();
}


bool Logger::IsEnabled() const
{
   if ( m_listeners.empty() )
      return false;

   return true;
}


void Logger::Log( const char * message, ... )
{
   if ( !IsEnabled() )
      return;

   va_list argList;
   va_start( argList, message );
   std::string result = vhcl::vFormat( message, argList );
   va_end( argList );

   std::string s = result;
   s.append( "\n" );

   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      m_listeners[ i ]->OnMessage( s );
   }
}


void Logger::vLog( const char * message, va_list argPtr )
{
   if ( !IsEnabled() )
      return;

   std::string result = vhcl::vFormat( message, argPtr );

   std::string s = result;
   s.append( "\n" );

   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      m_listeners[ i ]->OnMessage( s );
   }
}


#else  // ANDROID_BUILD


#include <android/log.h>
#include "vhcl_log.h"


Logger vhcl::Log::g_log;

void LOG( const char * message, ... )
{
#if VHCL_LOG_FUNCTION_ENABLED
   va_list argList;
   va_start( argList, message );    
   vhcl::Log::g_log.vLog( message, argList );
   std::string result = vhcl::vFormat( message, argList ); 
   __android_log_vprint(ANDROID_LOG_INFO,"SBM",message,argList); 
   va_end( argList );   
#endif
}


FileListener::FileListener( const char * filename )
{
   
}


FileListener::~FileListener()
{
}


void FileListener::OnMessage( const std::string & message )
{
   // we're now letting log4cxx handle the message
}



AndroidListener::AndroidListener( )
{
   
}


AndroidListener::~AndroidListener()
{
}


void AndroidListener::OnMessage( const std::string & message )
{
   // we're now letting log4cxx handle the message
   while (logList.size() > 100)
      logList.pop_front();
   logList.push_back(message);
}

std::string AndroidListener::getLogs()
{
    std::string strLog;
    std::list<std::string>::iterator li;
    for ( li  = logList.begin();
          li != logList.end();
          li++)
    {
        strLog += (*li);
        strLog += "\n";
    }
    return strLog;
}


DebuggerListener::DebuggerListener()
{

}


DebuggerListener::~DebuggerListener()
{
}


void DebuggerListener::OnMessage( const std::string & message )
{
   // we're now letting log4cxx handle the message
}


StdoutListener::StdoutListener()
{
   
}


StdoutListener::~StdoutListener()
{
}


void StdoutListener::OnMessage( const std::string & message )
{
   // we're now letting log4cxx handle the message
}


Logger::Logger()
{
   
}


Logger::~Logger()
{
   RemoveAllListeners();
}


void Logger::AddListener( Listener * listener )
{
   m_listeners.push_back( listener );
}


void Logger::RemoveListener( Listener * listener )
{
   std::vector<Listener *>::iterator iter = m_listeners.end();
   for (iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
   {
      if ((*iter) == listener)
         break;
   }

   if (iter != m_listeners.end())
      m_listeners.erase(iter);
}


void Logger::RemoveAllListeners()
{
   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      // don't call delete until we more formally describe the ownership policy
      //delete m_listeners[ i ];
   }
   m_listeners.clear();
}


bool Logger::IsEnabled() const
{
   if ( m_listeners.empty() )
      return false;

   return true;
}


void Logger::Log( const char * message, ... )
{
   if ( !IsEnabled() )
      return;

   va_list argList;
   va_start( argList, message );
   std::string result = vhcl::vFormat( message, argList );
   va_end( argList );

   std::string s = result;
   s.append( "\n" );

   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      m_listeners[ i ]->OnMessage( s );
   }
}


void Logger::vLog( const char * message, va_list argPtr )
{
   if ( !IsEnabled() )
      return;

   std::string result = vhcl::vFormat( message, argPtr );

   std::string s = result;
   s.append( "\n" );

   for ( size_t i = 0; i < m_listeners.size(); i++ )
   {
      m_listeners[ i ]->OnMessage( s );
   }
}

#endif  // ANDROID_BUILD
