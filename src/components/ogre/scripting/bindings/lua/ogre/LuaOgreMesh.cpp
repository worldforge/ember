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
void registerLua<Mesh>(sol::table& space) {
	auto mesh = space.new_usertype<Mesh>("Mesh", sol::no_constructor,
										 sol::base_classes, sol::bases<Resource>()
	);
	mesh["getNumSubMeshes"] = &Mesh::getNumSubMeshes;
	mesh["getSubMesh"] = sol::resolve<
			SubMesh*(size_t) const>(&Mesh::getSubMesh);
	mesh["getBounds"] = &Mesh::getBounds;
	mesh["getBoundingSphereRadius"] = &Mesh::getBoundingSphereRadius;
	mesh["setSkeletonName"] = &Mesh::setSkeletonName;
	mesh["hasSkeleton"] = &Mesh::hasSkeleton;
	mesh["getSkeleton"] = [](Mesh* self) { return self->getSkeleton(); };
	mesh["getSkeletonName"] = &Mesh::getSkeletonName;
	mesh["removeLodLevels"] = &Mesh::removeLodLevels;

	auto subMesh = space.new_usertype<SubMesh>("SubMesh");
	subMesh["getMaterialName"] = &SubMesh::getMaterialName;
	subMesh["setMaterialName"] = [](SubMesh* self, const String& matName) { self->setMaterialName(matName); };

	auto meshManager = space.new_usertype<MeshManager>("MeshManager", sol::no_constructor,
													   sol::base_classes, sol::bases<ResourceManager>()
	);
	meshManager["getSingleton"] = &MeshManager::getSingleton,
			meshManager["load"] = [](MeshManager* self, const String& name, const String& group) { return self->load(name, group); };
	// "getByName"]=[](MeshManager* self, const String& name, const String& group) { return std::shared_ptr(self->getByName(name, group)); },
	meshManager["getByName"] = &MeshManager::getByName;
	meshManager["create"] = [](MeshManager* self, const String& name, const String& group) { return self->create(name, group); };

}