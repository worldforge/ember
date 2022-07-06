/*
 Copyright (C) 2022 Erik Ogenvik

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
#include "RegisterLua.h"

using namespace Ogre;

template<>
void registerLua<SceneNode>(sol::table& space) {

	auto sceneNode = space.new_usertype<SceneNode>("SceneNode", sol::no_constructor);
	sceneNode["showBoundingBox"] = &SceneNode::showBoundingBox;
	sceneNode["getShowBoundingBox"] = &SceneNode::getShowBoundingBox;
	sceneNode["getOrientation"] = &SceneNode::getOrientation;
	sceneNode["setOrientation"] = sol::resolve<void(const Quaternion&)>(&SceneNode::setOrientation);
	sceneNode["setPosition"] = sol::resolve<void(const Vector3&)>(&SceneNode::setPosition);
	sceneNode["getPosition"] = &SceneNode::getPosition;
	sceneNode["setScale"] = sol::resolve<void(const Vector3&)>(&SceneNode::setScale);
	sceneNode["getScale"] = &SceneNode::getScale;
	sceneNode["yaw"] = [](SceneNode* self, const Radian& angle) { self->yaw(angle); };
	sceneNode["pitch"] = [](SceneNode* self, const Radian& angle) { self->pitch(angle); };
	sceneNode["roll"] = [](SceneNode* self, const Radian& angle) { self->roll(angle); };
	sceneNode["getChildByIndex"] = [](const SceneNode* self, unsigned short index) { return dynamic_cast<SceneNode*>(self->getChild(index)); };


}