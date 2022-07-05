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
#include "components/ogre/widgets/ModelRenderer.h"
#include "components/ogre/model/Model.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::OgreView::Model;
using namespace Ember::Lua;

template <>
void registerLua<ModelRenderer>(sol::table& space) {
	auto modelRenderer = space.new_usertype<ModelRenderer>("ModelRenderer",
	sol::constructors<ModelRenderer(CEGUI::Window* image, const std::string&)>(),
			sol::base_classes, sol::bases<MovableObjectRenderer>()
	);
	modelRenderer["showModel"] = sol::overload(sol::resolve<const std::string&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel),
											   sol::resolve<const ModelDefinitionPtr&, const Ogre::Vector3&, const Ogre::Quaternion&>(&ModelRenderer::showModel),
											   [](ModelRenderer* self, const char* model) { self->showModel(model); },
											   [](ModelRenderer* self, const ModelDefinitionPtr& model) { self->showModel(model); });
	modelRenderer["getModel"] = &ModelRenderer::getModel;
	modelRenderer["repositionSceneNode"] = &ModelRenderer::repositionSceneNode;

}