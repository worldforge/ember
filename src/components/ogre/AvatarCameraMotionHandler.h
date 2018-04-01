/*
 Copyright (C) 2009 Erik Ogenvik

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

#ifndef AVATARCAMERAMOTIONHANDLER_H_
#define AVATARCAMERAMOTIONHANDLER_H_

#include "ICameraMotionHandler.h"

namespace Ember
{
namespace OgreView
{
class Avatar;

/**
 * @brief A camera motion handler which directs camera motions to the Avatar, making the Avatar entity move.
 *
 * By using this the user will have control of the Avatar, and any movement will be translated into entity movement.
 */
class AvatarCameraMotionHandler: public ICameraMotionHandler
{
public:

	/**
	 * Ctor.
	 * @param avatar The avatar which will be moved as the camera moves.
	 */
	explicit AvatarCameraMotionHandler(Avatar& avatar);

	/**
	 * Dtor.
	 */
	~AvatarCameraMotionHandler() override = default;

	void move(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice) override;
private:

	/**
	 * @brief The avatar which will be moved as the camera moves.
	 */
	Avatar& mAvatar;
};

}

}

#endif /* AVATARCAMERAMOTIONHANDLER_H_ */
