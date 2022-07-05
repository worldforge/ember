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
#include "components/ogre/widgets/Compass.h"
#include "components/ogre/terrain/ITerrainAdapter.h"
#include "components/ogre/terrain/Map.h"


using namespace Ember::OgreView::Gui;
using namespace Ember::OgreView::Terrain;
using namespace Ember::Lua;

template <>
void registerLua<Compass>(sol::table& space) {
	auto compass = space.new_usertype<Compass>("Compass",
											 sol::constructors<Compass(Ember::OgreView::Gui::ICompassImpl*, Ogre::SceneManager&, ITerrainAdapter&)>()
	);
	compass["getMap"] = &Compass::getMap;
	compass["reposition"] = &Compass::reposition;
	compass["refresh"] = &Compass::refresh;

	auto renderedCompassImpl = space.new_usertype<RenderedCompassImpl>("RenderedCompassImpl",
																	 sol::constructors<RenderedCompassImpl(CEGUI::Window*)>(),
																	 sol::base_classes, sol::bases<ICompassImpl>());
	renderedCompassImpl["getTexture"] = [](RenderedCompassImpl* self) { return std::shared_ptr(self->getTexture()); };

	space.new_usertype<CompassCameraAnchor>("CompassCameraAnchor",
										  sol::constructors<CompassCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*)>());
	space.new_usertype<CompassSceneNodeAnchor>("CompassSceneNodeAnchor",
											 sol::constructors<CompassSceneNodeAnchor(Ember::OgreView::Gui::Compass&, Ogre::SceneNode*)>());
	space.new_usertype<CompassThirdPersonCameraAnchor>("CompassThirdPersonCameraAnchor",
													 sol::constructors<CompassThirdPersonCameraAnchor(Ember::OgreView::Gui::Compass&, Ogre::Camera*, Ogre::SceneNode*)>());

}