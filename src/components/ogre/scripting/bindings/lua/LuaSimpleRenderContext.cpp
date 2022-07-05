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
#include "components/ogre/SimpleRenderContext.h"


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<SimpleRenderContext>(sol::table& space) {
	auto simpleRenderContext = space.new_usertype<SimpleRenderContext>("SimpleRenderContext",
																		  sol::constructors<
																				  SimpleRenderContext(const std::string&, int, int),
																				  SimpleRenderContext(const std::string&, Ogre::TexturePtr)
																		  >());
	simpleRenderContext["getSceneNode"] = &SimpleRenderContext::getSceneNode;
	simpleRenderContext["getCamera"] = &SimpleRenderContext::getCamera;
	simpleRenderContext["getDefaultCameraDistance"] = &SimpleRenderContext::getDefaultCameraDistance;
	simpleRenderContext["setActive"] = &SimpleRenderContext::setActive;
	simpleRenderContext["repositionCamera"] = &SimpleRenderContext::repositionCamera;
	simpleRenderContext["pitchCamera"] = &SimpleRenderContext::pitchCamera;
	simpleRenderContext["yawCamera"] = &SimpleRenderContext::yawCamera;
	simpleRenderContext["rollCamera"] = &SimpleRenderContext::rollCamera;
	simpleRenderContext["setCameraDistance"] = &SimpleRenderContext::setCameraDistance;
	simpleRenderContext["getCameraDistance"] = &SimpleRenderContext::getCameraDistance;
	simpleRenderContext["getAbsoluteCameraDistance"] = &SimpleRenderContext::getAbsoluteCameraDistance;
	simpleRenderContext["getCameraOrientation"] = &SimpleRenderContext::getCameraOrientation;
	simpleRenderContext["resetCameraOrientation"] = &SimpleRenderContext::resetCameraOrientation;
	simpleRenderContext["setCameraPositionMode"] = &SimpleRenderContext::setCameraPositionMode;
	simpleRenderContext["CPM_OBJECTCENTER"] = sol::var(SimpleRenderContext::CPM_OBJECTCENTER);
	simpleRenderContext["CPM_WORLDCENTER"] = sol::var(SimpleRenderContext::CPM_WORLDCENTER);
	simpleRenderContext["CPM_FREE"] = sol::var(SimpleRenderContext::CPM_FREE);

}