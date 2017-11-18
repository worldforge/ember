/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef CAMERAMOUNTBASE_H_
#define CAMERAMOUNTBASE_H_

#include "ICameraMount.h"
#include "components/ogre/OgreIncludes.h"

namespace WFMath
{
template<int> class Vector;
class Quaternion;
}

namespace Ember
{
namespace OgreView
{
class ICameraMotionHandler;

namespace Camera
{
class MainCamera;
class CameraSettings;

class CameraMountBase : public ICameraMount
{
public:

	CameraMountBase(const CameraSettings& cameraSettings);
	virtual ~CameraMountBase();

	/**
	 * @brief Returns the current degrees of pitch from the cameras initial position.
	 * @return
	 */
	virtual Ogre::Degree getPitch() const;

	/**
	 * @brief Returns the current degrees of yaw from the cameras initial position.
	 * @return
	 */
	virtual Ogre::Degree getYaw() const;

	virtual void setMotionHandler(ICameraMotionHandler* handler);

 	virtual void attachToCamera(MainCamera& camera);

 	virtual void detachFromCamera();

	virtual void move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation, float timeslice);


protected:
	Ogre::Camera* mCamera;
	ICameraMotionHandler* mMotionHandler;
	const CameraSettings& mCameraSettings;
};


}
}
}


#endif /* CAMERAMOUNTBASE_H_ */
