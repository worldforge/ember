/*
 *  sr_timer.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */

# include <stdio.h>
# include <sr/sr_timer.h>
#include <vhcl_log.h>

# ifdef SR_TARGET_WINDOWS // defined in sr.h
# include <windows.h>
# include <sys/timeb.h>
# else 
# include <sys/stat.h>
# include <sys/time.h>
# endif

//======================= SrUtcTimer =====================================

SrUtcTimer::SrUtcTimer ()
 { 
   _perf_freq = 0.0;
   _utc_origin = 0.0;

   # ifdef SR_TARGET_WINDOWS
   LARGE_INTEGER lpFrequency;
   LARGE_INTEGER lpPerformanceCount;
   struct _timeb tbuf;
   if ( QueryPerformanceFrequency(&lpFrequency)==false )
    LOG("SrTimer: WIN32 High Resolution Performance Counter not supported.\n");
   else
    { _perf_freq = (double)lpFrequency.QuadPart;
      QueryPerformanceCounter ( &lpPerformanceCount );
      _ftime( &tbuf ); // get UTC time in seconds from Jan 1, 1970
      double hrcTime = double(lpPerformanceCount.QuadPart) / double(lpFrequency.QuadPart);
      double utcTime = double(tbuf.time) + double(tbuf.millitm)*1.0E-3;
      _utc_origin = utcTime - hrcTime;
    }
   # endif
 }

double SrUtcTimer::time () // returns time in seconds
 {
   # ifdef SR_TARGET_WINDOWS
   if ( _perf_freq==0 ) // not available
    { _timeb tp; 
      _ftime(&tp);
      return 0.001*(double)tp.millitm + (double)tp.time;
    }
   else
    { LARGE_INTEGER lpPerformanceCount;
      QueryPerformanceCounter (&lpPerformanceCount);
      return _utc_origin + double(lpPerformanceCount.QuadPart) / _perf_freq;
    }
   # endif
   
   # ifdef SR_TARGET_LINUX
   timeval tp;
   //   timezone tz;
   if ( gettimeofday(&tp,0)==-1 ) return 0; 
   return 0.000001*(double)tp.tv_usec + (double)tp.tv_sec;
   # endif
   
   // the following worked with SGIs:
   //timeval tp; 
   //if ( gettimeofday(&tp)==-1 ) return 0;  // resolution: >100Hz
   //return 0.000001*(double)tp.tv_usec + (double)tp.tv_sec;
 }

//======================= SrTimer =====================================

SrTimer::SrTimer ( sruint interval )
 { 
   _interval = (double)interval; 
   _last_start = 0.0;
   _last_stop = 0.0;
   _measurement_number = 0.0;
   _last_mps_number = 0.0;
   _last_mps_time = 0.0;
   _mps = 0.0;

   _loopdt = 0.0;
   _loopdt_sum = 0.0;
   _meandt = 0.0;
   _meandt_sum = 0.0;
   _dt = 0.0;
 }

void SrTimer::stop ()
 {
   // The first thing to do is to take the actual time:
   double curtime = time();

   // Increment the measurement counter:
   _measurement_number++;

   // Save the dt of this measurement:
   _dt = curtime - _last_start;

   // Accumulate deltas used to get mean values:
   _meandt_sum += _dt;
   _loopdt_sum += curtime - _last_stop;

   // Check if it is time to update mean values:
   if ( _measurement_number-_last_mps_number == _interval )
    {
      // mps calculation :
      _mps = _interval / (curtime-_last_mps_time); // frames/secs
      _last_mps_time = curtime;
      _last_mps_number = _measurement_number;

      // Loop calculation :
      _loopdt = _loopdt_sum / _interval;
      _loopdt_sum = 0.0;

      // Mean measurement calculation :
      _meandt = _meandt_sum / _interval;
      _meandt_sum = 0.0;
    }

   _last_stop = curtime;
 }

//============================= end of file ==========================
