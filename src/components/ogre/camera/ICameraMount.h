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

#ifndef ICAMERAMOUNT_H_
#define ICAMERAMOUNT_H_

#include "components/ogre/OgreIncludes.h"

namespace WFMath
{
template<int> class Vector;
class Quaternion;
}

namespace EmberOgre
{
class ICameraMotionHandler;

namespace Camera
{
class MainCamera;

class ICameraMount
{
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~ICameraMount() {};

	/**
	 * @brief Pitches the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree pitch(float relativeMovement) = 0;

	/**
	 * @brief Yaws the camera the supplied degrees
	 * @param degrees
	 */
	virtual Ogre::Degree yaw(float relativeMovement) = 0;

	/**
	 * @brief Returns the current degrees of pitch from the cameras initial position.
	 * @return
	 */
	virtual const Ogre::Degree& getPitch() const = 0;

	/**
	 * @brief Returns the current degrees of yaw from the cameras initial position.
	 * @return
	 */
	virtual const Ogre::Degree& getYaw() const = 0;

	virtual void move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation, float timeslice) = 0;

	virtual void attachToCamera(MainCamera& camera) = 0;

	virtual void detachFromCamera() = 0;

	virtual void setMotionHandler(ICameraMotionHandler* handler) = 0;

};
}
}
#endif /* ICAMERAMOUNT_H_ */
