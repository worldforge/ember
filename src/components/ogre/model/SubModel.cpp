/*
    Copyright (C) 2004  Erik Ogenvik

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "SubModel.h"
#include "Model.h"

#include "SubModelPart.h"
#include "ModelDefinitionManager.h"
#include <OgreSceneManager.h>
#include <OgreSubEntity.h>

namespace Ember {
namespace OgreView {
namespace Model {

SubModel::SubModel(Ogre::Entity& entity, Model& model) :
		mEntity(entity), mModel(model) {
	//begin by hiding all subentities
	size_t numSubEntities = mEntity.getNumSubEntities();
	for (unsigned int i = 0; i < numSubEntities; ++i) {
		mEntity.getSubEntity(i)->setVisible(false);
	}

}

SubModel::~SubModel() {
	for (auto& entry : mSubModelParts) {
		entry.second.destroy();
	}

	Ogre::SceneManager* sceneManager = mEntity._getManager();
	sceneManager->destroyEntity(&mEntity);

}

std::map<std::string, SubModelPart>& SubModel::getSubModelPartMap() {
	return mSubModelParts;
}

SubModelPart& SubModel::createSubModelPart(const std::string& name) {
	auto result = mSubModelParts.emplace(name, SubModelPart(name, *this));
	return result.first->second;

}

Ogre::Entity* SubModel::getEntity() const {
	return &mEntity;
}


}
}
}
