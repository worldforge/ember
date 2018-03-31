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

#include "CameraMountBase.h"
#include "MainCamera.h"
#include "framework/Exception.h"

#include "components/ogre/ICameraMotionHandler.h"
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>

namespace Ember
{
namespace OgreView
{
namespace Camera
{

CameraMountBase::CameraMountBase(const CameraSettings& cameraSettings)
: mCamera(nullptr),
  mMotionHandler(nullptr),
  mCameraSettings(cameraSettings)
{

}

CameraMountBase::~CameraMountBase()
{

}

Ogre::Degree CameraMountBase::getPitch() const
{
	throw Exception("Not implemented yet.");
// 	return degreePitch;
}

Ogre::Degree CameraMountBase::getYaw() const
{
	throw Exception("Not implemented yet.");
// 	return degreeYaw;
}

void CameraMountBase::attachToCamera(MainCamera& camera)
{
	mCamera = &camera.getCamera();
}

void CameraMountBase::detachFromCamera()
{
	mCamera = nullptr;
}

void CameraMountBase::move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation, float timeslice)
{
	if (mMotionHandler) {
		mMotionHandler->move(orientation, movement, timeslice);
	}
}

void CameraMountBase::setMotionHandler(ICameraMotionHandler* handler)
{
	mMotionHandler = handler;
}

}
}
}
