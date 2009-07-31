/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef ICAMERAMOTIONHANDLER_H_
#define ICAMERAMOTIONHANDLER_H_

namespace WFMath
{
template<int> class Vector;
class Quaternion;
}

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Handles movement requests from the camera mount.
 *
 * Each camera mount can have an instance of this attached to it. When the camera receives instructions to move, it will issue a move request through this interface.
 * It's then up to the implementations of this interface to determine what to do. Some implementations might want to only move the node that the camera is attached to, whilst other might want to move an entity instead.
 */
class ICameraMotionHandler
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~ICameraMotionHandler()
	{
	}

	/**
	 * @brief Move in the specified direction and using the specified orientation.
	 * It's up to the implementations of this interface to determine how they should react to a movement request.
	 * Some implementations might want to move the camera, while other might instead want to move an entity or avatar.
	 *
	 * @param orientation The orientation during the movement.
	 * @param movement The movement.
	 * @param timeslice The time slice, in seconds, for this movement.
	 */
	virtual void move(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice) = 0;
};

}
;
#endif /* ICAMERAMOTIONHANDLER_H_ */
