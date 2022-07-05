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
#include "components/ogre/widgets/OgreEntityRenderer.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<OgreEntityRenderer>(sol::table& space) {
	auto ogreEntityRenderer = space.new_usertype<OgreEntityRenderer>("OgreEntityRenderer",
																   sol::constructors<OgreEntityRenderer(CEGUI::Window*)>(),
																   sol::base_classes, sol::bases<MovableObjectRenderer>()
	);
	ogreEntityRenderer["showEntity"] = &OgreEntityRenderer::showEntity;
	ogreEntityRenderer["unloadEntity"] = &OgreEntityRenderer::unloadEntity;
	ogreEntityRenderer["getEntity"] = &OgreEntityRenderer::getEntity;
	ogreEntityRenderer["setAutoShowFull"] = &OgreEntityRenderer::setAutoShowFull;
	ogreEntityRenderer["getWireframeMode"] = &OgreEntityRenderer::getWireframeMode;
	ogreEntityRenderer["setWireframeMode"] = &OgreEntityRenderer::setWireframeMode;
	ogreEntityRenderer["setForcedLodLevel"] = &OgreEntityRenderer::setForcedLodLevel;
	ogreEntityRenderer["clearForcedLodLevel"] = &OgreEntityRenderer::clearForcedLodLevel;
	ogreEntityRenderer["enableAnimation"] = &OgreEntityRenderer::enableAnimation;
	ogreEntityRenderer["getEntityAnimationNames"] = &OgreEntityRenderer::getEntityAnimationNames;
	ogreEntityRenderer["setShowSkeleton"] = &OgreEntityRenderer::setShowSkeleton;

}