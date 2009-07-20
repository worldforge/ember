/*
    Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef IMOVABLE_H_
#define IMOVABLE_H_

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents something which can move, and therefore needs to get movement updates.
 *
 * An instance of this should be registered with the MotionManager. This will ensure that it will receive requests for movement updates through calls to the updateMotion() method each frame.
 * It's up to the actual implementation to determine how to present the movement update. For something attached to an Ogre::SceneNode it would be suitable to update the position and orientation of the scene node.
 */
class IMovable
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IMovable() {};

	/**
	 * @brief Called each frame to inform the implementation that it needs to update the motion.
	 * @param timeSlice The current time slice, in seconds.
	 */
	virtual void updateMotion(float timeSlice) = 0;
};

}

#endif /* IMOVABLE_H_ */
