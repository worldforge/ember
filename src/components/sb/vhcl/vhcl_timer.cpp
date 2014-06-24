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
#include <windows.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

#if defined(WIN_BUILD) || defined(MAC_BUILD) || defined(IPHONE_BUILD)
#include <sys/timeb.h>
#endif

#if defined(LINUX_BUILD)
#include <unistd.h>
#endif


using namespace vhcl;


void vhcl::Sleep( const double sleepSeconds )
{
#if defined(WIN_BUILD)
   ::Sleep( (uint32_t)( sleepSeconds * 1000 + 0.5 ) );
#else
   usleep( (int)( sleepSeconds * 1000 * 1000 + 0.5 ) );
#endif
}


void vhcl::PosixClockGetTime( long * tv_sec, long * tv_nsec )
{
   // Plaform independent version of clock_gettime()

   // Win32 doesn't support clock_gettime() which would be the normal way of handling this.  Use ftime() instead and convert it

#if defined(WIN_BUILD)
   // get the current time
   __timeb32 tb;
   _ftime32( &tb );

   *tv_sec = tb.time;
   *tv_nsec = tb.millitm * 1000000;  // millisecond to nanosecond
#elif defined(MAC_BUILD) || defined(IPHONE_BUILD)
   timeb tb;
   ftime( &tb );

   *tv_sec = tb.time;
   *tv_nsec = tb.millitm * 1000000;  // millisecond to nanosecond
#else
   timespec ts;
   clock_gettime( CLOCK_MONOTONIC, &ts );
   *tv_sec = ts.tv_sec;
   *tv_nsec = ts.tv_nsec;
#endif
}


/*
   Game Timing and Multicore Processors
   http://msdn.microsoft.com/en-us/library/bb173458(VS.85).aspx


   See the above article for considerations for this module:
      - locking to a single thread to avoid bugs jumping between cores/cpus
      - clamp values to prevent bugs going back in time, or large jumps forward
*/


Timer::Timer()
{
#if defined(WIN_BUILD)
   QueryPerformanceFrequency( (LARGE_INTEGER *)&m_ticksPerSecond );
   QueryPerformanceCounter( (LARGE_INTEGER *)&m_initialCounter );
#else
   struct timeval tv;
   gettimeofday( &tv, NULL );
   m_initialCounter = tv.tv_sec + ( tv.tv_usec / 1000000.0 );  // truncates decimal, but for starting point, not important
#endif
}


Timer::~Timer()
{
}


double Timer::GetTime()
{
#if defined(WIN_BUILD)
   int64_t currentCounter;
   QueryPerformanceCounter( (LARGE_INTEGER *)&currentCounter );

   return (double)( currentCounter - m_initialCounter ) / (double)m_ticksPerSecond;
#else
   struct timeval tv;
   gettimeofday( &tv, NULL );
   return ( tv.tv_sec + ( tv.tv_usec / 1000000.0 ) ) - m_initialCounter;
#endif
}
