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
#include <wfmath/rotbox.h>

namespace Ember
{
namespace Navigation
{

Steering::Steering(Awareness& awareness, Eris::Avatar& avatar) :
		mAwareness(awareness), mAvatar(avatar), mSteeringEnabled(false), mUpdateNeeded(false), mPadding(16)
{
	MainLoopController::getSingleton().EventFrameProcessed.connect(sigc::mem_fun(*this, &Steering::frameProcessed));
	mAwareness.EventTileUpdated.connect(sigc::mem_fun(*this, &Steering::Awareness_TileUpdated));
}

Steering::~Steering()
{
}

void Steering::setDestination(const WFMath::Point<3>& viewPosition)
{
	mViewDestination = viewPosition;

	const auto entityViewPosition = mAvatar.getEntity()->getViewPosition();

	WFMath::Point<2> destination2d(viewPosition.x(), viewPosition.y());
	WFMath::Point<2> entityPosition2d(entityViewPosition.x(), entityViewPosition.y());

	WFMath::Vector<2> direction(destination2d - entityPosition2d);
	double theta = atan2(direction.y(), direction.x()); // rotation about Z
	WFMath::RotMatrix<2> rm;
	rm.rotation(theta);

	WFMath::Point<2> start = entityPosition2d;
	start -= WFMath::Vector<2>(mPadding, mPadding);

	WFMath::Vector<2> size(direction.mag() + (mPadding * 2), mPadding * 2);

	WFMath::RotBox<2> area;
	area.size() = size;
	area.corner0() = start;
	area.orientation() = WFMath::RotMatrix<2>().identity();
	area.rotatePoint(rm, entityPosition2d);

	mAwareness.addAwarenessArea(area);

}

bool Steering::updatePath()
{
	mPath.clear();

	int result = mAwareness.findPath(mAvatar.getEntity()->getViewPosition(), mViewDestination, mPath);
	mUpdateNeeded = false;
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

bool Steering::isEnabled() const
{
	return mSteeringEnabled;
}

const std::list<WFMath::Point<3>>& Steering::getPath() const
{
	return mPath;
}

void Steering::frameProcessed(const TimeFrame&, unsigned int)
{
	if (mUpdateNeeded) {
		updatePath();
	}
	if (mSteeringEnabled) {
		if (!mPath.empty()) {
			auto entity = mAvatar.getEntity();
			const auto& finalDestination = mPath.back();
			auto entity3dPosition = entity->getViewPosition();
			const WFMath::Point<2> entityPosition(entity3dPosition.x(), entity3dPosition.y());
			if (WFMath::Distance(WFMath::Point<2>(finalDestination.x(), finalDestination.y()), entityPosition) < 0.1f) {
				//We've arrived at our destination. If we're moving we should stop.
				if (entity->isMoving()) {
					mAvatar.moveInDirection(WFMath::Vector<3>::ZERO());
				}
				stopSteering();
			} else {
				//We should send a move op is we're either not moving, or we've reached a waypoint.
				bool needToMove = !entity->isMoving();
				WFMath::Point<2> nextWaypoint(mPath.front().x(), mPath.front().y());
				if (WFMath::Distance(nextWaypoint, entityPosition) < 0.1f) {
					mPath.pop_front();
					nextWaypoint = WFMath::Point<2>(mPath.front().x(), mPath.front().y());
					needToMove = true;
				}
				if (needToMove) {
					WFMath::Vector<2> direction = nextWaypoint - entityPosition;
					direction.normalize();
					direction *= 5;

					mAvatar.moveInDirection(WFMath::Vector<3>(direction.x(), direction.y(), 0));
				}
			}
		}
	}
}

void Steering::Awareness_TileUpdated(int tx, int ty)
{
	mUpdateNeeded = true;
}

}
}
