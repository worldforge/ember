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

#ifndef IANIMATED_H_
#define IANIMATED_H_

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents something which can be animated, and therefore needs to get animation updates.
 *
 * An instance of this should be registered with the MotionManager. This will ensure that it will receive requests for animation updates through calls to the updateAnimation() method each frame.
 * It's up to the actual implementation to determine how to present the animation update.
 * For something represented by an instance of Model it would be suitable to update the current animation of the model.
 */
class IAnimated
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IAnimated()
	{
	}

	/**
	 * @brief Updates the animation.
	 * @param timeSlice The time slice to advance the animation with.
	 */
	virtual void updateAnimation(float timeSlice) = 0;
};

}

#endif /* IANIMATED_H_ */
