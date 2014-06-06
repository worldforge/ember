#ifndef BEHAVIOR_SCHEDULER_GESTURE_HPP
#define BEHAVIOR_SCHEDULER_GESTURE_HPP

#include "behavior_scheduler.hpp"
#include "controllers/me_controller.h"


namespace BML {
	/**
	 *  Schedules a behavior with a linearly adjustable duration.
	 *  Specifically made for gestures
	 */
	class BehaviorSchedulerGesture : public BehaviorScheduler {
		public:
	        // preferred "local times" of sync points
		    time_sec startTime;
			time_sec readyTime;
	        time_sec strokeStartTime;
	        time_sec strokeTime;
	        time_sec strokeEndTime;
		    time_sec relaxTime;
			time_sec endTime;

		public:
	        // preferred "local times" of sync points
			BehaviorSchedulerGesture(
				time_sec startTime,
				time_sec readyTime,
				time_sec strokeStartTime,
				time_sec strokeTime,
				time_sec strokeEndTime,
				time_sec relaxTime,
				time_sec endTime);

			virtual void schedule( BehaviorSyncPoints& behav_syncs, time_sec now );
	};
	typedef boost::shared_ptr<BehaviorSchedulerGesture> BehaviorSchedulerGesturePtr;

	/**
	 *  Build a linear BehaviorScheduler using the metadata of a MeController.
	 */
	BehaviorSchedulerGesturePtr buildGestureSchedulerForController( MeController* ct );

} // namespace BML

#endif // BEHAVIOR_SCHEDULER_GESTURE_HPP
