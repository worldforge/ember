/*
 *  behavior_scheduler_constant_speed.hpp - part of SmartBody-lib
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

#ifndef BEHAVIOR_SCHEDULER_CONSTANT_SPEED_HPP
#define BEHAVIOR_SCHEDULER_CONSTANT_SPEED_HPP

#include "behavior_scheduler.hpp"
#include "controllers/me_controller.h"


/**
 *  Transitional mode for calculating the behavior schedules by
 *  durations instead of a fictious local timeline.
 */

namespace BML {
	/**
	 *  Schedules a behavior with a linearly adjustable duration.
	 *  The proportions between sync points will always remain constant by default.
	 *	By toggle boolean variable 'constant', you can choose to break the proportions. 
	 *	(This feature is added to make the behavior sync points manipulation easier).
	 *  Often used by controller-based behaviors.
	 */
	class BehaviorSchedulerConstantSpeed : public BehaviorScheduler {
		public:
	        // preferred "local times" of sync points
		    time_sec startTime;
			time_sec readyTime;
	        time_sec strokeStartTime;
	        time_sec strokeTime;
	        time_sec strokeEndTime;
		    time_sec relaxTime;
			time_sec endTime;

	        time_sec speed; 

		public:
	        // preferred "local times" of sync points
			BehaviorSchedulerConstantSpeed(
				time_sec startTime,
				time_sec readyTime,
				time_sec strokeStartTime,
				time_sec strokeTime,
				time_sec strokeEndTime,
				time_sec relaxTime,
				time_sec endTime,
				time_sec speed );

			virtual void schedule( BehaviorSyncPoints& behav_syncs, time_sec now );
	};
	typedef boost::shared_ptr<BehaviorSchedulerConstantSpeed> BehaviorSchedulerConstantSpeedPtr;

	/**
	 *  Build a linear BehaviorScheduler using the metadata of a MeController.
	 */
	BehaviorSchedulerConstantSpeedPtr buildSchedulerForController( MeController* ct );

} // namespace BML

#endif // BEHAVIOR_SCHEDULER_CONSTANT_SPEED_HPP
