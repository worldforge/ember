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

#ifndef STEERING_H_
#define STEERING_H_

#include <wfmath/point.h>
#include <wfmath/vector.h>

#include <list>

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

namespace Eris
{
class Avatar;
}

namespace Ember
{
class TimeFrame;
namespace Navigation
{

class Awareness;

/**
 * @brief Handles steering of an avatar, with path finding and obstacle avoidance.
 */
class Steering: public virtual sigc::trackable
{
public:
	Steering(Awareness& awareness, Eris::Avatar& avatar);
	virtual ~Steering();

	void setDestination(const WFMath::Point<3>& viewPosition);
	/**
	 * @brief Updates the path.
	 * @return True if a path was found.
	 */
	bool updatePath();

	/**
	 * @brief Requests an update of the path.
	 *
	 * The actual update will be deferred to when updatePath() is called, which normally happens
	 * with a call to update()
	 */
	void requestUpdate();

	void startSteering();
	void stopSteering();
	bool isEnabled() const;

	void setSpeed(float speed);

	const std::list<WFMath::Point<3>>& getPath() const;

	/**
	 * @brief Returns true if we've just sent a movement update to the server and thus expect an update in return.
	 *
	 * This is useful to know whether any movement update received from the server was instigated by us or not.
	 * @return
	 */
	bool getIsExpectingServerMovement() const;

	/**
	 * @brief Sets if we're expecting an update from the server.
	 * @param expected
	 */
	void setIsExpectingServerMovement(bool expected);

	/**
	 * @brief Updates the steering.
	 *
	 * Call this often when steering is enabled.
	 */
	void update();

	/**
	 * @brief Emitted when the path has been updated.
	 */
	sigc::signal<void> EventPathUpdated;

private:

	Awareness& mAwareness;
	Eris::Avatar& mAvatar;

	WFMath::Point<3> mViewDestination;
	std::list<WFMath::Point<3>> mPath;
	bool mSteeringEnabled;
	bool mUpdateNeeded;

	float mPadding;

	float mSpeed;

	bool mExpectingServerMovement;
	WFMath::Vector<2> mLastSentVelocity;

	void setAwareness();

	void Awareness_TileUpdated(int tx, int ty);

	void moveInDirection(const WFMath::Vector<2>& direction);
	void moveToPoint(const WFMath::Point<3>& point);
};

}
}
#endif /* STEERING_H_ */
