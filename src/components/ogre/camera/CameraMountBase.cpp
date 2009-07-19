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

#include "CameraMountBase.h"
#include "MainCamera.h"
#include "framework/Exception.h"

#include "components/ogre/ICameraMotionHandler.h"
#include <OgreMath.h>
#include <wfmath/vector.h>
#include <wfmath/quaternion.h>

namespace EmberOgre
{
namespace Camera
{

CameraMountBase::CameraMountBase()
: mCamera(0), mMotionHandler(0)
{

}

CameraMountBase::~CameraMountBase()
{

}

const Ogre::Degree& CameraMountBase::getPitch() const
{
	throw Ember::Exception("Not implemented yet.");
// 	return degreePitch;
}

const Ogre::Degree& CameraMountBase::getYaw() const
{
	throw Ember::Exception("Not implemented yet.");
// 	return degreeYaw;
}

void CameraMountBase::attachToCamera(MainCamera& camera)
{
	mCamera = &camera.getCamera();
}

void CameraMountBase::detachFromCamera()
{

}

void CameraMountBase::move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation)
{
	if (mMotionHandler) {
		mMotionHandler->move(orientation, movement);
	}
}

void CameraMountBase::setMotionHandler(ICameraMotionHandler* handler)
{
	mMotionHandler = handler;
}

}
}
