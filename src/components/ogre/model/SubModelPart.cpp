/*
 -------------------------------------------------------------------------------
 This source file is part of Cataclysmos
 For the latest info, see http://www.cataclysmos.org/

 Copyright (c) 2005 The Cataclysmos Team
 Copyright (C) 2005  Erik Ogenvik

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 -------------------------------------------------------------------------------
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SubModelPart.h"

#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"

#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreMaterialManager.h>
#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSceneManager.h>
#include <OgreInstanceManager.h>
#include <OgreInstancedEntity.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreInstanceBatch.h>
#include <boost/algorithm/string.hpp>
#include <utility>

namespace Ember {
namespace OgreView {
namespace Model {

SubModelPart::SubModelPart(std::string name, SubModel& subModel) :
		mName(std::move(name)), mSubModel(subModel) {
}

//no need to try to delete the Ogre::Subentities in the mSubEntities store, since Ogre will take care of this
SubModelPart::~SubModelPart() = default;

void SubModelPart::addSubEntity(SubModelPartEntity subModelPartEntity) {
	mSubEntities.emplace_back(std::move(subModelPartEntity));
}

bool SubModelPart::addSubEntity(Ogre::SubEntity* subentity, SubEntityDefinition definition, unsigned short subEntityIndex) {
	mSubEntities.emplace_back(SubModelPartEntity{subentity, std::move(definition), subEntityIndex});
	return true;
}

bool SubModelPart::removeSubEntity(const Ogre::SubEntity* subentity) {
	for (auto I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		if (I->SubEntity == subentity) {
			mSubEntities.erase(I);
			return true;
		}
	}
	return false;
}

const std::string& SubModelPart::getName() const {
	return mName;
}

void SubModelPart::show() {
	showSubEntities();
	if (mSubModel.mModel.mUseInstancing) {
		createInstancedEntities();
	}
}

void SubModelPart::showSubEntities() {
	for (auto& subModelPartEntity : mSubEntities) {
		std::string materialName;
		if (subModelPartEntity.Definition && !subModelPartEntity.Definition->materialName.empty()) {
			materialName = subModelPartEntity.Definition->materialName;
		} else {
			//if no material name is set in the ModelDefinition, use the default one from the mesh
			materialName = subModelPartEntity.SubEntity->getSubMesh()->getMaterialName();
		}

		if (mSubModel.mEntity.hasSkeleton()) {

			//We first need to check the number of bones to use
			const Ogre::VertexElement* blendWeightsData;
			if (subModelPartEntity.SubEntity->getSubMesh()->useSharedVertices) {
				blendWeightsData = subModelPartEntity.SubEntity->getSubMesh()->parent->sharedVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_BLEND_WEIGHTS);
			} else {
				blendWeightsData = subModelPartEntity.SubEntity->getSubMesh()->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_BLEND_WEIGHTS);
			}

			unsigned short numWeightsPerVertex = 0;
			if (blendWeightsData) {
				numWeightsPerVertex = Ogre::VertexElement::getTypeCount(blendWeightsData->getType());
			}

			//The number suffix denotes the number of bones to use.
			std::string skinningSuffix = "/Skinning/" + std::to_string(numWeightsPerVertex);
			//Check if we can use Hardware Skinning material.
			//This is done by checking if a material by the same name, but with the "/Skinning/*" suffix is available.
			//If not, we try to create such a material by cloning the original and replacing the vertex shader with
			//one with the same suffix (if available and supported).
			if (!boost::algorithm::ends_with(materialName, skinningSuffix)) {

				std::string newMaterialName = materialName + skinningSuffix;
				auto& materialMgr = Ogre::MaterialManager::getSingleton();
				if (!materialMgr.resourceExists(newMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
					//Material does not exist; lets create it
					auto material = materialMgr.getByName(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					if (material) {
						material->load();
						auto newMaterial = material->clone(newMaterialName);
						for (auto tech : newMaterial->getTechniques()) {
							if (!tech->getPasses().empty()) {
								auto pass = tech->getPass(0);
								if (pass->hasVertexProgram()) {
									std::string newVertexProgramName = pass->getVertexProgramName() + skinningSuffix;
									auto program = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(newVertexProgramName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
									if (program) {
										program->load();
										if (program->isSupported()) {
											pass->setVertexProgram(newVertexProgramName);
										}
									}
								}

								auto shadowCasterMat = tech->getShadowCasterMaterial();
								if (shadowCasterMat && !boost::algorithm::ends_with(shadowCasterMat->getName(), skinningSuffix)) {
									std::string skinningShadowCasterMatName = shadowCasterMat->getName() + skinningSuffix;
									auto shadowCasterMatSkinning = materialMgr.getByName(skinningShadowCasterMatName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
									if (!shadowCasterMatSkinning) {
										shadowCasterMat->load();
										shadowCasterMatSkinning = shadowCasterMat->clone(skinningShadowCasterMatName);
										for (auto* shadowCasterTech : shadowCasterMatSkinning->getTechniques()) {
											auto shadowCasterPass = shadowCasterTech->getPass(0);
											if (shadowCasterPass->hasVertexProgram()) {
												std::string vertexProgramName = shadowCasterPass->getVertexProgram()->getName() + skinningSuffix;
												auto program = Ogre::HighLevelGpuProgramManager::getSingleton().getByName(vertexProgramName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
												if (program) {
													program->load();
													if (program->isSupported()) {
														shadowCasterPass->setVertexProgram(vertexProgramName);
													}
												}
											}
										}
									}
									shadowCasterMatSkinning->load();
									tech->setShadowCasterMaterial(shadowCasterMatSkinning);
								}
							}
						}
					}
				}
				materialName = newMaterialName;
			}
		}


		if (materialName != subModelPartEntity.SubEntity->getMaterialName()) {
			//TODO: store the material ptr in the definition so we'll avoid a lookup in setMaterialName
			subModelPartEntity.SubEntity->setMaterialName(materialName);
		}
		subModelPartEntity.SubEntity->setVisible(true);
	}
}

bool SubModelPart::createInstancedEntities() {
	std::vector<std::pair<Ogre::InstanceManager*, std::string>> managersAndMaterials;

	//We would like to use the HW based technique, but as of 1.10.9 that causes corruption with the PSSM shadows.
	//Until that's fixed we instead use the Shader based technique, which performs worse.
	//The HW based technique uses the suffix "/Instanced/HW".
	static std::string instancedSuffix = "/Instanced/Shader";
	static Ogre::InstanceManager::InstancingTechnique instancedTechnique = Ogre::InstanceManager::ShaderBased;

	for (auto& entry : mSubEntities) {
		Ogre::SubEntity* subEntity = entry.SubEntity;
		Ogre::Entity* entity = subEntity->getParent();
		Ogre::SceneManager* sceneManager = entity->_getManager();
		std::string instanceName = entity->getMesh()->getName() + "/" + std::to_string(entry.subEntityIndex);


		std::string materialName;
		if (entry.Definition && !entry.Definition->materialName.empty()) {
			materialName = entry.Definition->materialName;
		} else {
			//if no material name is set in the ModelDefinition, use the default one from the mesh
			materialName = entry.SubEntity->getSubMesh()->getMaterialName();
		}

		std::string instancedMaterialName = materialName;

		//Check if the material is "instanced", i.e. has the suffix as specified in "instancedSuffix".
		//If not, we'll create a new material by cloning the original and replacing the vertex shader with
		//one with the correct suffix, if such one is available and supported.
		if (!boost::algorithm::ends_with(materialName, instancedSuffix)) {

			instancedMaterialName += instancedSuffix;
			auto& materialMgr = Ogre::MaterialManager::getSingleton();

			if (!materialMgr.resourceExists(instancedMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
				auto originalMaterial = materialMgr.getByName(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				if (originalMaterial) {
					originalMaterial->load();
					auto material = originalMaterial->clone(instancedMaterialName);
					material->load();
					for (auto* tech : material->getTechniques()) {
						auto pass = tech->getPass(0);
						if (pass->hasVertexProgram()) {
							std::string vertexProgramName = pass->getVertexProgram()->getName() + instancedSuffix;
							if (Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(vertexProgramName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
								pass->setVertexProgram(vertexProgramName);
							}
						}

						auto shadowCasterMat = tech->getShadowCasterMaterial();
						if (shadowCasterMat && !boost::algorithm::ends_with(shadowCasterMat->getName(), instancedSuffix)) {
							std::string instancedShadowCasterMatName = shadowCasterMat->getName() + instancedSuffix;
							auto shadowCasterMatInstanced = materialMgr.getByName(instancedShadowCasterMatName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
							if (!shadowCasterMatInstanced) {
								shadowCasterMat->load();
								shadowCasterMatInstanced = shadowCasterMat->clone(instancedShadowCasterMatName);
								for (auto* shadowCasterTech : shadowCasterMatInstanced->getTechniques()) {
									auto shadowCasterPass = shadowCasterTech->getPass(0);
									if (shadowCasterPass->hasVertexProgram()) {
										std::string vertexProgramName = shadowCasterPass->getVertexProgram()->getName() + instancedSuffix;
										if (Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(vertexProgramName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
											shadowCasterPass->setVertexProgram(vertexProgramName);
										}
									}
								}
							}
							shadowCasterMatInstanced->load();
							tech->setShadowCasterMaterial(shadowCasterMatInstanced);
						}
					}
				} else {
					S_LOG_WARNING("The material '" << materialName << "' used by a submesh of the mesh '" << entity->getMesh()->getName() << "' could not be found. The submesh will be hidden.");
					continue;
				}
			}
		}

		if (sceneManager->hasInstanceManager(instanceName)) {
			managersAndMaterials.emplace_back(std::make_pair(sceneManager->getInstanceManager(instanceName), instancedMaterialName));
		} else {
			auto bestTech = subEntity->getMaterial()->getBestTechnique();
			if (!bestTech->getPasses().empty() && bestTech->getPass(0)->hasVertexProgram()) {
				auto& meshName = entity->getMesh()->getName();

				try {
					Ogre::InstanceManager* instanceManager = sceneManager->createInstanceManager(instanceName,
																								 meshName,
																								 entity->getMesh()->getGroup(),
																								 instancedTechnique,
																								 50, Ogre::IM_USEALL, entry.subEntityIndex);
					instanceManager->setBatchesAsStaticAndUpdate(true);

					managersAndMaterials.emplace_back(std::make_pair(instanceManager, instancedMaterialName));
				} catch (const std::exception& e) {
					S_LOG_FAILURE("Could not create instanced versions of mesh " << meshName);
				}

			} else {
				//Could not make into instanced.
				S_LOG_WARNING("Could not create instanced version of subentity with index " << entry.subEntityIndex << " of entity " << entity->getName());
			}
		}
	}

	for (auto& entry : managersAndMaterials) {
		try {
			auto instancedEntity = entry.first->createInstancedEntity(entry.second);
			if (instancedEntity) {
				mInstancedEntities.push_back(instancedEntity);
				mSubModel.mModel.addMovable(instancedEntity);
				::Ember::OgreView::Model::Model::sInstancedEntities[entry.first->getSceneManager()][instancedEntity] = &mSubModel.mModel;
			} else {
				S_LOG_FAILURE("Could not create instanced entity " << entry.first->getName());
			}
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Could not create instanced entity " << entry.first->getName() << ex);
		}
	}
	return true;
}


void SubModelPart::hide() {
	if (!mInstancedEntities.empty()) {
		for (auto& item : mInstancedEntities) {
			item->setVisible(false);
		}
	} else {
		for (auto& item : mSubEntities) {
			item.SubEntity->setVisible(false);
		}
	}

}

const std::vector<SubModelPartEntity>& SubModelPart::getSubentities() const {
	return mSubEntities;
}

void SubModelPart::destroy() {
	for (auto& item : mInstancedEntities) {
		//There's a bug where the InstancedEntity doesn't contain a pointer to it's scene manager; we need to go through the InstanceBatch instead
		::Ember::OgreView::Model::Model::sInstancedEntities[item->_getOwner()->_getManager()].erase(item);
		mSubModel.mModel.removeMovable(item);
		mSubModel.mEntity._getManager()->destroyInstancedEntity(item);
	}
}

}
}
}
