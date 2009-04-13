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

namespace EmberOgre {

namespace Model {

ModelBackgroundLoader::ModelBackgroundLoader(Model& model)
: mModel(model), mState(LS_UNINITIALIZED)
{
}


ModelBackgroundLoader::~ModelBackgroundLoader()
{
}



bool ModelBackgroundLoader::poll()
{

	if (mState == LS_UNINITIALIZED) {
		///Start to load the meshes
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) 
		{
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr>(Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			if (meshPtr.isNull() || !meshPtr->isPrepared()) {
				try {
					Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MeshManager::getSingleton().getResourceType(), (*I_subModels)->getMeshName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					if (ticket) {
						addTicket(ticket);
					}
				} catch (const Ogre::Exception& ex) {
					S_LOG_FAILURE("Could not load the mesh " << (*I_subModels)->getMeshName() << " when loading model " << mModel.getName());
					continue;
				}
			}
		}
		mState = LS_MESH_PREPARING;
		return poll();
	} else if (mState == LS_MESH_PREPARING) {
		if (mTickets.size() > 0) {
			for (TicketStore::iterator I = mTickets.begin(); I != mTickets.end(); ) {
				TicketStore::iterator I_copy = I;
				++I;
				if (Ogre::ResourceBackgroundQueue::getSingleton().isProcessComplete(*I_copy)) {
					mTickets.erase(I_copy);
				}
			}
		}
		if (mTickets.size() == 0) {
			mState = LS_MESH_PREPARED;
			return poll();
		}
	} else if (mState == LS_MESH_PREPARED) {
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) 
		{
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr>(Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			if (!meshPtr.isNull()) {
				if (!meshPtr->isLoaded()) {
					try {
						meshPtr->load();
					} catch (const Ogre::Exception& ex) {
						S_LOG_FAILURE("Could not load the mesh " << meshPtr->getName() << " when loading model " << mModel.getName());
						continue;
					}
				}
				Ogre::Mesh::SubMeshIterator subMeshI = meshPtr->getSubMeshIterator();
				while (subMeshI.hasMoreElements()) {
					Ogre::SubMesh* submesh(subMeshI.getNext());
					Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(submesh->getMaterialName()));
					if (materialPtr.isNull() || !materialPtr->isPrepared()) {
						Ogre::BackgroundProcessTicket ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MaterialManager::getSingleton().getResourceType(), submesh->getMaterialName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
						if (ticket) {
							addTicket(ticket);
						}
					}
				}
			}
		}
		mState = LS_MATERIAL_PREPARING;
		return poll();
	} else if (mState == LS_MATERIAL_PREPARING) {
		if (mTickets.size() > 0) {
			for (TicketStore::iterator I = mTickets.begin(); I != mTickets.end(); ) {
				TicketStore::iterator I_copy = I;
				++I;
				if (Ogre::ResourceBackgroundQueue::getSingleton().isProcessComplete(*I_copy)) {
					mTickets.erase(I_copy);
				}
			}
		}
		if (mTickets.size() == 0) {
			mState = LS_MATERIAL_PREPARED;
			return poll();
		}
	} else if (mState == LS_MATERIAL_PREPARED) {
		for (SubModelDefinitionsStore::const_iterator I_subModels = mModel.getDefinition()->getSubModelDefinitions().begin(); I_subModels != mModel.getDefinition()->getSubModelDefinitions().end(); ++I_subModels) 
		{
			Ogre::MeshPtr meshPtr = static_cast<Ogre::MeshPtr>(Ogre::MeshManager::getSingleton().getByName((*I_subModels)->getMeshName()));
			Ogre::Mesh::SubMeshIterator subMeshI = meshPtr->getSubMeshIterator();
			while (subMeshI.hasMoreElements()) {
				Ogre::SubMesh* submesh(subMeshI.getNext());
				Ogre::MaterialPtr materialPtr = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(submesh->getMaterialName()));
				if (!materialPtr.isNull() && !materialPtr->isLoaded()) {
					materialPtr->load();
				}
			}
		}
		mState = LS_DONE;
		return true;
	} else {
		return true;
	}
	return false;
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

}

}
