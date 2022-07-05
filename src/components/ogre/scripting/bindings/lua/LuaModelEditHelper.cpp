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
#include "components/ogre/widgets/ModelEditHelper.h"
#include "components/ogre/SimpleRenderContext.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::OgreView;
using namespace Ember::Lua;

template<>
void registerLua<ModelEditHelper>(sol::table& space) {
	auto modelEditHelper = space.new_usertype<ModelEditHelper>("ModelEditHelper",
															 sol::constructors<ModelEditHelper(Ember::OgreView::Model::Model * , SimpleRenderContext & )>());
	modelEditHelper["showAttachPointHelperEntity"] = sol::overload([](ModelEditHelper* self, const std::string& attachPointName, const std::string& meshName) { return self->showAttachPointHelperEntity(attachPointName, meshName); },
																   [](ModelEditHelper* self, const std::string& attachPointName) { return self->showAttachPointHelperEntity(attachPointName); });
	modelEditHelper["showAttachPointHelperModel"] = &ModelEditHelper::showAttachPointHelperModel;
	modelEditHelper["hideAttachPointHelper"] = &ModelEditHelper::hideAttachPointHelper;
	modelEditHelper["startInputRotate"] = &ModelEditHelper::startInputRotate;
	modelEditHelper["startInputTranslate"] = &ModelEditHelper::startInputTranslate;


}