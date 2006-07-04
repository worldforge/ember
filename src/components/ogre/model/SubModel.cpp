/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "SubModel.h"
#include "Model.h"

#include "SubModelPart.h"
#include "ModelDefinitionManager.h"

namespace EmberOgre {
namespace Model {

SubModel::SubModel(Ogre::Entity* entity) :
mEntity(entity)
{
	//begin by hiding all subentities
	unsigned int numSubEntities = mEntity->getNumSubEntities();
	for (unsigned int i = 0;i < numSubEntities; ++i) {
		mEntity->getSubEntity(i)->setVisible(false);
	}
	
}
SubModel::~SubModel()
{
	for (SubModelPartMap::iterator I = mSubModelParts.begin(); I != mSubModelParts.end(); ++I) {
		delete I->second;
	}
	Ogre::SceneManager* sceneManager = ModelDefinitionManager::instance().getSceneManager();
	sceneManager->destroyEntity(mEntity);
	
}

std::map<std::string, SubModelPart*>* SubModel::getSubModelPartMap()
{
	return &mSubModelParts;
	
}

SubModelPart* SubModel::createSubModelPart(const std::string& name)
{
	SubModelPart* part = new SubModelPart(name);
	mSubModelParts.insert(SubModelPartMap::value_type(name, part));
	return part;
	
}

// void SubModel::createSubModelParts(SubModelPartMapping* submodelPartMapping)
// {
// 	SubModelPartMapping::const_iterator I = submodelPartMapping->begin();
// 	SubModelPartMapping::const_iterator I_end = submodelPartMapping->end();
// 	
// 	for (;I != I_end; ++I) {
// 		std::string partname = I->first;
// 		SubModelPart* part = new SubModelPart(partname);
// 		std::set<std::string>::const_iterator J = I->second.begin();
// 		std::set<std::string>::const_iterator J_end = I->second.end();
// 		if (J == J_end) {
// 			//if the set is empty add all subentities
// 			unsigned int numSubEntities = mEntity->getNumSubEntities();
// 			for (unsigned int i = 0;i < numSubEntities; ++i) {
// 				part->addSubEntity(mEntity->getSubEntity(i));
// 			}
// 		} else {
// 			for (;J != J_end; ++J) {
// 				part->addSubEntity(mEntity->getSubEntity(*J));
// 			}
// 		}
// 		mSubModelParts.insert(SubModelPartMap::value_type(partname, part));
// 		
// 	}
// 	
// 	
// }

Ogre::Entity* SubModel::getEntity() const
{
	return mEntity;	
}



/*
bool SubModel::addEntity(Ogre::Entity* entity)
{
	mEntities.insert(entity);
	return true;	
}

bool SubModel::removeEntity(Ogre::Entity* entity)
{
	mEntities.erase(entity);
	return true;
}
*/

}
}
