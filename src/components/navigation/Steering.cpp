/*
 Copyright (C) 2014 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Steering.h"
#include "Awareness.h"

#include "framework/MainLoopController.h"

#include <Eris/Avatar.h>
#include <Eris/Entity.h>

#include <wfmath/point.h>
#include <wfmath/vector.h>

namespace Ember
{
namespace Navigation
{

Steering::Steering(Awareness& awareness, Eris::Avatar& avatar) :
		mAwareness(awareness), mAvatar(avatar), mSteeringEnabled(false)
{
	MainLoopController::getSingleton().EventFrameProcessed.connect(sigc::mem_fun(*this, &Steering::frameProcessed));
}

Steering::~Steering()
{
}

bool Steering::setDestination(const WFMath::Point<3>& viewPosition)
{
	mViewDestination = viewPosition;
	mPath.clear();

	const auto entityViewPosition = mAvatar.getEntity()->getViewPosition();

	WFMath::Point<3> low(std::min(entityViewPosition.x(), mViewDestination.x()), std::min(entityViewPosition.y(), mViewDestination.y()), -100);
	WFMath::Point<3> high(std::max(entityViewPosition.x(), mViewDestination.x()), std::max(entityViewPosition.y(), mViewDestination.y()), 100);

	//Increase the area a little, by 16 meters in each direction.
	low -= WFMath::Vector<3>(16, 16, 0);
	high += WFMath::Vector<3>(16, 16, 0);

	mAwareness.addAwarenessArea(WFMath::AxisBox<3>(low, high, true));

	int result = mAwareness.findPath(entityViewPosition, viewPosition, mPath);
	return result > 0;
}

void Steering::startSteering()
{
	mSteeringEnabled = true;
}
void Steering::stopSteering()
{
	mSteeringEnabled = false;
}

const std::list<WFMath::Point<3>>& Steering::getPath() const
{
	return mPath;
}

void Steering::frameProcessed(const TimeFrame&, unsigned int)
{
	if (mSteeringEnabled) {
		if (!mPath.empty()) {
			auto entity = mAvatar.getEntity();
			const auto& finalDestination = mPath.back();
			if (WFMath::Distance(finalDestination, entity->getViewPosition()) < 0.1f) {
				//We've arrived at our destination. If we're moving we should stop.
				if (entity->isMoving()) {
					mAvatar.moveInDirection(WFMath::Vector<3>::ZERO());
				}
				stopSteering();
			} else {
				auto nextWaypoint = mPath.front();
				if (WFMath::Distance(nextWaypoint, entity->getViewPosition()) < 0.1f) {
					mPath.pop_front();
					nextWaypoint = mPath.front();
				}
				WFMath::Vector<3> direction = nextWaypoint - entity->getViewPosition();
				direction.normalize();
				direction *= 5;

				mAvatar.moveInDirection(direction);
			}
		}
	}
}

}
}
