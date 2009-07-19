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

#ifndef AVATARCAMERAMOTIONHANDLER_H_
#define AVATARCAMERAMOTIONHANDLER_H_

#include "ICameraMotionHandler.h"

namespace EmberOgre {
class Avatar;

class AvatarCameraMotionHandler : public ICameraMotionHandler
{
public:
	AvatarCameraMotionHandler(Avatar& avatar);
	virtual ~AvatarCameraMotionHandler();

	virtual void move(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice);
private:
	Avatar& mAvatar;
};

}

#endif /* AVATARCAMERAMOTIONHANDLER_H_ */
