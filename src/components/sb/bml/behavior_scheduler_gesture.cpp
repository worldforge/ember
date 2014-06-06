#include "vhcl.h"

#include "bml.hpp"
#include "behavior_scheduler_gesture.hpp"

using namespace std;
using namespace BML;

/////////////////////////////////////////////////////////////////////////////
//  BehaviorSchedulerGesture
//


BehaviorSchedulerGesture::BehaviorSchedulerGesture(
    // preferred "local times" of sync points
    time_sec startTime,
	time_sec readyTime,
	time_sec strokeStartTime,
    time_sec strokeTime,
	time_sec strokeEndTime,
    time_sec relaxTime,
	time_sec endTime
) :
    // preferred "local times" of sync points
    startTime( startTime ),
	readyTime( readyTime ),
    strokeStartTime( strokeStartTime ),
    strokeTime( strokeTime ),
    strokeEndTime( strokeEndTime ),
    relaxTime( relaxTime ),
	endTime( endTime )
{		
}

void BehaviorSchedulerGesture::schedule( BehaviorSyncPoints& behav_syncs, time_sec now ) {
	// local references to standard sync points
	SyncPointPtr start        = behav_syncs.sync_start()->sync();
	SyncPointPtr ready        = behav_syncs.sync_ready()->sync();
	SyncPointPtr stroke_start = behav_syncs.sync_stroke_start()->sync();
	SyncPointPtr stroke       = behav_syncs.sync_stroke()->sync();
	SyncPointPtr stroke_end   = behav_syncs.sync_stroke_end()->sync();
	SyncPointPtr relax        = behav_syncs.sync_relax()->sync();
	SyncPointPtr end          = behav_syncs.sync_end()->sync();

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
	else	// only takes in stroke and relax for now
	{
		if (!(syncPointsSet[3] && syncPointsSet[5]))
		{
			LOG("BehaviorSchedulerGesture::schedule WARNING: you can only specify stroke and relax or any one of the sync points for GESTURE BML, starting behavior now...");
			start_at = now;
			for (int x = 0; x < 7; x++)
			{
				syncTimes[x] = start_at + rawTimes[x];
			}
		}
		else
		{		
			start_at = now;
			double offset = syncTimes[3] - (rawTimes[3] + start_at);
			syncTimes[0] = rawTimes[0] + start_at + offset;
			syncTimes[1] = rawTimes[1] + start_at + offset;
			syncTimes[2] = rawTimes[2] + start_at + offset;
			syncTimes[4] = rawTimes[4] + start_at + offset;
			double offset1 = syncTimes[5] - (rawTimes[5] + start_at);
			syncTimes[6] = rawTimes[6] + start_at + offset1;

			double gap = (syncTimes[5] - syncTimes[3]) - (rawTimes[5] - rawTimes[3]);
			if (gap < 0)
			{
				//LOG("sync time stroke->relax time is shorter than motion stroke->relax time by %f, re-adjust...", gap);
				syncTimes[5] += fabs(gap);
				syncTimes[6] += fabs(gap);
			}
		}
	}

	start->time = syncTimes[0];
	ready->time = syncTimes[1];
	stroke_start->time = syncTimes[2];
	stroke->time = syncTimes[3];
	stroke_end->time = syncTimes[4];
	relax->time = syncTimes[5];
	end->time = syncTimes[6];

	//LOG("BehaviorSchedulerGesture::schedule timing: %f, %f, %f, %f, %f, %f, %f", syncTimes[0], syncTimes[1], syncTimes[2], syncTimes[3], syncTimes[4], syncTimes[5], syncTimes[6]);
}

// Part of a transitional move to BehaviorSchedulers
BehaviorSchedulerGesturePtr BML::buildGestureSchedulerForController( MeController* ct ) {
	bool is_undefined_duration = ct->controller_duration() < 0;

	MeCtMotion* motionController = dynamic_cast<MeCtMotion*>(ct);
	if (motionController)
	{
		SkMotion* motion = motionController->motion();

		BehaviorSchedulerGesturePtr scheduler( 
			new BehaviorSchedulerGesture(
			/* startTime  */ 0, 
			/* readyTime  */ time_sec( motionController->time_ready() ), 
			/* strokeStartTime */ time_sec( motionController->time_stroke_start() ),
			/* strokeTime */ time_sec( motionController->time_stroke_emphasis() ),
			/* strokeEndTime */ time_sec(  motionController->time_stroke_end()  ),
			/* relaxTime  */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : motionController->time_relax()  ), 
			/* endTime    */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : motionController->time_stop() )
			) );
		return scheduler;
	}
	else
	{
		BehaviorSchedulerGesturePtr scheduler( 
			new BehaviorSchedulerGesture(
			/* startTime  */ 0, 
			/* readyTime  */ time_sec( ct->indt() ), 
			/* strokeStartTime */ time_sec( ct->emphasist() ),
			/* strokeTime */ time_sec( ct->emphasist() ),
			/* strokeEndTime */ time_sec( ct->emphasist() ),
			/* relaxTime  */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION :  ct->controller_duration() - ct->outdt() ), 
			/* endTime    */ time_sec( is_undefined_duration? BehaviorRequest::TEN_MILLION : ct->controller_duration() )
			) );

		return scheduler;
	}
}
