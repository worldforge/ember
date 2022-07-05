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
#include "components/ogre/widgets/AssetsManager.h"


using namespace Ember::OgreView;
using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<AssetsManager>(sol::table& space) {
	auto texturePair = space.new_usertype<TexturePair>("TexturePair", sol::no_constructor);
	texturePair["hasData"] = &TexturePair::hasData;
	texturePair["ogreTexture"] = sol::property([](TexturePair* self) { return std::shared_ptr(self->ogreTexture); });
	texturePair["textureImage"] = &TexturePair::textureImage;

	auto assetsManager = space.new_usertype<AssetsManager>("AssetsManager");
	assetsManager["showTexture"] = &AssetsManager::showTexture;
	assetsManager["createTextureImage"] = [](AssetsManager* self, const std::shared_ptr<Ogre::Texture>& texture, const char* imageSetName) {
		Ogre::SharedPtr<Ogre::Texture> ogreTexture(texture);
		return self->createTextureImage(ogreTexture, imageSetName);
	};
	//assetsManager["materialAsText"] = [](const std::shared_ptr <Ogre::Material>& material) { return AssetsManager::materialAsText({material}); };
	assetsManager["materialAsText"] = &AssetsManager::materialAsText;
	assetsManager["resolveResourceNameFromFilePath"] = &AssetsManager::resolveResourceNameFromFilePath;
	assetsManager["resolveFilePathForMesh"] = &AssetsManager::resolveFilePathForMesh;
	assetsManager["exportMesh"] = &AssetsManager::exportMesh;
	assetsManager["createModel"] = &AssetsManager::createModel;
	assetsManager["getLodInjectorSignaler"] = &AssetsManager::getLodInjectorSignaler;

}