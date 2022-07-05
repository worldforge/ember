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
#include "components/ogre/widgets/MovableObjectRenderer.h"
#include "components/ogre/widgets/EntityCEGUITexture.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<MovableObjectRenderer>(sol::table& space) {
	auto movableObjectRenderer = space.new_usertype<MovableObjectRenderer>("MovableObjectRenderer", sol::no_constructor);
	movableObjectRenderer["showFull"] = &MovableObjectRenderer::showFull;
	movableObjectRenderer["setCameraDistance"] = &MovableObjectRenderer::setCameraDistance;
	movableObjectRenderer["getCameraDistance"] = &MovableObjectRenderer::getCameraDistance;
	movableObjectRenderer["getAbsoluteCameraDistance"] = &MovableObjectRenderer::getAbsoluteCameraDistance;
	movableObjectRenderer["getCameraOrientation"] = &MovableObjectRenderer::getCameraOrientation;
	movableObjectRenderer["setAutoShowFull"] = &MovableObjectRenderer::setAutoShowFull;
	movableObjectRenderer["getAutoShowFull"] = &MovableObjectRenderer::getAutoShowFull;
	movableObjectRenderer["getActive"] = &MovableObjectRenderer::getActive;
	movableObjectRenderer["setActive"] = &MovableObjectRenderer::setActive;
	movableObjectRenderer["getEntityRotation"] = &MovableObjectRenderer::getEntityRotation;
	movableObjectRenderer["setEntityRotation"] = &MovableObjectRenderer::setEntityRotation;
	movableObjectRenderer["getEntityTranslation"] = &MovableObjectRenderer::getEntityTranslation;
	movableObjectRenderer["setEntityTranslation"] = &MovableObjectRenderer::setEntityTranslation;
	movableObjectRenderer["resetCameraOrientation"] = &MovableObjectRenderer::resetCameraOrientation;
	movableObjectRenderer["pitchCamera"] = &MovableObjectRenderer::pitchCamera;
	movableObjectRenderer["yawCamera"] = &MovableObjectRenderer::yawCamera;
	movableObjectRenderer["rollCamera"] = &MovableObjectRenderer::rollCamera;
	movableObjectRenderer["updateRender"] = &MovableObjectRenderer::updateRender;
	movableObjectRenderer["showAxis"] = &MovableObjectRenderer::showAxis;
	movableObjectRenderer["hideAxis"] = &MovableObjectRenderer::hideAxis;
	movableObjectRenderer["getCameraPositionMode"] = &MovableObjectRenderer::getCameraPositionMode;
	movableObjectRenderer["setCameraPositionMode"] = &MovableObjectRenderer::setCameraPositionMode;
	movableObjectRenderer["getEntityTexture"] = &MovableObjectRenderer::getEntityTexture;
	movableObjectRenderer["setShowMovingLight"] = &MovableObjectRenderer::setShowMovingLight;

}