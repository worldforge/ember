/*
 *  sr_timer.h - part of Motion Engine and SmartBody-lib
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

#ifndef SR_TIMER_H
#define SR_TIMER_H

/** \file sr_timer.h 
 * Measure time and execution per second rates.*/

#include "components/sb/sb/SBTypes.h"
#include "sr.h"

/*! \class SrUtcTimer sr_timer.h
    \brief Get UTC time in seconds  */
class SBAPI SrUtcTimer
 { private :
    double _perf_freq;
    double _utc_origin;

   public :
    /*! Constructor checks if high performance counter can be used in windows */
    SrUtcTimer ();

    /*! Returns the time in seconds since midnight (00:00:00) of
        January 1, 1970, coordinated universal time (UTC). */
    double time ();
 };
 
/*! \class SrTimer sr_timer.h
    \brief Measure time and execution per second rates.

    SrTimer is intended to help counting the time taken by any function or
    portion of code. It can also be used to measure the frames per second 
    rate of an application. After instantiation, the user should call start()
    just before the function to measure the time, and stop() right after. Then,
    the method dt() will return the elapsed time between both function calls.
    Many other time measurements are possible. It is considered that each time
    the start/end pair is called, one measurement is done. In this way the user
    can perform several measurements (like in a loop) and then retrieve the 
    meandt(), calculated according to the specified interval(). Times between
    measurements can also be retrieved with loopdt(). */
class SBAPI SrTimer : public SrUtcTimer
 { private :
    double _interval;       // the sampling interval
    double _last_start;
    double _last_stop;
    double _measurement_number;
    double _last_mps_number;
    double _last_mps_time;
    double _mps;
    double _loopdt; 
    double _loopdt_sum;
    double _meandt;
    double _meandt_sum;
    double _dt;

   public : 
    /*! Constructs a counter giving the sampling interval to consider for
        calculating mean times. The default interval value is 15 measurements.
        In this case, each time the pair start()/stop() is called "interval" 
        times, a new mps() rate value is calculated. If interval is initialized
        with 0, mean times are not computed. */ 
    SrTimer ( sruint interval=15 );

    /*! Changes the sampling interval being used. */
    void interval ( sruint i ) { _interval=(double)i; }

    /*! Returns the sampling interval being used. */
    sruint interval () { return (sruint)_interval; }

    /*! To be called just before your time measurement starts.
        This method simply calls the base class method start(). */
    void start () { _last_start=SrUtcTimer::time(); }

    /*! To be called just after your time measurement ends.
        This method calculates the ellapsed time since the last start()
        call (retrievable with dt()), and updates the mean times if the
        number of measurements performed reaches the specified interval. */
    void stop ();

    /*! Returns the elapsed time since the last call to start(). */
    double t () { return time()-_last_start; }

    /*! Returns the time taken by the last start/stop measurement. */
    double dt () { return _dt; } 

    /*! Returns the number of measurements already performed. The returned
        double type contains an integer value. */
    double measurements () { return _measurement_number; }

    /*! Returns the mean elapsed time taken by the last interval measurements,
        according to the specified sampling interval. */
    double meandt () { return _meandt; } 

    /*! Returns the current measurements per second rate being achieved,
        measured according to the current sampling interval. Note: if the 
        sampling interval is zero, the mps is simply not computed and the
        last value is returned. */
    double mps () { return _mps; }

    /*! Returns the mean time taken between measurements, according to the
        current sampling interval. One "loop delta t" is considered to be
        the ellapsed time of two consecutive stop() calls. As such measurements
        makes only sense to perform inside loops, loopdt() returns always the
        mean value according to the sampling interval. */
    double loopdt () { return _loopdt; }

    /*! Returns the time of the last call to stop(). */
    double last_stop () { return _last_stop; } 

    /*! Returns the time of the last call to start(). */
    double last_start () { return _last_start; } 
 };

//============================= end of file ==========================

# endif // SR_TIMER_H
