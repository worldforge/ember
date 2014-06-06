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

#ifndef VHCL_LOG_H
#define VHCL_LOG_H

#include "vhcl_public.h"

#include <stdarg.h>

#include <string>
#include <list>
#include <vector>


// issues
//    - using global var for ease-of-use
//    - using global namespace LOG() for ease-of-use
//    - need to add log levels
//    - need to add win32 window listener
//    - possibly replace w/ 3rd party logger:
//          Log4Cxx  - maybe static problems (dll), need to test
//          http://nocturnal.insomniacgames.com/index.php/Console  <- console colors
//    - need to solidify contract of who deletes listeners


void LOG( const char * message, ... );


namespace vhcl
{
namespace Log
{
   class Listener
   {
      public:
         virtual void OnMessage( const std::string & message ) = 0;
   };


   class FileListener : public Listener
   {
      private:
         std::string m_filename;

      public:
         FileListener( const char * filename );
         virtual ~FileListener();

         virtual void OnMessage( const std::string & message );
   };


   class DebuggerListener : public Listener
   {
      public:
         DebuggerListener();
         virtual ~DebuggerListener();

         virtual void OnMessage( const std::string & message );
   };


#ifdef ANDROID_BUILD
   class AndroidListener : public Listener
   {
      public:
         std::list<std::string> logList;

      public:
         AndroidListener();
         virtual ~AndroidListener();

         virtual void OnMessage( const std::string & message );
         std::string getLogs();
   };
#endif


   class StdoutListener : public Listener
   {
      public:
         StdoutListener();
         virtual ~StdoutListener();

         virtual void OnMessage( const std::string & message );
   };


   class Logger
   {
      private:
         std::vector< Listener * > m_listeners;

      public:
         Logger();
         virtual ~Logger();

         void AddListener( Listener * listener );
         void RemoveListener( Listener * listener );
         void RemoveAllListeners();

         bool IsEnabled() const;


         void Log( const char * message, ... );

         void vLog( const char * message, va_list argPtr );
   };


   // global instance of a logger
   extern Logger g_log;
};
};


#endif  // VHCL_LOG_H
