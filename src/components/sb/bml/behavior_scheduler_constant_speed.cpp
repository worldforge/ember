/*
 *  behavior_scheduler_constant_speed.cpp - part of SmartBody-lib
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

#include "bml.hpp"
#include "behavior_scheduler_constant_speed.hpp"

using namespace std;
using namespace BML;


const bool LOG_ABNORMAL_SPEED = false;



/////////////////////////////////////////////////////////////////////////////
//  BehaviorSchedulerLinear
//


BehaviorSchedulerConstantSpeed::BehaviorSchedulerConstantSpeed(
    // preferred "local times" of sync points
    time_sec startTime,
	time_sec readyTime,
	time_sec strokeStartTime,
    time_sec strokeTime,
	time_sec strokeEndTime,
    time_sec relaxTime,
	time_sec endTime,
    time_sec speed
) :
    // preferred "local times" of sync points
    startTime( startTime ),
	readyTime( readyTime ),
    strokeStartTime( strokeStartTime ),
    strokeTime( strokeTime ),
    strokeEndTime( strokeEndTime ),
    relaxTime( relaxTime ),
	endTime( endTime ),
	speed( speed )
{	
}



/** Tests ordering.  If invlaid, prints warning about ignoring SyncPoint before. */
bool testSyncBefore(
		SyncPointPtr& before, const char* before_name,
		SyncPointPtr& after, const char* after_name
) {
	if( before->time <= after->time ) {
		return true;
	} else {
        clog << "WARNING: BehaviorRequest::testSyncOrder(): "<<before_name<<" NOT before "<<after_name<<"... ignoring "<<before_name<<"." << endl;
		return false;
	}
}

/** Tests ordering.  If invlaid, prints warning about ignoring SyncPoint after. */
bool testSyncAfter(
		SyncPointPtr& before, const char* before_name,
		SyncPointPtr& after, const char* after_name
) {
	if( before->time <= after->time ) {
		return true;
	} else {
        clog << "WARNING: BehaviorRequest::testSyncOrder(): "<<before_name<<" NOT before "<<after_name<<"... ignoring "<<after_name<<"." << endl;
		return false;
	}
}



void BehaviorSchedulerConstantSpeed::schedule( BehaviorSyncPoints& behav_syncs, time_sec now ) {
	// local references to standard sync points
	SyncPointPtr start        = behav_syncs.sync_start()->sync();
	SyncPointPtr ready        = behav_syncs.sync_ready()->sync();
	SyncPointPtr stroke_start = behav_syncs.sync_stroke_start()->sync();
	SyncPointPtr stroke       = behav_syncs.sync_stroke()->sync();
	SyncPointPtr stroke_end   = behav_syncs.sync_stroke_end()->sync();
	SyncPointPtr relax        = behav_syncs.sync_relax()->sync();
	SyncPointPtr end          = behav_syncs.sync_end()->sync();

	/*  The following implements a search for the two most important SyncPoints, and then scales the time to meet both.
     *  Importance is ranked in this order: stroke, ready, relax, start, end
     *  If only one SyncPoint is found, the controller maintains its natural duration.
     *  If no sync points are found, the behavior starts immediately.
     */
    bool hasStart  = isTimeSet( start->time );
    bool hasReady  = isTimeSet( ready->time );
	bool hasStrokeStart = isTimeSet( stroke_start->time );
    bool hasStroke = isTimeSet( stroke->time );
	bool hasStrokeEnd = isTimeSet( stroke_end->time );
    bool hasRelax  = isTimeSet( relax->time );
    bool hasEnd    = isTimeSet( end->time );

	bool syncPointsSet[7] = { hasStart, hasReady, hasStrokeStart, hasStroke, hasStrokeEnd, hasRelax, hasEnd };
	bool rawSyncPointsSet[7] = { false, false, false, false, false, false, false };
	if (startTime >= 0.0)
		rawSyncPointsSet[0] = true;
	if (readyTime >= 0.0)
		rawSyncPointsSet[1] = true;
	if (strokeStartTime >= 0.0)
		rawSyncPointsSet[2] = true;
	if (strokeTime >= 0.0)
		rawSyncPointsSet[3] = true;
	if (strokeEndTime >= 0.0)
		rawSyncPointsSet[4] = true;
	if (relaxTime >= 0.0)
		rawSyncPointsSet[5] = true;
	if (endTime >= 0.0)
		rawSyncPointsSet[6] = true;
	
	
	double rawTimes[7] = {	startTime,
							readyTime,
							strokeStartTime,
							strokeTime,
							strokeEndTime,
							relaxTime,
							endTime
						 };

	double syncTimes[7] = {	start->time,
							ready->time,
							stroke_start->time,
							stroke->time,
							stroke_end->time,
							relax->time,
							end->time
						  };

	
	// set up desired -> controller time segments
	double controllerIntervals[6] = {	readyTime - startTime,
										strokeStartTime - readyTime,
										strokeTime - strokeStartTime,
										strokeEndTime - strokeTime,
										relaxTime - strokeEndTime,
										endTime - relaxTime
									   };

	double offsetTime = 0;

	time_sec start_at = TIME_UNSET;

	// check for invalid sync points desired when no sync point exists
	// i.e. when a stroke time is set, but motion has no stroke time at all
	// in this case, ignore the set sync point
	double lastGoodTime = 0.0;
	for (int x = 0; x < 7; x++)
	{
		if (syncPointsSet[x] && !rawSyncPointsSet[x])
		{
			// warning
			syncPointsSet[x] = false;
		}
		if (!rawSyncPointsSet[x])
		{ 
			// make sure that any missing sync point gets the value 
			// of the last sync point that had a proper time
			rawTimes[x] = lastGoodTime;
			rawSyncPointsSet[x] = true;
		}
		else
		{
			// make sure that later sync points don't come before earlier ones
			if (rawTimes[x] < lastGoodTime)
				rawTimes[x] = lastGoodTime ;
			else
				lastGoodTime = rawTimes[x];
		}
	}

	int numSyncPointsSet = 0;
	for (int x = 0; x < 7; x++)
		if (syncPointsSet[x])
			numSyncPointsSet++;

	if (numSyncPointsSet == 0)
	{
		start_at = now;
		for (int x = 0; x < 7; x++)
		{
			syncTimes[x] = start_at + rawTimes[x];
		}
	}
	else if (numSyncPointsSet == 1)
	{
		start_at = now;

		// which point is sync'ed
		int syncIndex = 0;
		for (int x = 0; x < 7; x++)
		{
			if (syncPointsSet[x])
			{
				syncIndex = x;
				break;
			}
		}
		// loop over first set of sync points
		double offset = syncTimes[syncIndex] - (rawTimes[syncIndex] + start_at);
		for (int x = 0; x < 7; x++)
		{
			if (x != syncIndex)
				syncTimes[x] = rawTimes[x] + start_at + offset;
		}
	}
	else
	{
		// determine the intervals
		int lastPoint = -1;
		std::vector<std::pair<int, int> > intervals;
		for (int x = 0; x < 7; x++)
		{
			if (syncPointsSet[x])
			{
				if (lastPoint == -1)
				{
					lastPoint = x;
				}
				else
				{
					intervals.push_back(std::pair<int, int>(lastPoint, x));
					lastPoint = x;
				}
			}
		}

		// now determine the scaling within those intervals
		int lastScaleIndex = 0;
		double lastScale = 1.0;
		std::vector<double> intervalScale; // intervalScale[0] = ready - start, intervalScale[1] = strokeStart - ready, intervalScale[2] = stroke - strokeStart, intervalScale[3] = strokeEnd - stroke, intervalScale[4] = relax - strokeEnd, intervalScale[5] = end - relax.
		for (std::vector<std::pair<int, int> >::iterator iter = intervals.begin();
			iter != intervals.end();
			iter++)
		{
			int start = (*iter).first;
			int end = (*iter).second;

			double scale = 1.0;
			if ((rawTimes[end] - rawTimes[start]) != 0)
				scale = (syncTimes[end] - syncTimes[start]) / (rawTimes[end] - rawTimes[start]);


			for (int x = lastScaleIndex; x < end; x++)
			{
				intervalScale.push_back(scale);
			}
			lastScaleIndex = end;
			lastScale = scale;
		}
		if (intervalScale.size() < 6)
		{
			for (int x = intervalScale.size() - 1; x < 6; x++)
			{	
				intervalScale.push_back(lastScale);
			}
		}
		// at this point, intervalScale[] should contain the scaling for those particular intervals
		// now determine the proper execution times based on the interval scaling

		// set the times that 
		lastScaleIndex = 0;
		for (std::vector<std::pair<int, int> >::iterator iter = intervals.begin();
			iter != intervals.end();
			iter++)
		{
			int start = (*iter).first;
			int end = (*iter).second;
			for (int x = end; x >= lastScaleIndex && x > 0; x--)
			{
				if (x - 1 == start)
					continue;
				double startSyncPointTime = syncTimes[x];
				double originalInterval = rawTimes[x] - rawTimes[x - 1];
				double newInterval = originalInterval * intervalScale[x - 1];
				syncTimes[x - 1] = startSyncPointTime - newInterval;
			}
			lastScaleIndex = end + 1;
		}
		// make sure the end points are set as well
		for (int x = lastScaleIndex; x < 7; x++)
		{
			double lastSyncPointTime = syncTimes[x - 1];
			double originalInterval = rawTimes[x] - rawTimes[x - 1];
			double newInterval = originalInterval * intervalScale[x - 1];
			syncTimes[x] = lastSyncPointTime + newInterval;
		}
	}

	start->time = syncTimes[0];
	ready->time = syncTimes[1];
	stroke_start->time = syncTimes[2];
	stroke->time = syncTimes[3];
	stroke_end->time = syncTimes[4];
	relax->time = syncTimes[5];
	end->time = syncTimes[6];
}




// Part of a transitional move to BehaviorSchedulers
BehaviorSchedulerConstantSpeedPtr BML::buildSchedulerForController( MeController* ct ) {
	bool is_undefined_duration = ct->controller_duration() < 0;

	MeCtMotion* motionController = dynamic_cast<MeCtMotion*>(ct);
	if (motionController)
	{
		SkMotion* motion = motionController->motion();

		BehaviorSchedulerConstantSpeedPtr scheduler( 
			new BehaviorSchedulerConstantSpeed(
				/* startTime  */ 0, 
				/* readyTime  */ time_sec( motionController->time_ready() ), 
				/* strokeStartTime */ time_sec( motionController->time_stroke_start() ),
				/* strokeTime */ time_sec( motionController->time_stroke_emphasis() ),
				/* strokeEndTime */ time_sec(  motionController->time_stroke_end()  ),
				/* relaxTime  */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : motionController->time_relax()  ), 
				/* endTime    */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : motionController->time_stop() ),
				/* speed      */ 1 ) );

		return scheduler;
	}
	else
	{
		BehaviorSchedulerConstantSpeedPtr scheduler( 
			new BehaviorSchedulerConstantSpeed(
				/* startTime  */ 0, 
				/* readyTime  */ time_sec( ct->indt() ), 
				/* strokeStartTime */ time_sec( ct->emphasist() ),
				/* strokeTime */ time_sec( ct->emphasist() ),
				/* strokeEndTime */ time_sec( ct->emphasist() ),
				/* relaxTime  */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION :  ct->controller_duration() - ct->outdt() ), 
				/* endTime    */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : ct->controller_duration() ),
				/* speed      */ 1 ) );

		return scheduler;
	}
}
