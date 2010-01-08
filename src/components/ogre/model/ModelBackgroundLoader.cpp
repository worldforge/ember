//
// C++ Implementation: ModelBackgroundLoader
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ModelBackgroundLoader.h"
#include "Model.h"
#include "framework/LoggingInstance.h"

#include <OgreResourceBackgroundQueue.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreResourceGroupManager.h>

namespace EmberOgre
{

namespace Model
{

ModelBackgroundLoaderListener::ModelBackgroundLoaderListener(ModelBackgroundLoader& loader) :
	mLoader(&loader)
{

}
void ModelBackgroundLoaderListener::operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result)
{
	if (mLoader) {
		mLoader->operationCompleted(ticket, result, this);
	}
	delete this;
}

void ModelBackgroundLoaderListener::detachFromLoader()
{
	mLoader = 0;
}

ModelBackgroundLoader::ModelBackgroundLoader(Model& model) :
	mModel(model), mState(LS_UNINITIALIZED)
{
}

ModelBackgroundLoader::~ModelBackgroundLoader()
{
	for (ListenerStore::iterator I = mListeners.begin(); I != mListeners.end(); ++I) {
		(*I)->detachFromLoader();
	}
}

bool ModelBackgroundLoader::poll()
{
#if OGRE_THREAD_SUPPORT
	if (mState == LS_UNINITIALIZED) {
		///Start to load the meshes
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) {
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr> (Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			if (meshPtr.isNull() || !meshPtr->isPrepared()) {
				try {
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().prepare(Ogre::MeshManager::getSingleton().getResourceType(), (*I_subModels)->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
					if (ticket) {
						addTicket(ticket);
					}
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Could not load the mesh " << (*I_subModels)->getMeshName() << " when loading model " << mModel.getName() << "." << ex);
					continue;
				}
			}
		}
		mState = LS_MESH_PREPARING;
		return poll();
	} else if (mState == LS_MESH_PREPARING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MESH_PREPARED;
			return poll();
		}
	} else if (mState == LS_MESH_PREPARED) {
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) {
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr> (Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			if (!meshPtr.isNull()) {
				if (!meshPtr->isLoaded()) {
#if OGRE_THREAD_SUPPORT == 1
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MeshManager::getSingleton().getResourceType(), meshPtr->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
					if (ticket) {
						//						meshPtr->setBackgroundLoaded(true);
						addTicket(ticket);
					}
#else
					try {
						meshPtr->load();
					} catch (const std::exception& ex) {
						S_LOG_FAILURE("Could not load the mesh " << meshPtr->getName() << " when loading model " << mModel.getName() << "." << ex);
						continue;
					}
#endif
				}
			}
		}
		mState = LS_MESH_LOADING;
		return poll();
	} else if (mState == LS_MESH_LOADING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MESH_LOADED;
			return poll();
		}
	} else if (mState == LS_MESH_LOADED) {

		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) {
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr> (Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			if (!meshPtr.isNull()) {
				if (!meshPtr->isLoaded()) {
					Ogre::Mesh::SubMeshIterator subMeshI = meshPtr->getSubMeshIterator();
					while (subMeshI.hasMoreElements()) {
						Ogre::SubMesh* submesh(subMeshI.getNext());
						Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(submesh->getMaterialName()));
						if (materialPtr.isNull() || !materialPtr->isPrepared()) {
							Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().prepare(Ogre::MaterialManager::getSingleton().getResourceType(), submesh->getMaterialName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
							if (ticket) {
								addTicket(ticket);
							}
						}
					}
				}
			}
			for (PartDefinitionsStore::const_iterator I_parts = (*I_subModels)->getPartDefinitions().begin(); I_parts != (*I_subModels)->getPartDefinitions().end(); ++I_parts) {
				if ((*I_parts)->getSubEntityDefinitions().size() > 0) {
					for (SubEntityDefinitionsStore::const_iterator I_subEntities = (*I_parts)->getSubEntityDefinitions().begin(); I_subEntities != (*I_parts)->getSubEntityDefinitions().end(); ++I_subEntities) {
						Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName((*I_subEntities)->getMaterialName()));
						if (materialPtr.isNull() || !materialPtr->isPrepared()) {
							Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().prepare(Ogre::MaterialManager::getSingleton().getResourceType(), (*I_subEntities)->getMaterialName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
							if (ticket) {
								addTicket(ticket);
							}
						}
					}
				}
			}
		}

		mState = LS_MATERIAL_PREPARING;
		return poll();
	} else if (mState == LS_MATERIAL_PREPARING) {
		if (areAllTicketsProcessed()) {
			mState = LS_MATERIAL_PREPARED;
			return poll();
		}
	} else if (mState == LS_MATERIAL_PREPARED) {
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) {
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr> (Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			Ogre::Mesh::SubMeshIterator subMeshI = meshPtr->getSubMeshIterator();
			while (subMeshI.hasMoreElements()) {
				Ogre::SubMesh* submesh(subMeshI.getNext());
				Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(submesh->getMaterialName()));
				if (!materialPtr.isNull() && !materialPtr->isLoaded()) {

#if OGRE_THREAD_SUPPORT == 1
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MaterialManager::getSingleton().getResourceType(), materialPtr->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
					if (ticket) {
						//						materialPtr->setBackgroundLoaded(true);
						addTicket(ticket);
					}
#else
					materialPtr->load();
#endif
				}
			}
#if OGRE_THREAD_SUPPORT == 1
			for (PartDefinitionsStore::const_iterator I_parts = (*I_subModels)->getPartDefinitions().begin(); I_parts != (*I_subModels)->getPartDefinitions().end(); ++I_parts) {
				if ((*I_parts)->getSubEntityDefinitions().size() > 0) {
					for (SubEntityDefinitionsStore::const_iterator I_subEntities = (*I_parts)->getSubEntityDefinitions().begin(); I_subEntities != (*I_parts)->getSubEntityDefinitions().end(); ++I_subEntities) {
						Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName((*I_subEntities)->getMaterialName()));
						if (!materialPtr.isNull() && !materialPtr->isLoaded()) {
							Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MaterialManager::getSingleton().getResourceType(), materialPtr->getName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, 0, 0, createListener());
							if (ticket) {
								addTicket(ticket);
							}
						}
					}
				}
			}
#endif
		}

		mState = LS_MATERIAL_LOADING;
		return poll();
	} else if (mState == LS_MATERIAL_LOADING) {
		if (areAllTicketsProcessed()) {
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

void ModelBackgroundLoader::operationCompleted(Ogre::BackgroundProcessTicket ticket, const Ogre::BackgroundProcessResult& result, ModelBackgroundLoaderListener* listener)
{
	ListenerStore::iterator I_listener = std::find(mListeners.begin(), mListeners.end(), listener);
	if (I_listener != mListeners.end()) {
		mListeners.erase(I_listener);
	}

	TicketStore::iterator I = std::find(mTickets.begin(), mTickets.end(), ticket);
	if (I != mTickets.end()) {
		mTickets.erase(I);
		poll();
	}
}

bool ModelBackgroundLoader::areAllTicketsProcessed()
{
	return mTickets.size() == 0;
}

const ModelBackgroundLoader::LoadingState ModelBackgroundLoader::getState() const
{
	return mState;
}

void ModelBackgroundLoader::reloadModel()
{
	mModel.reload();
}

void ModelBackgroundLoader::addTicket(Ogre::BackgroundProcessTicket ticket)
{
	mTickets.push_back(ticket);
}

ModelBackgroundLoaderListener* ModelBackgroundLoader::createListener()
{
	ModelBackgroundLoaderListener* listener = new ModelBackgroundLoaderListener(*this);
	mListeners.push_back(listener);
	return listener;
}

}

}
