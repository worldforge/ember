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

#include "AvatarAttachmentController.h"

#include "components/ogre/Avatar.h"


namespace EmberOgre
{

AvatarAttachmentController::AvatarAttachmentController(Avatar& avatar)
: mAvatar(avatar)
{

}

AvatarAttachmentController::~AvatarAttachmentController()
{
}

const WFMath::Point<3>& AvatarAttachmentController::getPosition() const
{
	mPosition = mAvatar.getClientSideAvatarPosition();
	return mPosition;
}
const WFMath::Quaternion& AvatarAttachmentController::getOrientation() const
{
	mOrientation = mAvatar.getClientSideAvatarOrientation();
	return mOrientation;
}

const WFMath::Vector<3>& AvatarAttachmentController::getVelocity() const
{
	mVelocity = mAvatar.getClientSideAvatarVelocity();
	return mVelocity;
}



}
