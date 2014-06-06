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

#ifndef VHCL_TIMER_H
#define VHCL_TIMER_H


#include "vhcl_public.h"


namespace vhcl
{

   void Sleep( const double sleepSeconds );

   void PosixClockGetTime( long * tv_sec, long * tv_nsec );


   class Timer
   {
      private:
         int64_t m_ticksPerSecond;
         int64_t m_initialCounter;

      public:
         Timer();
         virtual ~Timer();

         double GetTime();
         double GetTimeMS() { return GetTime() * 1000.0; }
   };


   class StopWatch
   {
      private:
         Timer & m_timer;
         bool m_started;
         double m_startTime;
         double m_endTime;

      private:
         StopWatch & operator= ( const StopWatch & );

      public:
         explicit StopWatch( Timer & timer )
            : m_timer( timer )
         {
            m_started = false;
            m_startTime = 0;
            m_endTime = 0;
         }

         void Start()
         {
            m_startTime = m_timer.GetTime();
            m_started = true;
         }

         void Stop()
         {
            m_endTime = m_timer.GetTime();
            m_started = false;
         }

         bool IsStarted() const { return m_started; }

         double GetTime() { return m_endTime - m_startTime; }
         double GetTimeMS() { return GetTime() * 1000.0; }
   };

};


#endif  // VHCL_TIMER_H
