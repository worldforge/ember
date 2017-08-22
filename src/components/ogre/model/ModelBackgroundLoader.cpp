//
// C++ Implementation: ModelBackgroundLoader
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelBackgroundLoader.h"
#include "Model.h"
#include "framework/TimeFrame.h"
#include "framework/LoggingInstance.h"

#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreTextureManager.h>

#include <OgreTechnique.h>

#include <Eris/EventService.h>
#include <framework/MainLoopController.h>
#include <Ogre.h>
#include <framework/TimedLog.h>

namespace Ember {
namespace OgreView {

namespace Model {

ModelBackgroundLoaderListener::ModelBackgroundLoaderListener(ModelBackgroundLoader& loader) :
		mLoader(loader) {

}

void ModelBackgroundLoaderListener::operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result) {
	mLoader.operationCompleted(ticket, result);
}

ModelBackgroundLoader::ModelBackgroundLoader(ModelDefinition& modelDefinition) :
		mModelDefinition(modelDefinition),
		mState(LS_UNINITIALIZED),
		mListener(*this),
		mSubModelLoadingIndex(0) {
}

ModelBackgroundLoader::~ModelBackgroundLoader() {
	for (auto& ticket : mTickets) {
		Ogre::ResourceBackgroundQueue::getSingleton().abortRequest(ticket);
	}
}

bool ModelBackgroundLoader::poll() {
	bool result = performLoading();
	if (result) {
		mModelDefinition.notifyAssetsLoaded();
		return true;
	}

	if (areAllTicketsProcessed()) {
		MainLoopController::getSingleton().getEventService().runOnMainThread([this]() {
			this->poll();
		}, mIsActive);
	}
	return false;

}

void ModelBackgroundLoader::prepareMaterialInBackground(const std::string& materialName) {
	if (!materialName.empty()) {
		Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
		if (materialPtr) {
			mMaterialsToLoad.insert(materialPtr);
			if (!materialPtr->isPrepared() && !materialPtr->isLoading() && !materialPtr->isLoaded()) {
				Ogre::ResourceBackgroundQueue& queue = Ogre::ResourceBackgroundQueue::getSingleton();
				Ogre::BackgroundProcessTicket ticket = queue.prepare(Ogre::MaterialManager::getSingleton().getResourceType(), materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, &mListener);
				if (ticket) {
					addTicket(ticket);
				}
			}
		}
	}
}


bool ModelBackgroundLoader::performLoading() {
#if OGRE_THREAD_SUPPORT
//	TimedLog log("performLoading " + mModelDefinition.getName() + " state: " + std::to_string(mState));
	if (mState == LS_UNINITIALIZED) {
		//Start to load the meshes
		for (auto subModel : mModelDefinition.getSubModelDefinitions()) {
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr>(Ogre::MeshManager::getSingleton().getByName(subModel->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
			if (!meshPtr || (!meshPtr->isPrepared() && !meshPtr->isLoading() && !meshPtr->isLoaded())) {
				try {
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().prepare(Ogre::MeshManager::getSingleton().getResourceType(), subModel->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, nullptr, nullptr, &mListener);
					if (ticket) {
						addTicket(ticket);
					}
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Could not load the mesh " << subModel->getMeshName() << " when loading model " << mModelDefinition.getName() << "." << ex);
					continue;
				}
			}
		}
		mState = LS_MESH_PREPARING;
		return false;
	} else if (mState == LS_MESH_PREPARING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MESH_PREPARED;
			return performLoading();
		}
	} else if (mState == LS_MESH_PREPARED) {
		auto& submodelDefinitions = mModelDefinition.getSubModelDefinitions();
		while (mSubModelLoadingIndex < submodelDefinitions.size()) {
			auto submodelDef = submodelDefinitions.at(mSubModelLoadingIndex);
			mSubModelLoadingIndex++;
			Ogre::MeshPtr meshPtr = Ogre::static_pointer_cast<Ogre::Mesh>(Ogre::MeshManager::getSingleton().getByName(submodelDef->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
			if (meshPtr) {
				if (!meshPtr->isLoaded()) {
#if OGRE_THREAD_SUPPORT == 1
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MeshManager::getSingleton().getResourceType(), meshPtr->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, mListener);
					if (ticket) {
						//						meshPtr->setBackgroundLoaded(true);
						addTicket(ticket);
					}
#else
					try {
						meshPtr->load();
						S_LOG_VERBOSE("Loaded mesh in main thread: " << meshPtr->getName() << " Memory used: " << (meshPtr->getSize() / 1000000.f) << " Mb");
					} catch (const std::exception& ex) {
						S_LOG_FAILURE("Could not load the mesh " << meshPtr->getName() << " when loading model " << mModelDefinition.getName() << "." << ex);
					}
					return false;
#endif
				}
			}
		}
		mState = LS_MESH_LOADING;
		return false;
	} else if (mState == LS_MESH_LOADING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MESH_LOADED;
			return performLoading();
		}
	} else if (mState == LS_MESH_LOADED) {

		for (auto& submodelDef : mModelDefinition.getSubModelDefinitions()) {

			Ogre::MeshPtr meshPtr = Ogre::static_pointer_cast<Ogre::Mesh>(Ogre::MeshManager::getSingleton().getByName(submodelDef->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
			if (meshPtr) {
				if (meshPtr->isLoaded()) {
					for (auto submesh : meshPtr->getSubMeshes()) {
						prepareMaterialInBackground(submesh->getMaterialName());
					}
				}
			}
			for (auto& partDef : submodelDef->getPartDefinitions()) {
				if (!partDef->getSubEntityDefinitions().empty()) {
					for (auto& subEntityDef : partDef->getSubEntityDefinitions()) {
						prepareMaterialInBackground(subEntityDef->getMaterialName());
					}
				}
			}
		}
		mState = LS_PARTICLE_SYSTEM_PREPARING;
		return false;
	} else if (mState == LS_PARTICLE_SYSTEM_PREPARING) {
		for (auto& particleSystemDef : mModelDefinition.mParticleSystems) {
			auto particleSystemTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate(particleSystemDef.Script);
			if (particleSystemTemplate) {
				prepareMaterialInBackground(particleSystemTemplate->getMaterialName());
			}
		}
		mState = LS_MATERIAL_PREPARING;
		return false;
	} else if (mState == LS_MATERIAL_PREPARING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MATERIAL_PREPARED;
			return performLoading();
		}
		return false;
	} else if (mState == LS_MATERIAL_PREPARED) {
		for (auto& materialPtr : mMaterialsToLoad) {
			if (materialPtr) {

#if OGRE_THREAD_SUPPORT == 1
				Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MaterialManager::getSingleton().getResourceType(), materialPtr->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, mListener);
				if (ticket) {
					//						materialPtr->setBackgroundLoaded(true);
					addTicket(ticket);
				}
#else
				for (auto* tech : materialPtr->getSupportedTechniques()) {
					for (auto* pass : tech->getPasses()) {
						for (auto* tus : pass->getTextureUnitStates()) {
							unsigned int frames = tus->getNumFrames();
							for (unsigned int i = 0; i < frames; ++i) {
								const auto& textureName = tus->getFrameTextureName(i);
								mTexturesToLoad.insert(textureName);
								Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().prepare(Ogre::TextureManager::getSingleton().getResourceType(), textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, &mListener);
								if (ticket) {
									addTicket(ticket);
								}
							}
						}
					}
				}
#endif
			}
		}
		mState = LS_TEXTURE_PREPARING;
		return false;
	} else if (mState == LS_TEXTURE_PREPARING) {
		if (areAllTicketsProcessed()) {
			mState = LS_TEXTURE_PREPARED;
			return performLoading();
		}
	} else if (mState == LS_TEXTURE_PREPARED) {
		if (!mTexturesToLoad.empty()) {
			auto I = mTexturesToLoad.begin();
			std::string textureName = *I;
			mTexturesToLoad.erase(I);
			auto texture = Ogre::TextureManager::getSingleton().getByName(textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			if (texture && !texture->isLoaded()) {
				try {
					if (!texture->isPrepared()) {
						S_LOG_WARNING("Texture was not prepared: "  << texture->getName());
					}
					texture->load();
					S_LOG_VERBOSE("Loaded texture in main thread: " << texture->getName() << " Memory used: " << (texture->getSize() / 1000000.f) << " Mb");
				} catch (const std::exception& e) {
					S_LOG_WARNING("Error when loading texture " << texture->getName() << e);
				}
				return false;
			}
		}
		mState = LS_MATERIAL_LOADING;
		return false;
	} else if (mState == LS_MATERIAL_LOADING) {
		if (areAllTicketsProcessed()) {
			if (!mMaterialsToLoad.empty()) {
				auto I = mMaterialsToLoad.begin();
				Ogre::MaterialPtr material = *I;
				mMaterialsToLoad.erase(I);
				if (!material->isLoaded()) {
					try {
						material->load();
						S_LOG_VERBOSE("Loaded material in main thread: " << material->getName());
					} catch (const std::exception& e) {
						S_LOG_WARNING("Error when loading material " << material->getName() << e);
					}
					return false;
				}
			}

			mState = LS_DONE;
			return true;
		}
	} else {
		return true;
	}
	return false;
#else
	//If there's no threading support, just return here.
	return true;
#endif
}

void ModelBackgroundLoader::operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result) {
	auto I = mTickets.find(ticket);
	if (I != mTickets.end()) {
		mTickets.erase(I);
		if (mTickets.empty()) {
			poll();
		}
	}
}

bool ModelBackgroundLoader::areAllTicketsProcessed() {
	return mTickets.empty();
}

ModelBackgroundLoader::LoadingState ModelBackgroundLoader::getState() const {
	return mState;
}

void ModelBackgroundLoader::addTicket(Ogre::BackgroundProcessTicket ticket) {
	mTickets.insert(ticket);
}

}

}
}
