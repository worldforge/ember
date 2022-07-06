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
void registerLua<GpuProgram>(sol::table& space) {
	auto gpuProgram = space.new_usertype<GpuProgram>("GpuProgram", sol::no_constructor,
													 sol::base_classes, sol::bases<Resource>()
	);
	gpuProgram["getSource"] = &GpuProgram::getSource;

	auto gpuProgramManager = space.new_usertype<GpuProgramManager>("GpuProgramManager", sol::no_constructor,
																   sol::base_classes, sol::bases<ResourceManager>()
	);
	gpuProgramManager["getSingleton"] = &GpuProgramManager::getSingleton;
//										 "getByName", [](GpuProgramManager* self, const String& name, const String& group) { return std::shared_ptr(self->getByName(name, group)); },
	gpuProgramManager["getByName"] = sol::resolve<SharedPtr<GpuProgram>(const String&, const String&) const>(&GpuProgramManager::getByName);


}