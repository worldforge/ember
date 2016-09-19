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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef AVATARATTACHMENTCONTROLLER_H_
#define AVATARATTACHMENTCONTROLLER_H_

#include "domain/IEntityControlDelegate.h"
#include <wfmath/point.h>
#include <wfmath/quaternion.h>
#include <wfmath/vector.h>

namespace Ember
{
namespace OgreView
{

class Avatar;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Delegates control of an entity to the Avatar class.
 *
 * This allows an ingame entity (typically the one representing the user) to be controlled by the client.
 */
class AvatarAttachmentController: public IEntityControlDelegate
{
public:

	/**
	 * @brief Ctor.
	 * @param avatar The avatar instance which will control the Entity.
	 */
	AvatarAttachmentController(Avatar& avatar);

	/**
	 * @brief Dtor.
	 */
	virtual ~AvatarAttachmentController();

	virtual const WFMath::Point<3>& getPosition() const;
	virtual const WFMath::Quaternion& getOrientation() const;
	virtual const WFMath::Vector<3>& getVelocity() const;

private:

	/**
	 * @brief The avatar, through which position, orientation and velocity is obtained.
	 */
	Avatar& mAvatar;

	/**
	 * @brief The position, as provided by the Avatar.
	 *
	 * This is mutable to allow for the conversion from non-reference values from the Avatar to reference values expected by the IEntityControlDelegate interface.
	 */
	mutable WFMath::Point<3> mPosition;

	/**
	 * @brief The orientation, as provided by the Avatar.
	 *
	 * This is mutable to allow for the conversion from non-reference values from the Avatar to reference values expected by the IEntityControlDelegate interface.
	 */
	mutable WFMath::Quaternion mOrientation;

	/**
	 * @brief The velocity, as provided by the Avatar.
	 *
	 * This is mutable to allow for the conversion from non-reference values from the Avatar to reference values expected by the IEntityControlDelegate interface.
	 */
	mutable WFMath::Vector<3> mVelocity;
};

}

}

#endif /* AVATARATTACHMENTCONTROLLER_H_ */
