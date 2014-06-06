/*
 *  time_regulator.h - part of SmartBody-lib
 *  Copyright (C) 2010  University of Southern California
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
 *      Marcus Thiebaux, USC
 */

#ifndef TIME_REGULATOR_H
#define TIME_REGULATOR_H

/*
      TimeRegulator class:
        
        - Generates a controllable time stream from an internal or optional external source.
        - Regulates both simulated time, and real simulation loop performance.
        - May facilitate decentralized control of time for distributed simulation.
        
        Core rate params:
        
        - If set to 0.0, function is disabled.
        
        sim_dt:
                - fixed increment applied to output time
                - if 0.0, uses 'real' measured time
        eval_dt:
                - minimum interval at which time is incremented, independent of application loop update rate
                - if 0.0, increments each update call
        sleep_dt:
                - target loop rate, calls sleep to delay update call
                        
        Input update param:
        
        - If negative, uses internal clock.
        - If non-negative, param is interpreted as a clock, with increasing sequential values moving the time forward correspondingly.
        - If non-increasing, no increment occurs.
        - Switching dynamically from internal to external will not interrupt output stream.
        
        Return bool value:
        
        - Indicates that update has occured according to specified rate settings.
        - Indicates that valid output dt value is available.
        
        Special cases:
        
        - If speed param is set, sim_dt is set to 0.0, and 'real' time is scaled by the speed factor.
        - If ( sleep_dt == 0.0 ) && ( eval_dt == 0.0 ) && ( sim_dt > 0.0 ), update always returns true, using sim_dt increment.
        - Sleep-lock feature copies sleep_dt to sim_dt.
        
        Pausing and stepping:
        
        - Pause function will halt clock increments and update returns false.
        - Step [n] function will pause and step through the next n evaluations.
        - Resume restores normal behavior.
        
        Deterministic control:
        
        - The start() function is used internally during a source switch, but can be used to set exact initial clock conditions prior to subsequent update().
        - The reset() function will allow raw output time to be reset to a non-negative value, to ensure bitwise exact replication of fixed time for regression testing.
        
        Instrumentation:
        
        - The set_perf() function will output basic performance info at the specified interval.
        - The print() function outputs immediate internal state info.
        - The print_update() function will call update and print the results for testing.
        - The set_verbose() function will report internal events such as source switch.
        
        Supplementary functions:
        
        double	SBM_get_real_time( void );
        void	SBM_sleep_msec( int msec );
        double	SBM_sleep_wait( double prev_time, double target_dt, bool verbose = false );
*/

class TimeRegulator	{

	public:
	
		TimeRegulator( void ) { defaults(); }
		~TimeRegulator( void ) {}

		bool isStarted() { return started; }
		bool isRunning() { return !paused; }

		void set_verbose( bool v = true ) { verbose = v; }
		void set_perf( double interval )	{
			perf_interval = interval;
		}
		void set_speed( double s ) {
			speed = s; 
			sim_dt = 0.0;
		}
		
		void set_sleep_dt( double dt )	{ sleep_dt = dt; }
		void set_eval_dt( double dt )	{ eval_dt = dt; }
		void set_sim_dt( double dt )	{ sim_dt = dt; }

		void set_sleep_fps( double fps ) { 
			if( fps > 0.0 ) set_sleep_dt( 1.0 / fps );
			else set_sleep_dt( 0.0 );
		}
		void set_eval_fps( double fps ) { 
			if( fps > 0.0 ) set_eval_dt( 1.0 / fps );
			else set_eval_dt( 0.0 );
		}
		void set_sim_fps( double fps ) { 
			if( fps > 0.0 ) set_sim_dt( 1.0 / fps );
			else set_sim_dt( 0.0 );
		}
		void set_sleep_lock( bool enable = true )	{
			if( enable ) set_sim_dt( sleep_dt );
			else set_sim_dt( 0.0 );
		}
		
		void start( double in_time = -1.0 );
		void stop( void );
		void reset( double in_time = -1.0 );
		bool update( double in_time = -1.0 );

		void pause( void )				{ do_pause = true; }
		void step( int num_steps = 1 )	{ do_steps = num_steps; }
		void resume( void )				{ do_resume = true; }
		
		double get_time( void ) 	{ return( out_time ); }
		double get_dt( void )		{ return( out_dt ); }
		double get_sim_dt( void )	{ return( sim_dt ); }

		void print( void );
		void print_update( int id, double in_time = -1.0 );
		
	private:
		void defaults( void )	{
			
			verbose = false;
			started = false;
			extern_src = false;
			
			perf_interval = 0.0;
			perf_real_sum = 0.0;
			perf_sim_sum = 0.0;
		 	perf_count = 0;

			speed = 1.0;
			sleep_dt = 0.0;
			eval_dt = 0.0;
			sim_dt = 0.0;
		
			start_time = -1.0;
			clock_time = -1.0;
			real_time = 0.0;
			real_dt = 0.0;
			prev_real_time = 0.0;
			prev_loop_time = 0.0;
			eval_wait = 0.0;
			
			do_pause = false;
			do_resume = false;
			do_steps = 0;
			paused = false;

			out_time = 0.0;
			out_dt = 0.0;
		}
		void 	update_perf( void );

		bool	verbose;
		bool	started;
		bool	extern_src;
		
		double	perf_interval;
		double	perf_real_sum;
		double	perf_sim_sum;
		int 	perf_count;
		
		double	speed;
		double	sleep_dt;
		double	eval_dt;
		double	sim_dt;
		
		double	start_time;
		double	clock_time;
		double	real_time;
		double	real_dt;
		double	prev_real_time;
		double	prev_loop_time;
		double	eval_wait;
		
		bool	do_pause;
		bool	do_resume;
		int 	do_steps;
		bool	paused;

		double	out_time;
		double	out_dt;
};

double SBM_get_real_time( void );
void SBM_sleep_msec( int msec );
double SBM_sleep_wait( double prev_time, double target_dt, bool verbose = false );

#endif
