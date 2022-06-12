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

#include "BindingsOgre.h"
#include <Ogre.h>
#include <OgreMesh.h>
#include <OgreLodConfig.h>

using namespace Ogre;


namespace sol {
template<typename T>
struct unique_usertype_traits<SharedPtr < T>> {
typedef T type;
typedef SharedPtr <T> actual_type;
static const bool value = true;

static bool is_null(const actual_type& ptr) {
	return ptr == nullptr;
}

static type* get(const actual_type& ptr) {
	return ptr.get();
}
};
}

void registerBindingsOgre(sol::state_view& lua) {
	auto Ogre = lua["Ogre"].get_or_create<sol::table>();

	Ogre.new_usertype<AxisAlignedBox>("AxisAlignedBox",
									  "getMinimum", sol::resolve<const Vector3&() const>(&AxisAlignedBox::getMinimum),
									  "getMaximum", sol::resolve<const Vector3&() const>(&AxisAlignedBox::getMaximum)
	);
	Ogre.new_usertype<ColourValue>("ColourValue");
	auto entity = Ogre.new_usertype<Entity>("Entity");
//							  "getMesh", [](Entity* self) { return std::shared_ptr(self->getMesh()); },
	entity["getMesh"] = &Entity::getMesh;
	entity["setMaterialName"] = &Entity::setMaterialName;

	Ogre.new_usertype<GpuProgram>("GpuProgram",
								  "getSource", &GpuProgram::getSource,
								  sol::base_classes, sol::bases<Resource>()
	);
	Ogre.new_usertype<GpuProgramManager>("GpuProgramManager",
										 "getSingleton", &GpuProgramManager::getSingleton,
//										 "getByName", [](GpuProgramManager* self, const String& name, const String& group) { return std::shared_ptr(self->getByName(name, group)); },
										 "getByName", sol::resolve<SharedPtr<GpuProgram>(const String&, const String&) const>(&GpuProgramManager::getByName),
										 sol::base_classes, sol::bases<ResourceManager>()
	);

	auto lodLevel = lua.create_table();
	lodLevel["VRM_PROPORTIONAL"] = LodLevel::VRM_PROPORTIONAL;
	lodLevel["VRM_CONSTANT"] = LodLevel::VRM_CONSTANT;
	lodLevel["VRM_COLLAPSE_COST"] = LodLevel::VRM_COLLAPSE_COST;

	Ogre["LodLevel"] = lodLevel;

	Ogre.new_usertype<LodStrategy>("LodStrategy",
								   "transformUserValue", &LodStrategy::transformUserValue
	);

	Ogre.new_usertype<Material>("Material",
								sol::base_classes, sol::bases<Resource>()
	);
	Ogre.new_usertype<MaterialManager>("MaterialManager",
									   "getSingleton", &MaterialManager::getSingleton,
									   "getByName", [](MaterialManager* self, const String& name, const String& group) { return self->getByName(name, group); },
									   sol::base_classes, sol::bases<ResourceManager>()
	);

	Ogre.new_usertype<Radian>("Radian",
							  sol::constructors<Radian(float), Radian(const Degree&)>()
	);
	Ogre.new_usertype<Degree>("Degree",
							  sol::constructors<Degree(float), Degree(const Radian&)>()
	);
	Ogre.new_usertype<Mesh>("Mesh",
							"getNumSubMeshes", &Mesh::getNumSubMeshes,
							"getSubMesh", sol::resolve<SubMesh*(size_t) const>(&Mesh::getSubMesh),
							"getBounds", &Mesh::getBounds,
							"getBoundingSphereRadius", &Mesh::getBoundingSphereRadius,
							"setSkeletonName", &Mesh::setSkeletonName,
							"hasSkeleton", &Mesh::hasSkeleton,
							"getSkeleton", [](Mesh* self) { return self->getSkeleton(); },
							"getSkeletonName", &Mesh::getSkeletonName,
							"removeLodLevels", &Mesh::removeLodLevels,
							sol::base_classes, sol::bases<Resource>()
	);
	Ogre.new_usertype<MeshManager>("MeshManager",
								   "getSingleton", &MeshManager::getSingleton,
								   "load", [](MeshManager* self, const String& name, const String& group) { return self->load(name, group); },
			// "getByName", [](MeshManager* self, const String& name, const String& group) { return std::shared_ptr(self->getByName(name, group)); },
								   "getByName", &MeshManager::getByName,
								   "create", [](MeshManager* self, const String& name, const String& group) { return self->create(name, group); },
								   sol::base_classes, sol::bases<ResourceManager>()
	);

	auto sceneNode = Ogre.new_usertype<SceneNode>("SceneNode");
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


	Ogre.new_usertype<Quaternion>("Quaternion",
								  "IDENTITY", sol::var(&Quaternion::IDENTITY),
								  "ZERO", sol::var(&Quaternion::ZERO)
	);

	auto RenderTarget = Ogre["RenderTarget"].get_or_create<sol::table>();
	RenderTarget.new_usertype<RenderTarget::FrameStats>("RenderTarget::FrameStats",
														"lastFPS", &RenderTarget::FrameStats::lastFPS,
														"avgFPS", &RenderTarget::FrameStats::avgFPS,
														"bestFPS", &RenderTarget::FrameStats::bestFPS,
														"worstFPS", &RenderTarget::FrameStats::worstFPS,
														"bestFrameTime", &RenderTarget::FrameStats::bestFrameTime,
														"worstFrameTime", &RenderTarget::FrameStats::worstFrameTime,
														"triangleCount", &RenderTarget::FrameStats::triangleCount,
														"batchCount", &RenderTarget::FrameStats::batchCount
	);

	Ogre.new_usertype<Resource>("Resource",
								"load", [](Resource* self) { self->load(); },
								"reload", [](Resource* self) { self->reload(); },
								"isReloadable", &Resource::isReloadable,
								"getName", &Resource::getName,
								"getGroup", &Resource::getGroup,
								"getOrigin", &Resource::getOrigin
	);

	auto resourceGroupManager = Ogre.new_usertype<ResourceGroupManager>("ResourceGroupManager");
	resourceGroupManager["DEFAULT_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME.c_str());
	resourceGroupManager["INTERNAL_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME.c_str());
	resourceGroupManager["AUTODETECT_RESOURCE_GROUP_NAME"] = sol::var(ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME.c_str());


	Ogre.new_usertype<ResourceManager>("ResourceManager",
									   "getResourceByName", [](ResourceManager* self, const std::string& name, const std::string& group) { return self->getResourceByName(name, group); }
	);
	Ogre.new_usertype<SceneManager>("SceneManager",
									"getRootSceneNode", &SceneManager::getRootSceneNode
	);
	auto subEntity = Ogre.new_usertype<SubEntity>("SubEntity");
	subEntity["getMaterialName"] = &SubEntity::getMaterialName;
	subEntity["setMaterialName"] = &SubEntity::setMaterialName;
	subEntity["setVisible"] = &SubEntity::setVisible;
	subEntity["isVisible"] = &SubEntity::isVisible;
	subEntity["getMaterial"] = &SubEntity::getMaterial;

	auto subMesh = Ogre.new_usertype<SubMesh>("SubMesh");
	subMesh["getMaterialName"] = &SubMesh::getMaterialName;
	subMesh["setMaterialName"] = [](SubMesh* self, const String& matName) { self->setMaterialName(matName); };


	Ogre.new_usertype<Texture>("Texture",
							   sol::base_classes, sol::bases<Resource>()
	);

	Ogre.new_usertype<TextureManager>("TextureManager",
									  "getSingleton", &TextureManager::getSingleton,
									  "load", [](TextureManager* self, const String& name, const String& group) { return self->load(name, group); },
									  "getByName", [](TextureManager* self, const String& name, const String& group) { return self->getByName(name, group); },
									  "create", [](TextureManager* self, const String& name, const String& group) { return self->create(name, group); },
									  sol::base_classes, sol::bases<ResourceManager>()
	);

	auto vector3 = Ogre.new_usertype<Vector3>("Vector3",
											  sol::constructors<Vector3(Vector3), Vector3(Real, Real, Real)>()
	);
	vector3["ZERO"] = sol::var(&Vector3::ZERO);
	vector3["x"] = &Vector3::x;
	vector3["y"] = &Vector3::y;
	vector3["z"] = &Vector3::z;

	lua["Ogre"] = Ogre;
}
