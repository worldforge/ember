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

#ifndef FREEFLYINGCAMERAMOTIONHANDLER_H_
#define FREEFLYINGCAMERAMOTIONHANDLER_H_
#include "ICameraMotionHandler.h"

#include "components/ogre/OgreIncludes.h"

namespace Ember {
namespace OgreView {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A motion handler for a free flying camera.
 * The handler is attached to an Ogre::SceneNode which is moved without constraints.
 */
class FreeFlyingCameraMotionHandler : public ICameraMotionHandler
{
public:
	explicit FreeFlyingCameraMotionHandler(Ogre::SceneNode& freeFlyingNode);

	~FreeFlyingCameraMotionHandler() override;

	void move(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice) override;

	float getSpeed() const;

	void setSpeed(float speed);

private:
	Ogre::SceneNode& mFreeFlyingNode;
	float mSpeed;
};

}

}

#endif /* FREEFLYINGCAMERAMOTIONHANDLER_H_ */
