/*
 *  behavior_scheduler_fixed.cpp - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 */

#include "vhcl.h"

#include <map>

#include "behavior_scheduler_fixed.hpp"

#include "bml.hpp"
#include "bml_exception.hpp"


using namespace std;
using namespace BML;


// local utility function


/////////////////////////////////////////////////////////////////////////////
//  BehaviorSchedulerFixed
//

BehaviorSchedulerFixed::BehaviorSchedulerFixed( const VecSyncPairs& input ) {
	// TODO: Replace exceptions in constructor with better solution

	VecSyncPairs::const_iterator it = input.begin();
	VecSyncPairs::const_iterator end = input.end();

	if( it != end ) {
		// At least one sync point
		time_sec     last_time = 0;
		unsigned int cur_index = 0;

		//// Make sure we deal with start first
		//if( it->first != BML::ATTR_START ) {
		//	sync_point_times.push_back( make_pair<wstring,float>( BML::ATTR_START, 0 ) );
		//	sync_id2index.insert( make_pair<wstring,unsigned int>( BML::ATTR_START, cur_index++ ) );
		//}

		MapNameIndex::iterator map_end = name_to_index.end();
		for( ; it != end; ++it ) {
			if( it->second < last_time ) {
				// TODO: include details like id and times.  Don't forget to transcode the wstring.
				throw BML::BmlException( "BehaviorSchedulerFixed: Invalid sync point timing." );
			}

			std::wstring sync_id = it->first;
			last_time = it->second;

			sync_point_times.push_back( std::make_pair( sync_id, last_time ) );
			name_to_index.insert( std::make_pair( sync_id, cur_index++ ) );
		}
	} else {
		throw BML::BmlException( "BehaviorSchedulerFixed: No sync points specified." );
	}
}

void BehaviorSchedulerFixed::validate_match( BehaviorSyncPoints& behav_syncs ) {
	BehaviorSyncPoints::iterator seq_it = behav_syncs.begin();
	BehaviorSyncPoints::iterator seq_end = behav_syncs.end();

	VecSyncPairs::iterator it = sync_point_times.begin();
	VecSyncPairs::iterator it_end = sync_point_times.begin();

	BehaviorSyncPoints::iterator last_sp;
	while( it!=it_end ) {
		if( seq_it==seq_end ) {
			throw BML::SchedulingException( "BehaviorSchedulerFixed: SyncPoint does not exist (sp_end reached before it_end)" );
		}

		if( seq_it->name() != it->first ) {
			throw BML::SchedulingException( "BehaviorSchedulerFixed: Unexpected SyncPoint (id mismatch or ordering issue)" );
		}

		++it;
		++seq_it;
	}

	if( seq_it != seq_end ) {
		throw BML::SchedulingException( "BehaviorSchedulerFixed: Unexpected SyncPoint (reached it_end before seq_end)" );
	}

	// Success! Valid ordering.
}

void BehaviorSchedulerFixed::schedule( BehaviorSyncPoints& behav_syncs, time_sec now ) {
	// validate the BehaviorSyncPoints match before manipulating them
	validate_match( behav_syncs );

	// Find first sync point that is set
	BehaviorSyncPoints::iterator sp_end = behav_syncs.end();
	BehaviorSyncPoints::iterator sp_first_set = behav_syncs.first_scheduled();
	if( sp_first_set == sp_end ) {
		// No SyncPoints previously scheduled
		// Schedule starting at time zero
		BehaviorSyncPoints::iterator sync_it = behav_syncs.begin();

		VecSyncPairs::iterator it = sync_point_times.begin();
		VecSyncPairs::iterator it_end = sync_point_times.begin();

		for( ; it!=it_end; ++it, ++sync_it ) {
			// Already verified name order in validate_match(..)
			sync_it->sync()->time = now + it->second;
		}
	} else {
		size_t first_set_index = name_to_index.find( sp_first_set->name() )->second;

		time_sec first_set_time = sp_first_set->time(); // Time set by parent sync points
		time_sec first_set_reltime = sync_point_times[ first_set_index ].second; // time relative to behavior start
		time_sec offset = first_set_time - first_set_reltime;

		// Verify there are no other SyncPoints with set times before assigning new times
		BehaviorSyncPoints::iterator sp_it = sp_first_set;
		size_t index = first_set_index+1;
		for( ++sp_it; sp_it!=sp_end; ++sp_it, ++index ) {
			if( sp_it->is_set() ) {
				// TODO: test if time set on this sync is within an expected threshold of the time as it would be scheduled
				// TODO construct string with more details
				throw BML::SchedulingException( "BehaviorSchedulerFixed: Found second SyncPoint previous set by other BehaviorSchedule." );
			}
		}

		// Assign the time values
		sp_it = behav_syncs.begin();
		index = 0;
		for( ; sp_it != sp_first_set; ++index, ++sp_it ) {
			sp_it->sync()->set_time( offset + sync_point_times[ index ].second );
		}
		++sp_it;
		++index;
		for( ; sp_it != sp_first_set; ++index, ++sp_it ) {
			sp_it->sync()->set_time( offset + sync_point_times[ index ].second );
		}
	}
}
