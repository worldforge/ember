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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef STEERING_H_
#define STEERING_H_

#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>

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
class Loitering;

/**
 * @brief Handles steering of an avatar, with path finding and obstacle avoidance.
 *
 * To enable steering, first set the destination through setDestination() and then call startSteering().
 */
class Steering: public virtual sigc::trackable
{
public:
	Steering(Awareness& awareness, Eris::Avatar& avatar);
	virtual ~Steering();

	/**
	 * @brief Sets a new destination, in view position.
	 * Note that this won't start steering; you need to call startSteering() separately.
	 * @param viewPosition
	 */
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

	/**
	 * @brief Starts the steering.
	 */
	void startSteering();

	/**
	 * @brief Stops the steering.
	 */
	void stopSteering();

	/**
	 * @brief Returns true if steering currently is enabled.
	 * @return True if steering is enabled.
	 */
	bool isEnabled() const;

	/**
	 * @brief Gets the current path.
	 * @return The current path.
	 */
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
	sigc::signal<void()> EventPathUpdated;

private:

	Awareness& mAwareness;
	Eris::Avatar& mAvatar;

	/**
	 * @brief The destination, in view coordinates.
	 */
	WFMath::Point<3> mViewDestination;

	/**
	 * @brief The calculated path to the destination.
	 * The waypoints is in order, with the next on in the beginning of the list.
	 */
	std::list<WFMath::Point<3>> mPath;

	/**
	 * @brief True if steering currently is enabled.
	 */
	bool mSteeringEnabled;

	/**
	 * @brief True if the path should be recalculated the next time update() is called.
	 */
	bool mUpdateNeeded;

	/**
	 * @brief In world units how much padding to expand the awareness area with.
	 */
	float mPadding;

	/**
	 * @brief The desired speed.
	 */
	double mSpeed;

	/**
	 * @brief True if we're expecting a movement response from the server.
	 *
	 * If we've told the server to that we should move, we'll be expecting a response and need to handle that slightly different
	 * than if we got a movement update out of the blue.
	 */
	bool mExpectingServerMovement;

	/**
	 * @brief Keep track of the last sent velocity.
	 *
	 * This is mainly used to keep track of if we need to send a stop velocity, once we've reached our destination.
	 */
	WFMath::Vector<2> mLastSentVelocity;

	/**
	 * @brief Keeps awareness around the closest area once we've stopped moving.
	 *
	 * The idea here is that the avatar probably want to move within the closest area, so we'll keep awareness of that
	 * until the avatar has moved out of the area.
	 */
	std::unique_ptr<Loitering> mLoitering;

	/**
	 * @brief Sets the awareness to be a corridor between where the avatar currently is and our destination.
	 *
	 * mPadding determines the width of the corridor.
	 */
	void setAwareness();

	/**
	 * @brief Listen to tiles being updated, and request updates.
	 * @param tx
	 * @param ty
	 */
	void Awareness_TileUpdated(int tx, int ty);

	/**
	 * @brief Tells the server to move in a certain direction.
	 * @param direction The direction to move in.
	 */
	void moveInDirection(const WFMath::Vector<2>& direction);

	/**
	 * @brief Tells the server to move towards a certain point.
	 *
	 * The server will make sure to stop at the specified point, unless we collide with something.
	 * This should therefore be used to reach our final destination.
	 *
	 * @param point The point to move towards.
	 */
	void moveToPoint(const WFMath::Point<3>& point);
};

}
}
#endif /* STEERING_H_ */
